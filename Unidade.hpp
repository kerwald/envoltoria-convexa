#pragma once
#include "Forma.hpp"
#include "Poligono.hpp"
#include <vector>
#include <cstdint>
#include <utility>

class Unidade{

    private:
        std::vector<std::pair<double, double>> pontos;
        std::vector<std::pair<double, double>> verticesEnvoltorio;
        std::vector< Forma > formas;
        uint32_t numeroDePontosAleatorios;

    public:
        Unidade();
        void setPontos( const std::pair<double, double> &ponto );
        void setNumeroDePontosAleatorios( const uint32_t &numero );
        std::vector<std::pair<double, double>> getPontos() const;
        std::vector<std::pair<double, double>> getVerticesEnvoltorio() const;
        std::vector<Forma> getFormas() const;
        void setForma( const std::pair<double, double> &pontoCentral, const double &raio, const Poligono &poligono );
        int verificaSentido( const std::pair<double, double> &p1, const std::pair<double, double> &p2, const std::pair<double, double> &p3 ) const;
        void ordenaPontos( );
        void criaEnvoltoriaConvexa( );
        void gerarPontosAleatorios( );
        std::pair<double, double> gerarPontoAleatorio( );
        void clean();

};