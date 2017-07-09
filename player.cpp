#include "player.h"

using namespace geometria;

Player::Player(){
	pos = Ponto(0, 0);
	velocidade = 0;
	largura = 0;
	altura = 0;
	arma = 0;
	dinheiro = 0;
}

Player::Player(Ponto p, int v, double l, double a, int vida, int arma, int dinheiro){
	pos = p;
	velocidade = v;
	largura = l;
	altura = a;
	this->vida = vida;
	this->vidaMax = 6;
	tempoInvulnerabilidade = Timer(1.5*60);
	timerAnimacaoAndar = Timer(0.05*8*60-1);
	setArma(arma);
	this->dinheiro = dinheiro;
	angulo = 0;
	
}

void Player::init(){
	verticeInicial[0] = Ponto(pos.getX()-largura/2, pos.getY()-altura/2);
	verticeInicial[1] = Ponto(pos.getX()+largura/2, pos.getY()-altura/2);
	verticeInicial[2] = Ponto(pos.getX()+largura/2, pos.getY()+altura/2);
	verticeInicial[3] = Ponto(pos.getX()-largura/2, pos.getY()+altura/2);
	for(int i = 0; i < 4; i++){
		v[i].x = verticeInicial[i].getX();
		v[i].y = verticeInicial[i].getY();
		v[i].z = 0;
		v[i].color = al_map_rgb(0, 0, 255);
		vertice[i] = verticeInicial[i];
	}
}

void Player::desenhar(bool movimento, bool mostrarHitbox){

	switch(arma){
		case pistola:	
			if(!movimento){
				al_draw_tinted_scaled_rotated_bitmap_region(spritePistola, 0, 3, 58, 69, 
					al_map_rgb(255, 255, 255), 18, 34.5, pos.getX(), pos.getY(), 0.5, 0.5, -angulo, 0);
			}
			else{
				al_draw_tinted_scaled_rotated_bitmap_region(spritePistola, timerAnimacaoAndar.getContador()/3*63, 0, 58, 69,
					al_map_rgb(255, 255, 255), 18, 34.5, pos.getX(), pos.getY(), 0.5, 0.5, -angulo, 0);
				if(!timerAnimacaoAndar.estaAtivo())
			       timerAnimacaoAndar.start();
			}
			break;
		case rpg:
			if(!movimento){
				al_draw_tinted_scaled_rotated_bitmap_region(spriteRPG, 0, 0, 63, 69, 
					al_map_rgb(255, 255, 255), 18, 34.5, pos.getX(), pos.getY(), 0.5, 0.5, -angulo, 0);
			}
			else{
				al_draw_tinted_scaled_rotated_bitmap_region(spriteRPG, timerAnimacaoAndar.getContador()/3*63, 0, 63, 69,
					al_map_rgb(255, 255, 255), 18, 34.5, pos.getX(), pos.getY(), 0.5, 0.5, -angulo, 0);
				if(!timerAnimacaoAndar.estaAtivo())
			       timerAnimacaoAndar.start();
			}
			break;
		case sub:
			if(!movimento){
				al_draw_tinted_scaled_rotated_bitmap_region(spriteSub, 0, 0, 63, 69, 
					al_map_rgb(255, 255, 255), 18, 34.5, pos.getX(), pos.getY(), 0.5, 0.5, -angulo, 0);
			}
			else{
				al_draw_tinted_scaled_rotated_bitmap_region(spriteSub, timerAnimacaoAndar.getContador()/3*63, 0, 63, 69,
					al_map_rgb(255, 255, 255), 18, 34.5, pos.getX(), pos.getY(), 0.5, 0.5, -angulo, 0);
				if(!timerAnimacaoAndar.estaAtivo())
			       timerAnimacaoAndar.start();
			}
			break;
		case shotgun:
			if(!movimento){
				al_draw_tinted_scaled_rotated_bitmap_region(spriteShotgun, 0, 0, 63, 69, 
					al_map_rgb(255, 255, 255), 18, 34.5, pos.getX(), pos.getY(), 0.5, 0.5, -angulo, 0);
			}
			else{
				al_draw_tinted_scaled_rotated_bitmap_region(spriteShotgun, timerAnimacaoAndar.getContador()/3*63, 0, 63, 69,
					al_map_rgb(255, 255, 255), 18, 34.5, pos.getX(), pos.getY(), 0.5, 0.5, -angulo, 0);
				if(!timerAnimacaoAndar.estaAtivo())
			       timerAnimacaoAndar.start();
			}
			break;
	}
	
	if(mostrarHitbox)
		al_draw_prim(v, NULL, 0, 0, 4, ALLEGRO_PRIM_LINE_LOOP);
}

void Player::andar(bool cima, bool baixo, bool esquerda, bool direita, int larguraTela, int alturaTela, Ponto mouse){
	Ponto dif = pos;
	if(cima){
		pos = pos - Vetor(0, velocidade);
		if(pos.getY() < 0)
			pos.setY(0);
	}
	if(baixo){
		pos = pos + Vetor(0, velocidade);
		if(pos.getY() > alturaTela)
			pos.setY(alturaTela);
	}	
	if(direita){
		pos = pos + Vetor(velocidade, 0);
		if(pos.getX() > larguraTela)
			pos.setX(larguraTela);
	}
	if(esquerda){
		pos = pos - Vetor(velocidade, 0);
		if(pos.getX() < 0)
			pos.setX(0);
	}
	dif = dif-Vetor(Ponto(0, 0), pos);
	for(int i = 0; i < 4; i++){
		verticeInicial[i] = verticeInicial[i] - Vetor(Ponto(0, 0), dif);
	}
	
}

void Player::atirar(Lista<Projetil>& listaProjetil, Ponto ponto_mouse, bool click){
	if(!tempoRecarga.estaAtivo() || click){
		switch(arma){
			case pistola:
				listaProjetil.insereNoInicio(Projetil(pos, Vetor(pos, ponto_mouse).normalizado()*8, 10, 10, circular, 1, al_map_rgb(0, 0, 155)));
				break;
			case shotgun:
				listaProjetil.insereNoInicio(Projetil(pos, Vetor(pos, ponto_mouse).normalizado()*12, 20, 20, circular, 5, al_map_rgb(0, 0, 155)));
				listaProjetil.insereNoInicio(Projetil(pos, Vetor(pos, ponto_mouse).rotacionado(0.3).normalizado()*12, 20, 20, circular, 5, al_map_rgb(0, 0, 155)));
				listaProjetil.insereNoInicio(Projetil(pos, Vetor(pos, ponto_mouse).rotacionado(-0.3).normalizado()*12, 20, 20, circular, 5, al_map_rgb(0, 0, 155)));
				break;
			case sub:
				listaProjetil.insereNoInicio(Projetil(pos, Vetor(pos, ponto_mouse).normalizado()*12, 20, 20, circular, 5, al_map_rgb(0, 0, 155)));
				break;
			case rpg:
				listaProjetil.insereNoInicio(Projetil(pos, Vetor(pos, ponto_mouse).normalizado()*12, 20, 20, circular, 5, al_map_rgb(0, 0, 155)));
				break;
			case sniper:
				listaProjetil.insereNoInicio(Projetil(pos, Vetor(pos, ponto_mouse).normalizado()*12, 20, 20, circular, 5, al_map_rgb(0, 0, 155)));
				break;
			case laser:
				listaProjetil.insereNoInicio(Projetil(pos, Vetor(pos, ponto_mouse).normalizado()*12, 20, 20, circular, 5, al_map_rgb(0, 0, 155)));
				break;
		}
		tempoRecarga.start();
	}
}

Ponto Player::getPos(){
	return pos;
}

void Player::setLargura(int largura){
	this->largura = largura;
}
int Player::getLargura(){
	return largura;
}

void Player::setAltura(int altura){
	this->altura = altura;
}
int Player::getAltura(){
	return altura;
}

void Player::recebeDano(int dano){
	if(!tempoInvulnerabilidade.estaAtivo()){
        vida-=dano;
        tempoInvulnerabilidade.start();
	}
}

void Player::setVida(int vida){
	this->vida = vida;
}

int Player::getVida(){
	return vida;
}

void Player::setVidaMax(int vidaMax){
	this->vidaMax = vidaMax;
}

int Player::getVidaMax(){
	return vidaMax;
}

void Player::atualizaTimers(){
	tempoInvulnerabilidade.update();
	timerAnimacaoAndar.update();
	tempoRecarga.update();
}

int Player::getArma(){
    return arma;
}

void Player::setArma(int arma){
    this->arma = arma;
    switch(arma){
    	case pistola:
    		tempoRecarga.setMaximo(15);
    		break;
    	case shotgun:
    		tempoRecarga.setMaximo(15);
    		break;
    	case sub:
    		tempoRecarga.setMaximo(15);
    		break;
    	case rpg:
    		tempoRecarga.setMaximo(15);
    		break;
    	case sniper:
    		tempoRecarga.setMaximo(15);
    		break;
    	case laser:
    		tempoRecarga.setMaximo(15);
    		break;
	}
	tempoRecarga.start();
}

int Player::getDinheiro(){
    return dinheiro;
}

void Player::setDinheiro(int dinheiro){
    this->dinheiro = dinheiro;
    if(dinheiro < 0)
    	this->dinheiro = 0;
}

void Player::atualizaAngulo(Ponto mouse){
	angulo = Vetor(1, 0).anguloCom_(Vetor(pos, mouse));
	for(int i = 0; i < 4; i++){
		vertice[i] = verticeInicial[i].rotacionado(pos, angulo);
		v[i].x = vertice[i].getX();
		v[i].y = vertice[i].getY();
	}
}

Ponto Player::getVertice(int i){
	return vertice[i];
}

ALLEGRO_BITMAP* Player::spritePistola;
ALLEGRO_BITMAP* Player::spriteSub;
ALLEGRO_BITMAP* Player::spriteShotgun;
ALLEGRO_BITMAP* Player::spriteRPG;

void Player::initImagens(){
	spritePistola = al_load_bitmap("imagens/player_pistola.png");
	spriteSub = al_load_bitmap("imagens/player_sub.png");
	spriteShotgun = al_load_bitmap("imagens/player_shotgun.png");
	spriteRPG = al_load_bitmap("imagens/player_rpg.png");
}

int Player::getVelocidade(){
	return velocidade;
}

void Player::setVelocidade(int v){
	this->velocidade = v;
}
