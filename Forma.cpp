#include "Forma.hpp"
#include "Poligono.hpp"
#include "Ponto.hpp"
#include "p8g/p8g.hpp"
#include <vector>
#include <cmath>

Forma::Forma( const Ponto &pontoCentral, const double &raio, const Poligono &poligono  ) : 
    cor{ corAleatoria() }, 
    vertices{},
    poligono{ poligono }
{
    switch ( poligono )
    {
    case Poligono::circulo:
        gerarCirculo( pontoCentral, raio );
        break;
    case Poligono::quadrado:
        gerarQuadrado( pontoCentral, raio );
        break;
    case Poligono::triangulo:
        gerarTriangulo( pontoCentral, raio );
        break;
    case Poligono::retangulo:
        gerarRetangulo( pontoCentral, raio );
    default:
        break;
    }

}

std::vector<uint16_t> Forma::getCor() const{
    return cor;
}
std::vector<Ponto> Forma::getVertices() const{
    return vertices;
}

void Forma::gerarCirculo( const Ponto &pontoCentral, const double &raio ) {

    const uint32_t numeroDePontosDasFormas{ 100 };
 
    const double pi2 = 2.0 * std::acos(-1.0); // 2 * pi
    const double passoAngular = pi2 / numeroDePontosDasFormas;

    for ( uint32_t i=0; i < numeroDePontosDasFormas; ++i ) {

        // O ângulo atual (em radianos)
        double angulo = passoAngular * i;
        
        // Aplica a fórmula trigonométrica para as coordenadas (x, y)
        // do círculo centrado em (0, 0)
        // Translada as coordenadas para o centro desejado
        Ponto ponto{ raio * std::cos( angulo ) + pontoCentral.x, raio * std::sin( angulo ) + pontoCentral.y };
        
        vertices.push_back( ponto );
    }

}

void Forma::gerarQuadrado( const Ponto &pontoCentral, const double &raio ){

    vertices.push_back(  Ponto{ pontoCentral.x - raio, pontoCentral.y - raio } );
    vertices.push_back(  Ponto{ pontoCentral.x - raio, pontoCentral.y + raio } );
    vertices.push_back(  Ponto{ pontoCentral.x + raio, pontoCentral.y + raio } );
    vertices.push_back(  Ponto{ pontoCentral.x + raio, pontoCentral.y - raio } );

}

void Forma::gerarRetangulo( const Ponto &pontoCentral, const double &raio ){

    std::vector<Ponto> retangulo{};
    const double lado = ( raio / 2.0) + raio;

    vertices.push_back(  Ponto{ pontoCentral.x - lado, pontoCentral.y - raio } );
    vertices.push_back(  Ponto{ pontoCentral.x - lado, pontoCentral.y + raio } );
    vertices.push_back(  Ponto{ pontoCentral.x + lado, pontoCentral.y + raio } );
    vertices.push_back(  Ponto{ pontoCentral.x + lado, pontoCentral.y - raio } );

}

void Forma::gerarTriangulo( const Ponto &pontoCentral, const double &raio ){

    const double pi = std::acos(-1.0); // Constante PI
    const double pi2 = 2.0 * pi;
    
    // CORREÇÃO: Usar -PI/2 (-90 graus) em vez de PI/2 (90 graus)
    // Isso move o primeiro vértice para a posição 6 horas, fazendo a base ficar no topo.
    const double anguloInicial = -pi / 2.0; 
    
    // Passo angular de 120 graus (2*PI / 3.0)
    const double passoAngular = pi2 / 3.0;

    for (int i = 0; i < 3; ++i) {
        
        double anguloAtual = anguloInicial + (i * passoAngular);
        
        // Aplica a fórmula trigonométrica para encontrar as coordenadas
        vertices.push_back( Ponto{
            pontoCentral.x + raio * std::cos( anguloAtual ),
            pontoCentral.y + raio * std::sin( anguloAtual )
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