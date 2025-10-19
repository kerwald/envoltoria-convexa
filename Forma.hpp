#pragma once
#include <cstdint>
#include <vector>
#include <utility>
#include "Poligono.hpp"

class Forma{
    private:
        std::vector<uint16_t> cor;
        std::vector< std::pair<double, double> > vertices;
        Poligono poligono;

    public:
        Forma( const std::pair<double, double> &pontoCentral, const double &raio, const Poligono &poligono );
        std::vector<uint16_t> getCor() const;
        std::vector< std::pair<double, double> > getVertices() const;
        Poligono getPoligono() const;

    private:
        void gerarTriangulo( const std::pair<double, double> &pontoCentral, const double &raio );
        std::vector<uint16_t> corAleatoria() const;

};