#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

void ordenaPontos( std::vector<sf::Vector2f>  &pontos );
int verificaSentido( sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3 );
std::vector<sf::Vector2f> criaEnvoltoriaConvexa( const std::vector<sf::Vector2f> &pontos );



int main() {

    std::vector<sf::Vector2f>  pontos{};
    std::vector<sf::Vector2f>  envoltoriaConvexa{};

    sf::RenderWindow window( sf::VideoMode::getDesktopMode(), "Envoltoria Convexa - C++/SFML", sf::State::Fullscreen );
    window.setFramerateLimit( 60 );

    while ( window.isOpen() ) {

        while ( std::optional<sf::Event> optEvent = window.pollEvent() ) {
            const sf::Event& event = *optEvent;

            if ( event.is<sf::Event::Closed>() ) {
                window.close();
            }

            if ( const auto* keyPressed = event.getIf<sf::Event::KeyPressed>() ) {
                if ( keyPressed->code == sf::Keyboard::Key::Escape ) {
                    window.close();
                }
            }

            if ( const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>() ) {

                if ( mouseEvent->button == sf::Mouse::Button::Left ) {
                    
                    sf::Vector2i pixelPos = mouseEvent->position;
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    pontos.push_back( worldPos );

                    ordenaPontos( pontos );
                    if( pontos.size() > 2 ){
                        envoltoriaConvexa = criaEnvoltoriaConvexa( pontos );
                    }

                }
            }
        }


        window.clear(); 

       sf::VertexArray linhas( sf::PrimitiveType::LineStrip );

        if ( envoltoriaConvexa.size() > 1){           

            for ( const sf::Vector2f& ponto : envoltoriaConvexa ) {
                sf::Vertex vertice;
                vertice.position = ponto;
                vertice.color = sf::Color::Green;
                linhas.append(vertice);
            }

            if ( !envoltoriaConvexa.empty() ) {
                sf::Vertex verticeFinal;
                verticeFinal.position = envoltoriaConvexa[0];
                verticeFinal.color = sf::Color::Green;
                linhas.append( verticeFinal );
            }

            window.draw( linhas );
        }



        for ( const sf::Vector2f& ponto : pontos ) {

            sf::CircleShape pontoShape( 5.f );
            pontoShape.setFillColor( sf::Color::White );
            pontoShape.setOrigin( sf::Vector2f( 5.f, 5.f ) );
            pontoShape.setPosition( ponto ); 
            window.draw( pontoShape );   

        }
  

        window.display();
    }
    return 0;
}

void ordenaPontos( std::vector<sf::Vector2f>  &pontos ){

    sf::Vector2f ponto0 = pontos[0];

    for( sf::Vector2f ponto : pontos ){

        if( ponto == ponto0 ){
            continue;
        }

        if( ( ponto.y > ponto0.y ) || ( ponto.y == ponto0.y && ponto.x < ponto0.x ) ){
            ponto0 = ponto;
        }

    }

    std::sort( pontos.begin(), pontos.end(),
        [ ponto0 ]( sf::Vector2f a, sf::Vector2f b ) {

            if (a == ponto0) return true;
            if (b == ponto0) return false;

            float anguloA = atan2( a.y - ponto0.y, a.x - ponto0.x );
            
            float anguloB = atan2( b.y - ponto0.y, b.x - ponto0.x );

            if ( anguloA != anguloB ) {
                return anguloA < anguloB;
            }

            // Se os ângulos forem iguais
            // o ponto mais próximo do ponto0 vem primeiro.
            float distA = (a.x - ponto0.x)*(a.x - ponto0.x) + (a.y - ponto0.y)*(a.y - ponto0.y);
            float distB = (b.x - ponto0.x)*(b.x - ponto0.x) + (b.y - ponto0.y)*(b.y - ponto0.y);
            return distA < distB;

        }
    );
}

int verificaSentido( sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3 ) {
    
    // Usamos double para maior precisão no cálculo intermediário
    double valor = (double)(p2.x - p1.x) * (p3.y - p1.y) -
                   (double)(p2.y - p1.y) * (p3.x - p1.x);

    // Definimos uma pequena tolerância (epsilon) para lidar com erros de ponto flutuante
    const double epsilon = 1e-9;

    if ( std::fabs(valor) < epsilon ) {
        return 0; // Colinear
    } else if ( valor > 0 ) {
        return 1; // Horário 
    } else {
        return -1; // Anti-horário 
    }
}

std::vector<sf::Vector2f> criaEnvoltoriaConvexa( const std::vector<sf::Vector2f> &pontos ){

    std::vector<sf::Vector2f> envoltoriaConvexa{}; 
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