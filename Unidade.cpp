#include "Unidade.hpp"
#include "p8g/p8g.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include "Forma.hpp"
#include "Poligono.hpp"
#include <utility>
#include <chrono> 
#include <fstream> 
#include <string>

#define WIDTH  960.0
#define LENGHT 540.0

Unidade::Unidade() : numeroDePontosAleatorios( 25 )
{}

void Unidade::setPontos( const std::pair<double, double> &ponto ){
    pontos.push_back( ponto );
}

void Unidade::setNumeroDePontosAleatorios( const uint32_t &numero ){
    numeroDePontosAleatorios = numero;
}

std::vector<std::pair<double, double>> Unidade::getPontos() const{
    return pontos;
}

std::vector<std::pair<double, double>> Unidade::getVerticesEnvoltorio() const{
    return verticesEnvoltorio;
}
std::vector<Forma> Unidade::getFormas() const{
    return formas;
}

void Unidade::setForma( const std::pair<double, double> &pontoCentral, const double &raio, const Poligono &poligono ){
    formas.push_back( Forma{ pontoCentral, raio, poligono } );
}
    
void Unidade::criaEnvoltoriaConvexa( ){

    verticesEnvoltorio = {};

    verticesEnvoltorio.push_back( pontos[0] );
    verticesEnvoltorio.push_back( pontos[1] );

    for( size_t i = 2; i < pontos.size(); i++ ){
        
        while ( verticesEnvoltorio.size() >= 2 && 
            verificaSentido( verticesEnvoltorio[ verticesEnvoltorio.size()-2], verticesEnvoltorio.back(), pontos[i] ) >= 0) 
        {
            verticesEnvoltorio.pop_back();
        }
        
        verticesEnvoltorio.push_back( pontos[i] );
    }

}

int Unidade::verificaSentido( const std::pair<double, double> &p1, const std::pair<double, double> &p2, const std::pair<double, double> &p3 ) const{
    
    double valor = (double) ( p2.first - p1.first ) * ( p3.second - p1.second ) -
                (double) ( p2.second - p1.second ) * ( p3.first - p1.first );

    // pequena tolerância (epsilon) para lidar com erros de ponto flutuante
    const double epsilon = 1e-9;

    if ( std::fabs( valor ) < epsilon ) {
        return 0; // Colinear
    } else if ( valor > 0 ) {
        return 1; // Horário 
    } else {
        return -1; // Anti-horário 
    }
}
    
void Unidade::ordenaPontos( ){

    if (pontos.size() < 2) return; 


    int p0_index = 0;
    for ( unsigned long long int i = 1; i < pontos.size(); ++i ) {
        if ( ( pontos[i].second > pontos[p0_index].second ) || 
            ( pontos[i].second == pontos[p0_index].second && pontos[i].first < pontos[p0_index].first ) ) {
            p0_index = i;
        }
    }

    std::swap( pontos[0], pontos[p0_index] );
    std::pair<double, double> ponto0 = pontos[0];

    std::sort( pontos.begin() + 1, pontos.end(),

        [ ponto0, this ]( const std::pair<double, double> &a, const std::pair<double, double> &b ) {
            
            int sentido = verificaSentido( ponto0, a, b );

            if ( sentido == 0 ) { // Pontos colineares
                // Retorna true se 'a' for mais próximo de 'ponto0' que 'b'
                float distA = ( a.first - ponto0.first ) * ( a.first - ponto0.first ) + ( a.second - ponto0.second ) * ( a.second - ponto0.second );
                float distB = ( b.first - ponto0.first ) * ( b.first - ponto0.first ) + ( b.second - ponto0.second ) * ( b.second - ponto0.second );
                return distA < distB;
            }

            // Se for anti-horário (sentido < 0), 'a' vem antes de 'b'.
            // Se for horário (sentido > 0), 'a' vem depois de 'b'.
            return sentido < 0;
        }
    );
}

void Unidade::gerarPontosAleatorios( ){
    
    for( uint32_t i = 0; i < numeroDePontosAleatorios; i++ ){
        std::pair<double, double> pontoAleatorio = gerarPontoAleatorio();
        pontos.push_back( pontoAleatorio );
    }

}

std::pair<double, double> Unidade::gerarPontoAleatorio(){
    double aleatorioX = p8g::random( 0.0, WIDTH );
    double aleatorioY = p8g::random( 0.0, LENGHT );
    return std::pair<double, double> { aleatorioX, aleatorioY };
}

void Unidade::clean(){
    pontos.clear();
    formas.clear();
    verticesEnvoltorio.clear();
}


void Unidade::medirCustoECriarArquivo( const std::string& nomeArquivo, void (Unidade::*gerador)() ){

    std::ofstream arquivoSaida( nomeArquivo );
    // Novo cabeçalho inclui a métrica H (Vertices na Envoltória)
    arquivoSaida << "Tamanho_N,Tempo_ms,Vertices_H" << std::endl; 

    // ** CORREÇÃO APLICADA AQUI: Limite razoável para N **
    for (uint32_t N = 100; N <= 200000; N += 500) {

        setNumeroDePontosAleatorios( N );
        
        long long tempoTotal_ms = 0;
        uint32_t H_total = 0;
        const int numRepeticoes = 5; 

        for (int rep = 0; rep < numRepeticoes; ++rep) {
            clean(); 
            // Chama a função de geração passada como parâmetro (aleatório ou círculo)
            (this->*gerador)(); 

            auto inicio = std::chrono::high_resolution_clock::now(); 

            ordenaPontos();
            criaEnvoltoriaConvexa();

            auto fim = std::chrono::high_resolution_clock::now(); 

            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
            
            tempoTotal_ms += duracao.count();
            H_total += verticesEnvoltorio.size(); // SOMA DOS VÉRTICES (H)
        }

        long long tempoMedio_ms = tempoTotal_ms / numRepeticoes;
        uint32_t H_medio = H_total / numRepeticoes; // MÉDIA DE H

        // Registra N, Tempo e H
        arquivoSaida << N << "," << tempoMedio_ms << "," << H_medio << std::endl;
    }

    arquivoSaida.close();
}

std::pair<double, double> Unidade::gerarPontoNoCirculo( double centroX, double centroY, double raio ) {

    const double pi{ std::acos(-1.0) };
    double angulo = p8g::random( 0.0, 2.0 * pi );

    // Converte de coordenadas polares para cartesianas
    double x = centroX + raio * std::cos( angulo );
    double y = centroY + raio * std::sin( angulo );

    return std::pair<double, double> { x, y };
}

void Unidade::gerarPontosNoCirculo( ){
    // Define o centro e o raio do círculo
    const double centroX = WIDTH / 2.0;
    const double centroY = LENGHT / 2.0;
    // Escolha um raio que caiba na sua tela
    const double raio = std::min( WIDTH, LENGHT ) * 0.4; 
    
    // Gera N pontos (onde N é o valor de numeroDePontosAleatorios)
    for( uint32_t i = 0; i < numeroDePontosAleatorios; i++ ){
        std::pair<double, double> pontoCirculo = gerarPontoNoCirculo( centroX, centroY, raio );
        pontos.push_back( pontoCirculo );
    }
}