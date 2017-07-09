#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <cmath>
#include <iostream>

namespace geometria{
	
	class Ponto;
	class Vetor;
	
	class Ponto{
		private:
			double x, y;
		
		public:
			Ponto();
			Ponto(double x, double y);
			void setX(double x);
			double getX();
			void setY(double y);
			double getY();
			
			Ponto operator+(Vetor);
			Ponto operator-(Vetor);
			
			Ponto rotacionado(Ponto p, double angulo);
	};
	
	class Vetor{
		private:
			double x, y, norma;
		
		public:
			Vetor();
			Vetor(double x, double y);
			Vetor(Ponto a, Ponto b);
			
			void setX(double x);
			double getX();
			void setY(double y);
			double getY();
			double getNorma();
			void calculaNorma();
			
			double operator*(Vetor);
			Vetor operator^(Vetor);
			
			Vetor operator*(double k);
			
			Vetor operator+=(Vetor);
			Vetor operator-=(Vetor);
			Vetor operator*=(Vetor);
			Vetor operator/=(Vetor);
			
			double anguloCom(Vetor v);
			double anguloCom_(Vetor v);
			
			Vetor rotacionado(double angulo);
			Vetor normalizado();
			
			Vetor normal();
	};
	
}

#endif
