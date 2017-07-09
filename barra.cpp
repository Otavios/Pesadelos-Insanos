#include "barra.h"

Barra::Barra(){
	
}

Barra::Barra(Ponto pos, int largura, int altura, double valorMax, double valorAtual, ALLEGRO_COLOR corDentro, ALLEGRO_COLOR corBorda, double velocidadeAnimacao, int grossuraBorda){
	this->pos = pos;
	this->largura = largura;
	this->altura = altura;
	this->valorMax = valorMax;
	this->valorAtual = valorAtual;
	this->corDentro = corDentro;
	this->corBorda = corBorda;
	decremento = 0;
	incremento = 0;
	this->velocidadeAnimacao = velocidadeAnimacao;
	this->grossuraBorda = grossuraBorda;
}

void Barra::desenhar(){
	if(valorAtual > 0)
		al_draw_filled_rounded_rectangle(pos.getX()-largura/2, pos.getY()-altura/2, pos.getX()-largura/2+valorAtual*largura/valorMax, pos.getY()+altura/2, 2, 2, corDentro);
	al_draw_rounded_rectangle(pos.getX()-largura/2, pos.getY()-altura/2, pos.getX()+largura/2, pos.getY()+altura/2, 2, 2, corBorda, grossuraBorda);
}

void Barra::atualizar(Ponto p){
	if(decremento > 0){
		valorAtual -= velocidadeAnimacao;
		decremento -= velocidadeAnimacao;
		if(decremento < 0){
			valorAtual -= decremento;
			decremento = 0;
		}
	}else{
		decremento = 0;
	}
	if(incremento > 0){
		valorAtual += velocidadeAnimacao;
		incremento -= velocidadeAnimacao;
		if(incremento < 0){
			valorAtual += incremento;
			incremento = 0;
		}
	}else{
		incremento = 0;
	}
	pos = p;
}

void Barra::diminui(double v){
	decremento += v;
}

void Barra::aumenta(double v){
	incremento += v;
}
