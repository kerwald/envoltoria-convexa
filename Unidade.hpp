#pragma once
#include "Ponto.hpp"
#include <vector>
#include <cstdint>
#include <random>

class Unidade{

    private:
        std::vector<Ponto> pontos;
        std::vector<Ponto> verticesEnvoltorio;
        std::vector< std::vector<Ponto> > formas;
        static std::random_device rd;
        static std::mt19937 gerador;
        static std::uniform_real_distribution<> dist_x;
        static std::uniform_real_distribution<> dist_y;

    public:
        void setPontos( const Ponto &ponto );
        std::vector<Ponto> getPontos() const;
        std::vector<Ponto> getVerticesEnvoltorio() const;
        std::vector< std::vector<Ponto> > getFormas() const;
        int verificaSentido( const Ponto &p1, const Ponto &p2, const Ponto &p3 ) const;
        void ordenaPontos( );
        void criaEnvoltoriaConvexa( );
        void gerarCirculo( const Ponto pontoCentral, const double &raio, const uint32_t &numPontos );
        void gerarPontosAleatorios( const uint32_t &quantidadeDePontos );
        Ponto gerarPontoAleatorio( );
        void clean();
};