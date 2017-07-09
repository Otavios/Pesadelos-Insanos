#include "geometria.h"

using namespace geometria;

Vetor::Vetor(){
	x = 0;
	y = 0;
	norma = 0;
}

Vetor::Vetor(double x, double y){
	this->x = x;
	this->y = y;
}

Vetor::Vetor(Ponto a, Ponto b){
	x = b.getX() - a.getX();
	y = b.getY() - a.getY();
}

void Vetor::setX(double x){
	this->x = x;
	calculaNorma();
}
double Vetor::getX(){
	return x;
}

void Vetor::setY(double y){
	this->y = y;
	calculaNorma();
}
double Vetor::getY(){
	return y;
}

void Vetor::calculaNorma(){
	norma = sqrt(x*x + y*y);
}

double Vetor::getNorma(){
	calculaNorma();
	return norma;
}

double Vetor::operator*(Vetor v){
	return x*v.getX() + y*v.getY();
}

Vetor Vetor::operator^(Vetor v){
	
}

Vetor Vetor::operator*(double k){
	return Vetor(x*k, y*k);
}

double Vetor::anguloCom(Vetor v){
	return acos((*this * v)/(getNorma()*v.getNorma()));
}

Vetor Vetor::normalizado(){
	double n = getNorma();
	if(n == 0)
		return Vetor(1, 0);
	else
		return Vetor(x/n, y/n);
}

Vetor Vetor::rotacionado(double a){
	return Vetor(x*cos(a)+y*sin(a), -x*sin(a)+y*cos(a));
}

double Vetor::anguloCom_(Vetor v){
	if(getNorma() == 0 || v.getNorma() == 0)
		return 0;
	else if(v.getY() < 0)
		return acos((*this * v)/(getNorma()*v.getNorma()));
	else
		return (-acos((*this * v)/(getNorma()*v.getNorma())) + 2*M_PI);
}

Vetor Vetor::normal(){
	return Vetor(-y, x);
}
