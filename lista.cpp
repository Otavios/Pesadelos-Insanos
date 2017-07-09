#include "lista.h"

template <class T>		
Lista<T>::Lista(){
	header = new Node<T>;
	header->next = header;
	tam = 0;
}

template <class T>		
bool Lista<T>::vazia(){
	if(header->next == header)
		return true;
	return false;
}

template <class T>
void Lista<T>::insere(T elem, int pos){
	if(pos <= tam){
		int i = 0;
		Node<T> *atual, *aux;
		atual = header;
		while(i < pos){
			atual = atual->next;
			i++;
		}
		aux = new Node<T>;
		aux->next = atual->next;
		atual->next = aux;
		aux->info = elem;
		tam++;
	}
}

template <class T>
void Lista<T>::insereNoInicio(T elem){
	insere(elem, 0);
}

template <class T>
void Lista<T>::insereNoFim(T elem){
	insere(elem, tam);
}

template <class T>
void Lista<T>::remove(int pos){
	if(pos < tam){
		int i = 0;
		Node<T> *atual, *anterior;
		anterior = header;
		atual = header->next;
		while(i < pos){
			anterior = atual;
			atual = atual->next;
			i++;
		}
		anterior->next = atual->next;
		delete atual;
		tam--;
	}
}

template <class T>
void Lista<T>::removePrimeiro(){
	remove(0);
}

template <class T>
void Lista<T>::removeUltimo(){
	remove(tam-1);
}

template <class T>
T& Lista<T>::operator[](int i){
	if(!vazia()){
		int j = 0;
		Node<T> *atual;
		atual = header;
		while(j <= i){
			atual = atual->next;
			j++;
		}
		return atual->info;
	}
}

template <class T>
bool Lista<T>::removeElemento(T elem){
	if(vazia())
		return false;
	int i = 0;
	Node<T> *atual, *anterior;
	anterior = header;
	atual = header->next;
	while(atual != header){
		if(atual->info == elem){
			anterior->next = atual->next;
			delete atual;
			tam--;
			return true;
		}
		anterior = atual;
		atual = atual->next;
	}
	return false;
}

template <class T>
bool Lista<T>::procura(T elem){
	if(vazia())
		return false;
	int i = 0;
	Node<T> *atual, *anterior;
	anterior = header;
	atual = header->next;
	while(atual != header){
		if(atual->info == elem){
			return true;
		}
		anterior = atual;
		atual = atual->next;
	}
	return false;
}

template <class T>
int Lista<T>::getTam(){
	return tam;
}

template class Lista<Projetil>;
template class Lista<Inimigo>;
