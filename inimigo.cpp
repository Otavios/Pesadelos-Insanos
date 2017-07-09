#include "inimigo.h"

using namespace geometria;

Inimigo::Inimigo(){
	pos = Ponto(0, 0);
	velocidade = Vetor(0, 0);
	moduloVelocidade = 0;
	largura = 0;
	altura = 0;
	vida = 0;
	tipo = 0;
}

Inimigo::Inimigo(Ponto p, Vetor vel, double m, int vida, int t, int cooldownMax){
	pos = p;
	moduloVelocidade = m;
	velocidade = vel;
	this->vida = vida;
	tipo = t;
	cooldown = Timer(cooldownMax);
	cooldown.start();
	tempoMovimento = Timer(50);
	tempoMovimento.start();
	if(aleatorio(0, 1) == 0){
		sentidoRotacao = -1;
	}	
	else{
		sentidoRotacao = 1;
	}
	angulo = 0;
	transparencia = 0;
	
	if(tipo == aranha){
		largura = 33;
		altura = 28;
		barraVida = Barra(pos+Vetor(0, altura/2+70), 35, 5, vida, vida, al_map_rgba(255, 0, 0, 0.8), al_map_rgb(0, 0, 0), 2, 2);
		timerAnimacaoAndar = Timer(0.05*5*60-1); // (segundos de cada frame * numero de frames * 60) - 1
		timerAnimacaoAndar.start();
	}
	
	if(tipo == bp){
		largura = 32;
		altura = 52;
		barraVida = Barra(pos, 35, 5, vida, vida, al_map_rgba(255, 0, 0, 0.8), al_map_rgb(0, 0, 0), 2, 2);
		timerAnimacaoAndar = Timer(0.05*8*60-1);
		timerAnimacaoAndar.start();
	}
	
	if(tipo == palhaco){
		largura = 18;
		altura = 32;
		barraVida = Barra(pos, 35, 5, vida, vida, al_map_rgba(255, 0, 0, 0.8), al_map_rgb(0, 0, 0), 2, 2);
		timerAnimacaoAndar = Timer(0.05*8*60-1);
		timerAnimacaoAndar.start();
	}
	
	if(tipo == foguinho){
		largura = 13;
		altura = 13;
		barraVida = Barra(pos, 35, 5, vida, vida, al_map_rgba(255, 0, 0, 0.8), al_map_rgb(0, 0, 0), 2, 2);
		velocidade = Vetor(aleatorio(-100, 100), aleatorio(-100, 100)).normalizado()*moduloVelocidade;
		timerAnimacaoAndar = Timer(0.05*8*60-1);
		timerAnimacaoAndar.start();
	}
	
	if(tipo == boss1){
		largura = 30;
		altura = 30;
		barraVida = Barra(Ponto(385, 20), 480, 30, vida, vida, al_map_rgba(255, 0, 0, 0.8), al_map_rgb(0, 0, 0), 10, 5);
		velocidade = Vetor(aleatorio(-100, 100), aleatorio(-100, 100)).normalizado()*moduloVelocidade;
		timerAnimacaoAndar = Timer(0.05*8*60-1);
		timerAnimacaoAndar.start();
	}
	
	if(tipo == boss2){
		largura = 90;
		altura = 500;
		barraVida = Barra(Ponto(385, 20), 480, 30, vida, vida, al_map_rgba(255, 0, 0, 0.8), al_map_rgb(0, 0, 0), 10, 5);
		velocidade = Vetor(aleatorio(-100, 100), aleatorio(-100, 100)).normalizado()*moduloVelocidade;
		timerAnimacaoAndar = Timer(0.05*8*60-1);
		timerAnimacaoAndar.start();
	}
	
	verticeInicial[0] = Ponto(pos.getX()-largura/2, pos.getY()-altura/2);
	verticeInicial[1] = Ponto(pos.getX()+largura/2, pos.getY()-altura/2);
	verticeInicial[2] = Ponto(pos.getX()+largura/2, pos.getY()+altura/2);
	verticeInicial[3] = Ponto(pos.getX()-largura/2, pos.getY()+altura/2);
	for(int i = 0; i < 4; i++){
		v[i].x = verticeInicial[i].getX();
		v[i].y = verticeInicial[i].getY();
		v[i].z = 0;
		v[i].color = al_map_rgb(255, 0, 0);
		vertice[i] = verticeInicial[i];
	}
	
	timerAtaquesBoss1 = Timer(25*60);
	timerAtaquesBoss1.start();
	
	timerAtaquesBoss2 = Timer(25*60);
	timerAtaquesBoss2.start();
}

void Inimigo::desenhar(bool mostrarHitbox){
	
	if(tipo == palhaco){
		al_draw_tinted_scaled_rotated_bitmap_region(spritePalhaco, timerAnimacaoAndar.getContador()/3*72, 0, 72, 72,
			al_map_rgba_f(transparencia, transparencia, transparencia, transparencia), 25, 36, pos.getX(), pos.getY(), 0.5, 0.5, -angulo, 0);
			
		if(!timerAnimacaoAndar.estaAtivo())
		    timerAnimacaoAndar.start();
	}
	
	else if (tipo == bp){
		al_draw_tinted_scaled_rotated_bitmap_region(spriteBp, timerAnimacaoAndar.getContador()/3*120, 0, 120, 150,
			al_map_rgba_f(transparencia, transparencia, transparencia, transparencia), 56, 75, pos.getX(), pos.getY(), 0.40, 0.40, -angulo, 0);
		
		if(!timerAnimacaoAndar.estaAtivo())
		    timerAnimacaoAndar.start();
	}
	else if (tipo == aranha){
		al_draw_tinted_scaled_rotated_bitmap_region(spriteAranha, timerAnimacaoAndar.getContador()/3*25, 0, 25, 25,
			al_map_rgba_f(transparencia, transparencia, transparencia, transparencia), 12.5, 12.5, pos.getX(), pos.getY(), 1.8, 1.8, -angulo, 0);
		
		if(!timerAnimacaoAndar.estaAtivo())
		    timerAnimacaoAndar.start();
	}
	
	if(mostrarHitbox)
		al_draw_prim(v, NULL, 0, 0, 4, ALLEGRO_PRIM_LINE_LOOP);
}

void Inimigo::andar(Ponto posicao_player, int larguraTela, int alturaTela){
	if(tipo == bp){
		if(distancia(pos, posicao_player) > 5){
			velocidade = Vetor(pos, posicao_player).normalizado() * moduloVelocidade;
			pos = pos + velocidade;
			for(int i = 0; i < 4; i++)
				verticeInicial[i] = verticeInicial[i] + velocidade;
		}
	}
	else if(tipo == aranha){
		velocidade = velocidade.normalizado() * moduloVelocidade;
		pos = pos + velocidade;
		for(int i = 0; i < 4; i++){
			verticeInicial[i] = verticeInicial[i] + velocidade;
			v[i].x = verticeInicial[i].getX();
			v[i].y = verticeInicial[i].getY();
		}
		if(pos.getX() < 0){
			pos.setX(0);
			if(velocidade.getX() < 0 && velocidade.getY() > 0)
				velocidade = velocidade.rotacionado(M_PI/2);
			else
				velocidade = velocidade.rotacionado(-M_PI/2);
		}
		if(pos.getX() > larguraTela){
			pos.setX(larguraTela);
			if(velocidade.getX() > 0 && velocidade.getY() < 0)
				velocidade = velocidade.rotacionado(M_PI/2);
			else
				velocidade = velocidade.rotacionado(-M_PI/2);
		}
		if(pos.getY() < 0){
			pos.setY(0);
			if(velocidade.getX() > 0 && velocidade.getY() < 0)
				velocidade = velocidade.rotacionado(-M_PI/2);
			else
				velocidade = velocidade.rotacionado(M_PI/2);
		}
		if(pos.getY() > alturaTela){
			pos.setY(alturaTela);
			if(velocidade.getX() > 0 && velocidade.getY() > 0)
				velocidade = velocidade.rotacionado(M_PI/2);
			else
				velocidade = velocidade.rotacionado(-M_PI/2);
		}
	}
	else if(tipo == palhaco){
		if(distancia(pos, posicao_player) > 120){
			velocidade = Vetor(pos, posicao_player).normalizado() * moduloVelocidade;
			pos = pos + velocidade;
			for(int i = 0; i < 4; i++){
				verticeInicial[i] = verticeInicial[i] + velocidade;
				v[i].x = verticeInicial[i].getX();
				v[i].y = verticeInicial[i].getY();
			}	
		}else{
			velocidade = Vetor(pos, posicao_player).normalizado() * moduloVelocidade;
			double x = pos.getX();
			double y = pos.getY();
			pos.setY((y-posicao_player.getY())*cos(2*M_PI/180.0) + sentidoRotacao*(x-posicao_player.getX())*sin(2*M_PI/180.0) + posicao_player.getY());
			pos.setX((x-posicao_player.getX())*cos(2*M_PI/180.0) - sentidoRotacao*(y-posicao_player.getY())*sin(2*M_PI/180.0) + posicao_player.getX());
			x = x-pos.getX();
			y = y-pos.getY();
			for(int i = 0; i < 4; i++){
				verticeInicial[i].setX(verticeInicial[i].getX()-x);
				verticeInicial[i].setY(verticeInicial[i].getY()-y);
			}
			if(pos.getX() < 0 || pos.getX() > larguraTela || pos.getY() < 0 || pos.getY() > alturaTela){
				sentidoRotacao *= -1;
			}
		}
	}
	else if(tipo == foguinho){
		if(tempoMovimento.getContador() == 1){
			velocidade = Vetor(aleatorio(-100, 100), aleatorio(-100, 100)).normalizado()*moduloVelocidade;
		}
			
		pos = pos + velocidade;
		for(int i = 0; i < 4; i++){
			verticeInicial[i] = verticeInicial[i] + velocidade;
			v[i].x = verticeInicial[i].getX();
			v[i].y = verticeInicial[i].getY();
		}
		if(pos.getX() < 0){
			pos.setX(0);
			if(velocidade.getX() < 0 && velocidade.getY() > 0)
				velocidade = velocidade.rotacionado(M_PI/2);
			else
				velocidade = velocidade.rotacionado(-M_PI/2);
		}
		if(pos.getX() > larguraTela){
			pos.setX(larguraTela);
			if(velocidade.getX() > 0 && velocidade.getY() < 0)
				velocidade = velocidade.rotacionado(M_PI/2);
			else
				velocidade = velocidade.rotacionado(-M_PI/2);
		}
		if(pos.getY() < 0){
			pos.setY(0);
			if(velocidade.getX() > 0 && velocidade.getY() < 0)
				velocidade = velocidade.rotacionado(-M_PI/2);
			else
				velocidade = velocidade.rotacionado(M_PI/2);
		}
		if(pos.getY() > alturaTela){
			pos.setY(alturaTela);
			if(velocidade.getX() > 0 && velocidade.getY() > 0)
				velocidade = velocidade.rotacionado(M_PI/2);
			else
				velocidade = velocidade.rotacionado(-M_PI/2);
		}
	}
	else if(tipo == boss1){
		if(distancia(pos, Ponto(385, 270)) > 10){
			velocidade = Vetor(pos, Ponto(385, 270)).normalizado()*moduloVelocidade;
			pos = pos + velocidade;
			for(int i = 0; i < 4; i++){
				verticeInicial[i] = verticeInicial[i] + velocidade;
				v[i].x = verticeInicial[i].getX();
				v[i].y = verticeInicial[i].getY();
			}
		}else{
			velocidade = Vetor(pos, posicao_player);
		}
	}
	else if(tipo == boss2){
		if(distancia(pos, Ponto(385, 270)) > 250){
			velocidade = Vetor(pos, Ponto(385, 270)).normalizado()*moduloVelocidade;
			pos = pos + velocidade;
			for(int i = 0; i < 4; i++){
				verticeInicial[i] = verticeInicial[i] + velocidade;
				v[i].x = verticeInicial[i].getX();
				v[i].y = verticeInicial[i].getY();
			}
			velocidade = Vetor(-1, 0);
		}
	}
}

Ponto Inimigo::getPos(){
	return pos;
}

bool Inimigo::operator==(Inimigo i){
	return true;
}

int Inimigo::getVida(){
	return vida;
}
void Inimigo::setVida(int vida){
	this->vida = vida;
}

void Inimigo::recebeDano(int dano){
	vida -= dano;
	barraVida.diminui(dano);
}

void Inimigo::setLargura(int largura){
	this->largura = largura;
}
int Inimigo::getLargura(){
	return largura;
}

void Inimigo::setAltura(int altura){
	this->altura = altura;
}
int Inimigo::getAltura(){
	return altura;
}

void Inimigo::atirar(Lista<Projetil>& listaProjeteisInimigos, Ponto ponto_player, Lista<Inimigo>& listaInimigos){
	if(tipo == aranha){
		
	}
	else if(tipo == bp){
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(pos, ponto_player).normalizado()*8, 10, 10, circular, 1, al_map_rgb(255, 0, 0)));
	}
	else if(tipo == palhaco){
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(pos, ponto_player).normalizado()*8, 6, 6, circular, 5, al_map_rgb(255, 0, 0)));
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(pos, ponto_player).rotacionado(8*M_PI/180.0).normalizado()*8, 6, 6, circular, 5, al_map_rgb(255, 0, 0)));
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(pos, ponto_player).rotacionado(-8*M_PI/180.0).normalizado()*8, 6, 6, circular, 5, al_map_rgb(255, 0, 0)));
	}
	else if(tipo == foguinho){
		double a = 2*M_PI/7;
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, velocidade.normalizado()*8, 5, 5, circular, 1, al_map_rgb(255, 0, 0)));
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, velocidade.rotacionado(a).normalizado()*8, 5, 5, circular, 1, al_map_rgb(255, 0, 0)));
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, velocidade.rotacionado(2*a).normalizado()*8, 5, 5, circular, 1, al_map_rgb(255, 0, 0)));
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, velocidade.rotacionado(3*a).normalizado()*8, 5, 5, circular, 1, al_map_rgb(255, 0, 0)));
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, velocidade.rotacionado(4*a).normalizado()*8, 5, 5, circular, 1, al_map_rgb(255, 0, 0)));
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, velocidade.rotacionado(5*a).normalizado()*8, 5, 5, circular, 1, al_map_rgb(255, 0, 0)));
		listaProjeteisInimigos.insereNoInicio(Projetil(pos, velocidade.rotacionado(6*a).normalizado()*8, 5, 5, circular, 1, al_map_rgb(255, 0, 0)));
	}
	else if(tipo == boss1){
		if(timerAtaquesBoss1.getContador() > 20*60){
			if(timerAtaquesBoss1.getContador()%15 == 0){
				listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(pos, ponto_player).normalizado()*7, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
			}
		}
		else if(timerAtaquesBoss1.getContador() > 15*60){
			if(timerAtaquesBoss1.getContador()%60 == 0){
				listaInimigos.insereNoInicio(Inimigo(Ponto(pos.getX(), pos.getY()), Vetor(1, 1), 3, 10, foguinho, 180));
				listaInimigos.insereNoInicio(Inimigo(Ponto(pos.getX(), pos.getY()), Vetor(1, 1), 3, 10, foguinho, 180));
				listaInimigos.insereNoInicio(Inimigo(Ponto(pos.getX(), pos.getY()), Vetor(1, 1), 3, 10, foguinho, 180));
			}
		}
		else if(timerAtaquesBoss1.getContador() > 10*60){
			if(timerAtaquesBoss1.getContador()%15 == 0){
				listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(pos, ponto_player).normalizado()*8, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
			}
		}
		else if(timerAtaquesBoss1.getContador() > 5*60){
			if(timerAtaquesBoss1.getContador()%3 == 0){
				listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(pos, ponto_player).rotacionado(0.6).normalizado()*8, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(pos, ponto_player).rotacionado(-0.6).normalizado()*8, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
			}
			if(timerAtaquesBoss1.getContador()%20 == 0){
				listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(pos, ponto_player).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
			}
		}
		else if(timerAtaquesBoss1.getContador() > 0){
			if(timerAtaquesBoss1.getContador()%15 == 0){
				for(int i = 0; i < 20; i++){
					listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(aleatorio(-100, 100), aleatorio(-100, 100)).normalizado()*5, 10, 10, circular, 1, al_map_rgb(255, 0, 0)));
				}
			}
		}
		
	}
	
	else if(tipo == boss2){
		Ponto cima(pos.getX(), pos.getY()-altura/3);
		Ponto centro(pos.getX(), pos.getY());
		Ponto baixo(pos.getX(), pos.getY()+altura/3);
		if(timerAtaquesBoss1.getContador() > 20*60){
			if(timerAtaquesBoss1.getContador()%20 == 0){
				listaProjeteisInimigos.insereNoInicio(Projetil(cima, Vetor(pos, ponto_player).rotacionado(0.2).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(cima, Vetor(pos, ponto_player).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(cima, Vetor(pos, ponto_player).rotacionado(-0.2).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(baixo, Vetor(pos, ponto_player).rotacionado(0.4).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(baixo, Vetor(pos, ponto_player).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(baixo, Vetor(pos, ponto_player).rotacionado(-0.4).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
			}
			if(timerAtaquesBoss1.getContador()%35 == 0){
				for(int i = 0; i < 25; i++){
					listaProjeteisInimigos.insereNoInicio(Projetil(pos, Vetor(aleatorio(-100, 100), aleatorio(-100, 100)).normalizado()*5, 10, 10, circular, 1, al_map_rgb(255, 0, 0)));
				}
			}
				
		}
		else if(timerAtaquesBoss1.getContador() > 15*60){
			if(timerAtaquesBoss1.getContador()%15 == 0){
				listaProjeteisInimigos.insereNoInicio(Projetil(cima, Vetor(-1, 0).rotacionado(0.1).normalizado()*8, 18, 18, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(cima, Vetor(-1, 0).normalizado()*8, 18, 18, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(cima, Vetor(-1, 0).rotacionado(-0.1).normalizado()*8, 18, 18, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(baixo, Vetor(-1, 0).rotacionado(0.1).normalizado()*8, 18, 18, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(baixo, Vetor(-1, 0).normalizado()*8, 18, 18, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(baixo, Vetor(-1, 0).rotacionado(-0.1).normalizado()*8, 18, 18, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(centro, Vetor(pos, ponto_player).normalizado()*10, 18, 18, circular, 100, al_map_rgb(255, 0, 0)));
			}
		}
		else if(timerAtaquesBoss1.getContador() > 10*60){
			if(timerAtaquesBoss1.getContador()%4 == 0){
				listaProjeteisInimigos.insereNoInicio(Projetil(centro, Vetor(-1, 0).normalizado()*12, 20, 20, circular, 100, al_map_rgb(255, 0, 155)));
			}
			if(timerAtaquesBoss1.getContador()%15 == 0){
				listaProjeteisInimigos.insereNoInicio(Projetil(cima, Vetor(pos, ponto_player).rotacionado(0.2).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(cima, Vetor(pos, ponto_player).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(cima, Vetor(pos, ponto_player).rotacionado(-0.2).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(baixo, Vetor(pos, ponto_player).rotacionado(0.4).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(baixo, Vetor(pos, ponto_player).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
				listaProjeteisInimigos.insereNoInicio(Projetil(baixo, Vetor(pos, ponto_player).rotacionado(-0.4).normalizado()*6, 10, 10, circular, 100, al_map_rgb(255, 0, 0)));
			}
		}
		else if(timerAtaquesBoss1.getContador() > 5*60){
			if(timerAtaquesBoss1.getContador()%60 == 0){
				listaInimigos.insereNoInicio(Inimigo(Ponto(pos.getX(), pos.getY()), Vetor(1, 1), 3, 10, bp, 180));
			}
			if(timerAtaquesBoss1.getContador()%25 == 0){
				listaProjeteisInimigos.insereNoInicio(Projetil(centro, Vetor(-1, 0).normalizado()*10, 20, 20, circular, 100, al_map_rgb(255, 0, 155)));
			}
		}
		else if(timerAtaquesBoss1.getContador() > 0){
			if(timerAtaquesBoss1.getContador()%65 == 0){
				listaInimigos.insereNoInicio(Inimigo(Ponto(pos.getX(), pos.getY()), Vetor(1, 1), 3, 10, palhaco, 180));
			}
		}
		
	}
}

void Inimigo::atualiza(Lista<Projetil>& listaProjeteisInimigos, Ponto ponto_player, Lista<Inimigo>& listaInimigos){
	
	cooldown.update();
	if(!cooldown.estaAtivo()){
		atirar(listaProjeteisInimigos, ponto_player, listaInimigos);
		cooldown.start();
	}
	
	tempoMovimento.update();
	if(!tempoMovimento.estaAtivo()){
		tempoMovimento.start();
	}
	
	timerAnimacaoAndar.update();
	transparencia += 1/50.0;
	if(transparencia > 1)
		transparencia = 1;
		
	switch(tipo){
		case aranha:
			barraVida.atualizar(Ponto(pos.getX(), pos.getY()-23));
			break;
		case bp:
			barraVida.atualizar(Ponto(pos.getX(), pos.getY()-30));
			break;
		case palhaco:
			barraVida.atualizar(Ponto(pos.getX(), pos.getY()-20));
			break;
		case foguinho:
			barraVida.atualizar(Ponto(pos.getX(), pos.getY()-altura/2));
			break;
		case boss1:
			barraVida.atualizar(Ponto(385, 22));
			break;
		case boss2:
			barraVida.atualizar(Ponto(385, 22));
			break;
	}
	
	timerAtaquesBoss1.update();
	if(!timerAtaquesBoss1.estaAtivo()){
		timerAtaquesBoss1.start();
	}
	
	timerAtaquesBoss2.update();
	if(!timerAtaquesBoss2.estaAtivo()){
		timerAtaquesBoss2.start();
	}
}

void Inimigo::atualizaAngulo(){
	angulo = Vetor(1, 0).anguloCom_(velocidade);
	for(int i = 0; i < 4; i++){
		vertice[i] = verticeInicial[i].rotacionado(pos, angulo);
		v[i].x = vertice[i].getX();
		v[i].y = vertice[i].getY();
	}
}

Ponto Inimigo::getVertice(int i){
	return vertice[i];
}

ALLEGRO_BITMAP* Inimigo::spritePalhaco;
ALLEGRO_BITMAP* Inimigo::spriteBp;
ALLEGRO_BITMAP* Inimigo::spriteAranha;


void Inimigo::initImagens(){
	spritePalhaco = al_load_bitmap("imagens/palhaco.png");
	spriteBp = al_load_bitmap("imagens/bichopapao.png");
	spriteAranha = al_load_bitmap("imagens/aranha.png");
}

void Inimigo::desenhaBarraVida(){
	barraVida.desenhar();
} 
