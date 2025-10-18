#pragma once
#include "Ponto.hpp"
#include "Forma.hpp"
#include "Poligono.hpp"
#include <vector>
#include <cstdint>

class Unidade{

    private:
        std::vector<Ponto> pontos;
        std::vector<Ponto> verticesEnvoltorio;
        std::vector< Forma > formas;
        uint32_t numeroDePontosAleatorios;

    public:
        void setPontos( const Ponto &ponto );
        void setNumeroDePontosAleatorios( const uint32_t &numero );
        std::vector<Ponto> getPontos() const;
        std::vector<Ponto> getVerticesEnvoltorio() const;
        std::vector<Forma> getFormas() const;
        void setForma( const Ponto &pontoCentral, const double &raio, const Poligono &poligono );
        int verificaSentido( const Ponto &p1, const Ponto &p2, const Ponto &p3 ) const;
        void ordenaPontos( );
        void criaEnvoltoriaConvexa( );
        void gerarPontosAleatorios( const uint32_t &quantidadeDePontos );
        Ponto gerarPontoAleatorio( );
        void clean();

};