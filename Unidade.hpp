#pragma once
#include "Ponto.hpp"
#include <vector>
#include <cstdint>

class Unidade{

    private:
        std::vector<Ponto> pontos;
        std::vector<Ponto> verticesEnvoltorio;
        std::vector< std::vector<Ponto> > formas;
        std::vector<Ponto> pontosAleatorios;

    public:
        void setPontos( const Ponto &ponto );
        std::vector<Ponto> getPontos() const;
        std::vector<Ponto> getVerticesEnvoltorio() const;
        std::vector< std::vector<Ponto> > getFormas() const;
        std::vector<Ponto> getPontosAleatorios() const;
        int verificaSentido( const Ponto &p1, const Ponto &p2, const Ponto &p3 ) const;
        void ordenaPontos( );
        void criaEnvoltoriaConvexa( );
        void gerarCirculo( const Ponto pontoCentral, const double &raio, const uint32_t &numPontos );

};