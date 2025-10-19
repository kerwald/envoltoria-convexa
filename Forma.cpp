#include "p8g/p8g.hpp"
#include "Forma.hpp"
#include "Poligono.hpp"
#include <vector>
#include <cmath>
#include <utility>

Forma::Forma( const std::pair<double, double> &pontoCentral, const double &raio, const Poligono &poligono  ) : 
    cor{ corAleatoria() }, 
    vertices{},
    poligono{ poligono }
{
    if( poligono == Poligono::triangulo ){
        gerarTriangulo( pontoCentral, raio );

    } else if( poligono == Poligono::circulo ){
        vertices.push_back( std::pair<double, double> { pontoCentral.first, pontoCentral.second } );
        vertices.push_back( std::pair<double, double> { raio, raio } );
    } else{ // Se quadrado ou retangulo
        const double lado{ ( raio / 2.0 ) + raio } ;
        vertices.push_back( std::pair<double, double> { pontoCentral.first - raio, pontoCentral.second - raio } );
        ( poligono == Poligono::quadrado ) ? 
        vertices.push_back( std::pair<double, double> { raio, raio } ) :
        vertices.push_back( std::pair<double, double> { lado, raio } ) ;
    }

}

std::vector<uint16_t> Forma::getCor() const{
    return cor;
}
std::vector< std::pair< double, double > > Forma::getVertices() const{
    return vertices;
}

Poligono Forma::getPoligono() const{
    return poligono;
}

void Forma::gerarTriangulo( const std::pair<double, double> &pontoCentral, const double &raio ){

    const double pi = std::acos(-1.0); // Constante PI
    const double pi2 = 2.0 * pi;

    const double anguloInicial = -pi / 2.0; 

    const double passoAngular = pi2 / 3.0;

    for (int i = 0; i < 3; ++i) {
        
        double anguloAtual = anguloInicial + (i * passoAngular);
        
        // Aplica a fórmula trigonométrica para encontrar as coordenadas
        vertices.push_back( std::pair<double, double> {
            pontoCentral.first + raio * std::cos( anguloAtual ),
            pontoCentral.second + raio * std::sin( anguloAtual )
        } );

    }
}

std::vector<uint16_t> Forma::corAleatoria() const{
    std::vector<uint16_t> cor;
    cor.push_back( p8g::random( 0, 255 ) );
    cor.push_back( p8g::random( 0, 255 ) );
    cor.push_back( p8g::random( 0, 255 ) );
    return cor;
}