#ifndef LISTA_H
#define LISTA_H

#include "includes.h"
#include "projetil.h"
#include "inimigo.h"

template <class T>
struct Node{
	T info;
	Node* next;
};

template <class T>
class Lista{
	private:
		Node<T> *header;
		int tam;	
	public:
		Lista();
		bool vazia();
		void insere(T elem, int pos);	
		void insereNoInicio(T elem);
		void insereNoFim(T elem);
		void remove(int pos);		
		void removePrimeiro();		
		void removeUltimo();		
		T& operator[](int i);		
		bool removeElemento(T elem);
		bool procura(T elem);
		int getTam();
		
};

#endif
