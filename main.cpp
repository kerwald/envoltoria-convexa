#include "p8g/p8g.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

struct Ponto{
    double x;
    double y;
};

struct Envoltorio{

    std::vector<Ponto> pontos;
    std::vector<Ponto> verticesEnvoltorio;

    Envoltorio() :
        pontos{}, 
        verticesEnvoltorio{} 
    {}
};

Envoltorio *envoltorioptr = nullptr;

void p8g::draw() {
    background(220);
    rect(50, 50, 100, 100);
}

void p8g::keyPressed() {}
void p8g::keyReleased() {}
void p8g::mouseMoved() {}
void p8g::mousePressed() {
    envoltorioptr->pontos.push_back( Ponto{ mouseX, mouseY } );
    ordenaPontos( envoltorioptr->pontos );
    envoltorioptr->verticesEnvoltorio = criaEnvoltoriaConvexa( envoltorioptr->pontos );
}
void p8g::mouseReleased() {}
void p8g::mouseWheel(float delta) {}

void ordenaPontos( std::vector<Ponto>  &pontos );
int verificaSentido( const Ponto &p1, const Ponto &p2, const Ponto &p3 );
std::vector<Ponto> criaEnvoltoriaConvexa( const std::vector<Ponto> &pontos );

int main() {

    Envoltorio *envoltorio = new Envoltorio();
    envoltorioptr = envoltorio;

    {
        using namespace p8g;
        run( 320, 320, "Envoltoria Convexa" );
    }

    for( Ponto ponto: envoltorio->pontos ){
        std::cout << ponto.x << " " << ponto.y << " " << std::endl;
    }




    delete envoltorio;
    return 0;
}

void ordenaPontos( std::vector<Ponto> &pontos ){

    if (pontos.size() < 2) return; 

 
    int p0_index = 0;
    for ( int i = 1; i < pontos.size(); ++i ) {
        if ( ( pontos[i].y > pontos[p0_index].y ) || 
            ( pontos[i].y == pontos[p0_index].y && pontos[i].x < pontos[p0_index].x ) ) {
            p0_index = i;
        }
    }

    std::swap( pontos[0], pontos[p0_index] );
    Ponto ponto0 = pontos[0];

    std::sort( pontos.begin() + 1, pontos.end(), 
        [ponto0]( const Ponto &a, const Ponto &b ) {
            
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

int verificaSentido( const Ponto &p1, const Ponto &p2, const Ponto &p3 ) {
    
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

std::vector<Ponto> criaEnvoltoriaConvexa( const std::vector<Ponto> &pontos ){

    std::vector<Ponto> envoltoriaConvexa{}; 
    envoltoriaConvexa.push_back( pontos[0] );
    envoltoriaConvexa.push_back( pontos[1] );

    for( size_t i = 2; i < pontos.size(); i++ ){
        
        while ( envoltoriaConvexa.size() >= 2 && 
               verificaSentido( envoltoriaConvexa[envoltoriaConvexa.size()-2], envoltoriaConvexa.back(), pontos[i] ) >= 0) 
        {
            envoltoriaConvexa.pop_back();
        }
        
        envoltoriaConvexa.push_back( pontos[i] );
    }

    return envoltoriaConvexa;

}