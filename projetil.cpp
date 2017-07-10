#include "projetil.h"

Projetil::Projetil(){
	this->pos = Ponto(0, 0);
	this->velocidade = Vetor(0, 0);
	this->altura = 0;
	this->largura = 0;
	this->tipo = 0;
	this->range = 0;
	this->dano = 0;
}

Projetil::Projetil(Ponto pos, Vetor velocidade, int largura, int altura, int tipo, int dano, ALLEGRO_COLOR cor){
	this->pos = pos;
	this->velocidade = velocidade;
	this->largura = largura;
	this->altura = altura;
	this->dano = dano;
	this->tipo = tipo;
	this->range = range;
	this->cor = cor;

	verticeInicial[0] = Ponto(pos.getX()-largura/2, pos.getY()-altura/2);
	verticeInicial[1] = Ponto(pos.getX()+largura/2, pos.getY()-altura/2);
	verticeInicial[2] = Ponto(pos.getX()+largura/2, pos.getY()+altura/2);
	verticeInicial[3] = Ponto(pos.getX()-largura/2, pos.getY()+altura/2);
	for(int i = 0; i < 4; i++){
		v[i].x = verticeInicial[i].getX();
		v[i].y = verticeInicial[i].getY();
		v[i].z = 0;
		v[i].color = al_map_rgb(0, 255, 0);
		vertice[i] = verticeInicial[i];
	}

	angulo = 0;
}

void Projetil::andar(){
	pos = pos + velocidade;
	for(int i = 0; i < 4; i++){
		verticeInicial[i] = verticeInicial[i] + velocidade;
	}
}

void Projetil::desenhar(bool mostrarHitbox){
	if(tipo == circular){
		al_draw_filled_circle(pos.getX(), pos.getY(), largura*0.6, cor);
	}
	else if(tipo == retangular){
		for(int i = 0; i < 4; i++)
			v[i].color = al_map_rgb(0, 0, 255);
        al_draw_prim(v, NULL, 0, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
        for(int i = 0; i < 4; i++)
			v[i].color = al_map_rgb(0, 255, 0);
        al_draw_prim(v, NULL, 0, 0, 4, ALLEGRO_PRIM_LINE_LOOP);
	}
	if(mostrarHitbox)
		al_draw_prim(v, NULL, 0, 0, 4, ALLEGRO_PRIM_LINE_LOOP);
}

bool Projetil::operator==(Projetil p){
	return true;
}

void Projetil::setPos(Ponto pos){
	this->pos = pos;
}

Ponto Projetil::getPos(){
	return pos;
}

void Projetil::setVelocidade(Vetor velocidade){
	this->velocidade = velocidade;
}

Vetor Projetil::getVelocidade(){
	return velocidade;
}

void Projetil::setLargura(int largura){
	this->largura = largura;
}
int Projetil::getLargura(){
	return largura;
}

void Projetil::setAltura(int altura){
	this->altura = altura;
}
int Projetil::getAltura(){
	return altura;
}

void Projetil::setDano(int dano){
	this->dano = dano;
}
int Projetil::getDano(){
	return dano;
}


void Projetil::setRange(int range){
	this->range = range;
}
int Projetil::getRange(){
	return range;
}

void Projetil::atualizaAngulo(){
	angulo = Vetor(1, 0).anguloCom_(velocidade);
	for(int i = 0; i < 4; i++){
		vertice[i] = verticeInicial[i].rotacionado(pos, angulo);
		v[i].x = vertice[i].getX();
		v[i].y = vertice[i].getY();
	}
}

Ponto Projetil::getVertice(int i){
	return vertice[i];
}
