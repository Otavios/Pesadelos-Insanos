#include "geometria.h"

using namespace geometria;

Ponto::Ponto(){
	x = 0;
	y = 0;
}

Ponto::Ponto(double x, double y){
	this->x = x;
	this->y = y;
}

void Ponto::setX(double x){
	this->x = x;
}
double Ponto::getX(){
	return x;
}

void Ponto::setY(double y){
	this->y = y;
}
double Ponto::getY(){
	return y;
}

Ponto Ponto::operator+(Vetor v){
	return Ponto(x+v.getX(), y+v.getY());
}

Ponto Ponto::operator-(Vetor v){
	return Ponto(x-v.getX(), y-v.getY());
}

Ponto Ponto::rotacionado(Ponto p, double angulo){
	double a = p.x;
	double b = p.y;
	return Ponto((x-a)*cos(angulo)+(y-b)*sin(angulo)+a, (y-b)*cos(angulo)-(x-a)*sin(angulo)+b);
}
