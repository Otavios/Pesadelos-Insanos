#ifndef INIMIGO_H
#define INIMIGO_H

#include "includes.h"
#include "timer.h"
#include "lista.h"
#include "projetil.h"
#include "barra.h"

using namespace geometria;

template <class T>
class Lista;

class Inimigo{

	private:
		Ponto pos, verticeInicial[4], vertice[4];
		Vetor velocidade;
		double moduloVelocidade;
		double largura, altura;
		int tipo;
		int vida;
		Timer cooldown;
		Timer timerAnimacaoAndar;
		int sentidoRotacao;
		ALLEGRO_VERTEX v[4];
		static ALLEGRO_BITMAP *spritePalhaco, *spriteBp, *spriteAranha, *spriteFoguinho, *spriteMariposa, *spriteMariposaBoss;
		double angulo;
		Barra barraVida;
		Timer tempoMovimento; // tipo4
		double transparencia;
		Timer timerAtaquesBoss1; // ataques boss 1
		Timer timerAtaquesBoss2;
		bool vaiNoPlayer; // mariposa

	public:
		Inimigo();
		Inimigo(Ponto, Vetor, double, int, int, int);
		void desenhar(bool mostrarHitBox);
		void andar(Ponto posicao_player, int larguraTela, int alturaTela);
		void atirar(Lista<Projetil>& listaProjeteisInimigos, Ponto ponto_player, Lista<Inimigo>& listaInimigos);

		Ponto getPos();

		bool operator==(Inimigo);

		int getVida();
		void setVida(int vida);

		void recebeDano(int dano);

		void setLargura(int largura);
		int getLargura();
		void setAltura(int altura);
		int getAltura();

		void atualiza(Lista<Projetil>& listaProjeteisInimigos, Ponto ponto_player, Lista<Inimigo>& listaInimigos);

		Ponto getVertice(int i);

		void atualizaAngulo();

		void desenhaBarraVida();

		static void initImagens();
};

#endif
