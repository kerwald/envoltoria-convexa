#include "p8g/p8g.hpp"
#include "Unidade.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <string>
#include "Forma.hpp"
#include "Poligono.hpp"

#define WIDTH  960
#define LENGHT 540

Unidade *unidadeptr = nullptr;

void p8g::draw() {

    p8g::background( 17, 24, 39 );


    for( Forma forma : unidadeptr->getFormas() ){

        const std::vector< std::pair<double, double> > vertices{ forma.getVertices() };

        p8g::fill( forma.getCor()[0], forma.getCor()[1], forma.getCor()[2] );
        p8g::stroke( 255, 255, 255 ); 
        p8g::strokeWeight( 8.0 ); 


        switch ( forma.getPoligono() ){
            case Poligono::quadrado :
                p8g::rect( vertices[0].first, vertices[0].second, vertices[1].first,  vertices[1].second);
                break;
            case Poligono::retangulo:
                p8g::rect( vertices[0].first, vertices[0].second, vertices[1].first,  vertices[1].second);
                break;
            case Poligono::triangulo:
                p8g::triangle( 
                        vertices[0].first, 
                        vertices[0].second, 
                        vertices[1].first,  
                        vertices[1].second, 
                        vertices[2].first, 
                        vertices[2].second
                    );
                break;
            case Poligono::circulo:
                p8g::ellipse( vertices[0].first, vertices[0].second, vertices[1].first,  vertices[1].second);
                break;
        }
    }
    
    if( unidadeptr->getPontos().size() > 1 ){
        p8g::stroke( 255, 255, 255, 255 );
        p8g::strokeWeight( 5 );
        for( long long unsigned int i=0; i<unidadeptr->getVerticesEnvoltorio().size(); i++ ){
            std::vector< std::pair<double, double> > vertices = unidadeptr->getVerticesEnvoltorio();
            std::pair<double, double> p1{ vertices[ i ] };
            std::pair<double, double> p2{ vertices[ ( i + 1 )  % vertices.size() ] };
            line( p1.first, p1.second, p2.first, p2.second );
        }
    }    

    for( std::pair<double, double> &ponto : unidadeptr->getPontos() ){
        p8g::stroke( 255, 255, 255, 255);
        p8g::strokeWeight( 20 );
        p8g::point( ponto.first, ponto.second );
        p8g::stroke( 255, 165, 0, 255);
        p8g::strokeWeight( 15 );
        p8g::point( ponto.first, ponto.second );
    }

}

void p8g::keyPressed() {
    uint32_t valor{};
    switch ( keyCode )
    {
    case 65: // A
        unidadeptr->gerarPontosAleatorios( );
        unidadeptr->ordenaPontos( );
        unidadeptr->criaEnvoltoriaConvexa( );
        break;
    case 67: // C
        unidadeptr->setForma( unidadeptr->gerarPontoAleatorio(), 100, Poligono::circulo );
        break;
    case 81: // Q
        unidadeptr->setForma( unidadeptr->gerarPontoAleatorio(), 100, Poligono::quadrado );
        break;
    case 82: // R
        unidadeptr->setForma( unidadeptr->gerarPontoAleatorio(), 100, Poligono::retangulo );
        break;
    case 84: // T
        unidadeptr->setForma( unidadeptr->gerarPontoAleatorio(), 100, Poligono::triangulo );
        break;
    case 76: // L
        unidadeptr->clean();
        break;
    case 86: // V
        std::cout << "Digite o numero de pontos Aleatorios: " << std::endl;
        std::cin >> valor;
        unidadeptr->setNumeroDePontosAleatorios( valor );
        break;
    case 71: // G Gerar Dados
        std::cout << "Gerando arquivo de dados de custo..." << std::endl;
        unidadeptr->medirCustoECriarArquivo("custo_uniforme.csv", &Unidade::gerarPontosAleatorios);
        unidadeptr->medirCustoECriarArquivo("custo_circulo.csv", &Unidade::gerarPontosNoCirculo);
        std::cout << "Arquivo gerado! Salve-o e use no Colab." << std::endl;
        break;
    default:
        break;
    }
}
void p8g::keyReleased() {}
void p8g::mouseMoved() {}
void p8g::mousePressed() {

    unidadeptr->setPontos( std::pair<double, double>{ mouseX, mouseY } );
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
        run( WIDTH, LENGHT, "Envoltoria Convexa", true );
    }

    unidadeptr = nullptr;
    return 0;

}
