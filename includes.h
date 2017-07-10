#ifndef INCLUDES_H
#define INCLUDES_H

#define N_TECLAS 52
#define N_MOUSE_BOTOES 3

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <string>
#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdio>

#include "geometria.h"

using namespace std;
using namespace geometria;

enum teclas{A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	K0, K1, K2, K3, K4, K5, K6, K7, K8, K9, ESC, TAB, CAPSLOCK, LSHIFT, RSHIFT, ALT, ALTGR, LCTRL, RCTRL, SPACE, ENTER, BACKSPACE,
	UP, DOWN, LEFT, RIGHT};

enum mouseBotoes{M1, M2, M3};

enum estados{menu, instrucoes, jogando, loja, gameOver};

enum inimigos{aranha, bp, palhaco, mariposa, foguinho, boss1, boss2};

enum armas{pistola, shotgun, sub, rpg, sniper, laser};

enum tipoProjeteis{circular, retangular};

bool verificaColisao(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2);

int aleatorio(int min, int max);

double distancia(Ponto a, Ponto b);



#endif
