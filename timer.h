#ifndef TIMER_H
#define TIMER_H

#include "includes.h"

class Timer{
	
	private:
		int contador, maximo;
		bool ativo;
		
	public:
		Timer();
		Timer(int max);
		void setMaximo(int max);
		int getMaximo();
		void start();
		void pause();
		void stop();
		void reset();
		int getContador();
		bool estaAtivo();
		void update();
		
};

#endif
