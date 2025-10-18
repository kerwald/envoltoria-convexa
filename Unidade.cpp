#include "Unidade.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include "Ponto.hpp"

std::random_device Unidade::rd;
std::mt19937 Unidade::gerador( Unidade::rd() );
std::uniform_real_distribution<> Unidade::dist_x( 0.0, 1920.0 );
std::uniform_real_distribution<> Unidade::dist_y( 0.0, 1080.0 );

void Unidade::setPontos( const Ponto &ponto ){
    pontos.push_back( ponto );
}

std::vector<Ponto> Unidade::getPontos() const{
    return pontos;
}

std::vector<Ponto> Unidade::getVerticesEnvoltorio() const{
    return verticesEnvoltorio;
}
std::vector< std::vector<Ponto> > Unidade::getFormas() const{
    return formas;
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

void Unidade::gerarCirculo( const Ponto pontoCentral, const double &raio, const uint32_t &numPontos ) {

    std::vector<Ponto> circulo{};
    const double pi2 = 2.0 * std::acos(-1.0); // 2 * pi
    const double passoAngular = pi2 / numPontos;

    for ( uint32_t i=0; i < numPontos; ++i ) {

        // O ângulo atual (em radianos)
        double angulo = passoAngular * i;
        
        // Aplica a fórmula trigonométrica para as coordenadas (x, y)
        // do círculo centrado em (0, 0)
        // Translada as coordenadas para o centro desejado
        Ponto ponto{ raio * std::cos( angulo ) + pontoCentral.x, raio * std::sin( angulo ) + pontoCentral.y };
        
        circulo.push_back( ponto );
    }

    formas.push_back( circulo );

}

int Unidade::verificaSentido( const Ponto &p1, const Ponto &p2, const Ponto &p3 ) const{
    
    double valor = (double) ( p2.x - p1.x ) * ( p3.y - p1.y ) -
                (double) ( p2.y - p1.y ) * ( p3.x - p1.x );

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
        if ( ( pontos[i].y > pontos[p0_index].y ) || 
            ( pontos[i].y == pontos[p0_index].y && pontos[i].x < pontos[p0_index].x ) ) {
            p0_index = i;
        }
    }

    std::swap( pontos[0], pontos[p0_index] );
    Ponto ponto0 = pontos[0];

    std::sort( pontos.begin() + 1, pontos.end(),

        [ ponto0, this ]( const Ponto &a, const Ponto &b ) {
            
            int sentido = verificaSentido( ponto0, a, b );

            if ( sentido == 0 ) { // Pontos colineares
                // Retorna true se 'a' for mais próximo de 'ponto0' que 'b'
                float distA = ( a.x - ponto0.x ) * ( a.x - ponto0.x ) + ( a.y - ponto0.y ) * ( a.y - ponto0.y );
                float distB = ( b.x - ponto0.x ) * ( b.x - ponto0.x ) + ( b.y - ponto0.y ) * ( b.y - ponto0.y );
                return distA < distB;
            }

            // Se for anti-horário (sentido < 0), 'a' vem antes de 'b'.
            // Se for horário (sentido > 0), 'a' vem depois de 'b'.
            return sentido < 0;
        }
    );
}

void Unidade::gerarPontosAleatorios( const uint32_t &quantidadeDePontos ){
    
    for( uint32_t i = 0; i < quantidadeDePontos; i++ ){
        Ponto pontoAleatorio = gerarPontoAleatorio();
        pontos.push_back( pontoAleatorio );
    }

}

Ponto Unidade::gerarPontoAleatorio(){
    // Gera as coordenadas X e Y, usando o gerador estático de classe
    double aleatorioX = dist_x( gerador );
    double aleatorioY = dist_y( gerador );
    return Ponto{ aleatorioX, aleatorioY };
}

void Unidade::clean(){
    pontos.clear();
    formas.clear();
    verticesEnvoltorio.clear();
}
