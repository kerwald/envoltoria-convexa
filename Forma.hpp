#pragma once
#include <cstdint>
#include <vector>
#include "Ponto.hpp"
#include "Poligono.hpp"

class Forma{
    private:
        std::vector<uint16_t> cor;
        std::vector<Ponto> vertices;
        Poligono poligono;

    public:
        Forma( const Ponto &pontoCentral, const double &raio, const Poligono &poligono );
        std::vector<uint16_t> getCor() const;
        std::vector<Ponto> getVertices() const;

    private:
        void gerarCirculo( const Ponto &pontoCentral, const double &raio ); 
        void gerarQuadrado( const Ponto &pontoCentral, const double &raio );
        void gerarRetangulo( const Ponto &pontoCentral, const double &raio );
        void gerarTriangulo( const Ponto &pontoCentral, const double &raio );
        std::vector<uint16_t> corAleatoria() const;

};