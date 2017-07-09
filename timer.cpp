#include "timer.h"

Timer::Timer(){
	maximo = 0;
	contador = 0;
	ativo = false;
}

Timer::Timer(int max){
	maximo = max;
	contador = 0;
	ativo = false;
}

void Timer::setMaximo(int max){
	maximo = max;
}

int Timer::getMaximo(){
	return maximo;
}

void Timer::update(){
	if(ativo){
		contador--;
	}
	if(contador == 0){
		ativo = false;
	}
}

void Timer::start(){
	ativo = true;
	contador = maximo;
}

void Timer::pause(){
	ativo = false;
}

void Timer::stop(){
	ativo = false;
	contador = 0;
}

void Timer::reset(){
	contador = maximo;
}

int Timer::getContador(){
	return contador;
}

bool Timer::estaAtivo(){
	return ativo;
}
