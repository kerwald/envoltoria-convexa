#pragma once
#include "Unidade.hpp"
#include "p8g/p8g.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include "Ponto.hpp"
#include "Forma.hpp"
#include "Poligono.hpp"

void Unidade::setPontos( const Ponto &ponto ){
    pontos.push_back( ponto );
}

void Unidade::setNumeroDePontosAleatorios( const uint32_t &numero ){
    numeroDePontosAleatorios = numero;
}

std::vector<Ponto> Unidade::getPontos() const{
    return pontos;
}

std::vector<Ponto> Unidade::getVerticesEnvoltorio() const{
    return verticesEnvoltorio;
}
std::vector<Forma> Unidade::getFormas() const{
    return formas;
}

void Unidade::setForma( const Ponto &pontoCentral, const double &raio, const Poligono &poligono ){
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
    double aleatorioX = p8g::random( 0.0, 1920.0 );
    double aleatorioY = p8g::random( 0.0, 1080.0 );
    return Ponto{ aleatorioX, aleatorioY };
}

void Unidade::clean(){
    pontos.clear();
    formas.clear();
    verticesEnvoltorio.clear();
}