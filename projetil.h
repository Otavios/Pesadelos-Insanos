#ifndef PROJETIL_H
#define PROJETIL_H

#include "includes.h"

class Projetil{
	private:
		int largura, altura, tipo, range, dano;
		Ponto pos, verticeInicial[4], vertice[4];
		Vetor velocidade;
		ALLEGRO_VERTEX v[4];
		double angulo;
		ALLEGRO_COLOR cor;
		
	public:
		Projetil();
		Projetil(Ponto pos, Vetor velocidade, int largura, int altura, int tipo, int dano, ALLEGRO_COLOR cor);
		void andar();
		void desenhar(bool mostrarHitbox);
		bool operator==(Projetil);
		
		void setPos(Ponto pos);
		Ponto getPos();
		void setVelocidade(Vetor velocidade);
		Vetor getVelocidade();
		void setLargura(int largura);
		int getLargura();
		void setAltura(int altura);
		int getAltura();
		void setTipo(int tipo);
		int getTipo();
		void setDano(int dano);
		int getDano();
		void setRange(int range);
		int getRange();
		
		Ponto getVertice(int i);
		
		void atualizaAngulo();
			
};


#endif
