#include "p8g/p8g.hpp"
#include "Unidade.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include "Ponto.hpp"


Unidade *unidadeptr = nullptr;

void p8g::draw() {

    p8g::background( 17, 24, 39 );

    for( std::vector<Ponto> forma: unidadeptr->getFormas() ){
        p8g::stroke( 255, 255, 255, 255 );
        p8g::strokeWeight( 5 );
        for( long long unsigned int i=0; i<forma.size(); i++ ){
            Ponto p1{ forma[ i ] };
            Ponto p2{ forma[ ( i + 1 )  % forma.size() ] };
            line( p1.x, p1.y, p2.x, p2.y );
        }
    }
    
    if( unidadeptr->getPontos().size() > 1 ){
        p8g::stroke( 255, 255, 255, 255 );
        p8g::strokeWeight( 5 );
        for( long long unsigned int i=0; i<unidadeptr->getVerticesEnvoltorio().size(); i++ ){
            std::vector<Ponto> vertices = unidadeptr->getVerticesEnvoltorio();
            Ponto p1{ vertices[ i ] };
            Ponto p2{ vertices[ ( i + 1 )  % vertices.size() ] };
            line( p1.x, p1.y, p2.x, p2.y );
        }
    }    

    for( Ponto &ponto : unidadeptr->getPontos() ){
        p8g::stroke( 255, 255, 255, 255);
        p8g::strokeWeight( 20 );
        p8g::point( ponto.x, ponto.y );
        p8g::stroke( 255, 165, 0, 255);
        p8g::strokeWeight( 15 );
        p8g::point( ponto.x, ponto.y );
    }

}

void p8g::keyPressed() {
    switch ( keyCode )
    {
    case 65:
        unidadeptr->gerarPontosAleatorios( 50 );
        unidadeptr->ordenaPontos( );
        unidadeptr->criaEnvoltoriaConvexa( );
        break;
    case 67:
        unidadeptr->gerarCirculo( unidadeptr->gerarPontoAleatorio(), 100, 100 );
        break;
    case 76:
        unidadeptr->clean();
        break;
    default:
        break;
    }
}
void p8g::keyReleased() {}
void p8g::mouseMoved() {}
void p8g::mousePressed() {

    unidadeptr->setPontos( Ponto{ mouseX, mouseY } );
    unidadeptr->ordenaPontos( );
    unidadeptr->criaEnvoltoriaConvexa( );

}
void p8g::mouseReleased() {}
void p8g::mouseWheel(float delta) {}

int main() {

    Unidade unidade{};
    unidadeptr = &unidade;

    {
        using namespace p8g;
        run( 1920, 1080, "Envoltoria Convexa", true );
    }

    unidadeptr = nullptr;
    return 0;

}
