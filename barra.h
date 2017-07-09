#ifndef BARRA_H
#define BARRA_H

#include "includes.h"

class Barra{
	private:
		Ponto pos;
		int largura, altura, grossuraBorda;
		ALLEGRO_COLOR corBorda, corDentro;
		int decremento, incremento, velocidadeAnimacao, valorMax, valorAtual;
	public:
		Barra();
		Barra(Ponto pos, int largura, int altura, double valorMax, double valorAtual, ALLEGRO_COLOR corDentro, ALLEGRO_COLOR corBorda, double velocidadeAnimacao, int grossuraBorda);	
		void desenhar();
		void atualizar(Ponto);
		void diminui(double v);
		void aumenta(double v);
};


#endif
