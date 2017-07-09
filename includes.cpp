#include "includes.h"

bool verificaColisao(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2){
	if( (x1<x2 && x2<x1+w1 || x2<x1 && x1<x2+w2) && (y1<y2 && y2<y1+h1 || y2<y1 && y1<y2+h2) ){
		return true;
	}
	else 
		return false;
}

int aleatorio(int min, int max){
	return rand()%(max-min+1) + min;
}

double distancia(Ponto a, Ponto b){
	return Vetor(a, b).getNorma();
}


