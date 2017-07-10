#include "includes.h"

#include "geometria.h"
#include "player.h"
#include "projetil.h"
#include "inimigo.h"
#include "botao.h"
#include "lista.h"

#define preco_pistola 0
#define preco_shotgun 55
#define preco_sub 120
#define preco_rpg 200
#define preco_sniper 200
#define preco_laser 400
#define preco_vida 120
#define preco_velocidade 70
#define preco_kit 70

bool colisao(Player p, Projetil b);
bool colisao(Player p, Inimigo b);
bool colisao(Inimigo p, Projetil& b);

void spawnarInimigo(Lista<Inimigo>& listaInimigos, int tipo, int larguraTela, int alturaTela, int vida, double velocidade, int cooldown);

void atualizaWave(int& wave, Timer& timerWave, Lista<Inimigo>& listaInimigos, int larguraTela, int alturaTela);

int main(){
	int largura = 960;
	int altura = 540;
	int fps = 60;
	bool acabou = false;
	Ponto mouse(0, 0);
	double sx = 1.0, sy = 1.0;
	int estado = menu;
	bool mostrarHitboxInimigos = false;
	bool mostrarHitboxPlayer = false;
	bool mostrarHitboxProjeteis = false;
	int wave = 1;
	Timer timerWave(35*60);
	timerWave.start();
	Timer timerExplosao(3);
	int xExplosao, yExplosao;
	bool explosao = false;
	bool cheat = false;

	Lista<Projetil> listaProjeteisPlayer;
	Lista<Projetil> listaProjeteisInimigos;
	Lista<Inimigo> listaInimigos;

	Timer mensagemDinheiro(2*60);
	bool comprouArma[6] = {true, false, false, false, false, false};

	ALLEGRO_BITMAP *coracaoCheio;
	ALLEGRO_BITMAP *coracaoMetade;
	ALLEGRO_BITMAP *coracaoVazio;

	ALLEGRO_BITMAP *img_pistola;
	ALLEGRO_BITMAP *img_sub;
	ALLEGRO_BITMAP *img_shotgun;
	ALLEGRO_BITMAP *img_rpg;
	ALLEGRO_BITMAP *img_sniper;
	ALLEGRO_BITMAP *img_laser;

	ALLEGRO_BITMAP *fundoMenu;
	ALLEGRO_BITMAP *frescura;
	ALLEGRO_BITMAP *fundoJogo;
	ALLEGRO_BITMAP *fundoLoja;
	ALLEGRO_BITMAP *fundoGameOver;

	ALLEGRO_BITMAP *img_comprar;
	ALLEGRO_BITMAP *img_usar;
	ALLEGRO_BITMAP *img_max;

	bool tecla[N_TECLAS];
	for(int i = 0; i < N_TECLAS; i++) tecla[i] = false;

	bool mouseBotao[N_MOUSE_BOTOES];
	for(int i = 0; i < N_MOUSE_BOTOES; i++) mouseBotao[i] = false;

	ALLEGRO_DISPLAY *tela = NULL;
	ALLEGRO_EVENT_QUEUE *filaEventos = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *arial24 = NULL;

	srand(time(NULL));

	al_init();
    al_install_keyboard();
    al_install_mouse();
	al_install_audio();

	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();

	al_reserve_samples(15);

	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	tela = al_create_display(largura, altura);
	timer = al_create_timer(1.0 / fps);
	filaEventos = al_create_event_queue();
	arial24 = al_load_font("fonts/arial.ttf", 24, 0);

	coracaoCheio = al_load_bitmap("imagens/coracao_cheio.png");
	coracaoMetade = al_load_bitmap("imagens/coracao_metade.png");
	coracaoVazio = al_load_bitmap("imagens/coracao_vazio.png");

	img_pistola = al_load_bitmap("imagens/pistola.png");
	img_sub = al_load_bitmap("imagens/sub.png");
	img_shotgun = al_load_bitmap("imagens/shotgun.png");
	img_rpg = al_load_bitmap("imagens/rpg.png");
	img_sniper = al_load_bitmap("imagens/sniper.png");
	img_laser = al_load_bitmap("imagens/laser.png");

	img_comprar = al_load_bitmap("imagens/btn_comprar.png");
	img_usar = al_load_bitmap("imagens/btn_usar.png");
	img_max = al_load_bitmap("imagens/btn_max.png");

	al_register_event_source(filaEventos, al_get_display_event_source(tela));
	al_register_event_source(filaEventos, al_get_keyboard_event_source());
	al_register_event_source(filaEventos, al_get_timer_event_source(timer));
	al_register_event_source(filaEventos, al_get_mouse_event_source());

	Player player(Ponto(300, 300), 2.5, 13, 30, 6, 0, 0);
	player.init();
	Player::initImagens();
	Inimigo::initImagens();

	Botao btnComprarPistola(215, 155, img_usar);
	Botao btnComprarShotgun(465, 155, img_comprar);
	Botao btnComprarSub(715, 155, img_comprar);
	Botao btnComprarRPG(215, 310, img_comprar);
	Botao btnComprarSniper(465, 310, img_comprar);
	Botao btnComprarLaser(715, 310, img_comprar);
	Botao btnComprarVida(215, 460, img_comprar);
	Botao btnComprarVelocidade(465, 460, img_comprar);
	Botao btnComprarMedicamento(715, 460, img_comprar);

	Botao botaoJogar(572, 177, "imagens/botaoJogar.png");
	Botao botaoSair(572, 238, "imagens/botaoSair.png");

	fundoMenu = al_load_bitmap("imagens/menu.png");
	frescura = al_load_bitmap("imagens/ola.png");
	fundoJogo = al_load_bitmap("imagens/sad.png");
	fundoLoja = al_load_bitmap("imagens/esboco_1.png");
	fundoGameOver = al_load_bitmap("imagens/gameOver.png");

	al_start_timer(timer);

	while(!acabou){
		if(estado == menu){
			ALLEGRO_EVENT evento;
			al_wait_for_event(filaEventos, &evento);

            if(evento.type == ALLEGRO_EVENT_TIMER){
				
                botaoJogar.atualiza(mouse.getX(), mouse.getY());
                botaoSair.atualiza(mouse.getX(), mouse.getY());
                al_draw_bitmap(fundoMenu, 0, 0, 0);

                botaoJogar.desenhar();
                botaoSair.desenhar();

                 if(botaoJogar.estaSelecionado())
                    al_draw_bitmap(frescura, 440, 158, 0);
                 else if (botaoSair.estaSelecionado())
                    al_draw_bitmap(frescura, 440, 223, 0);

                al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));

            }

            else if(evento.type == ALLEGRO_EVENT_MOUSE_AXES){
                    mouse.setX(evento.mouse.x / sx);
                    mouse.setY(evento.mouse.y / sy);
            }

            else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                if(evento.mouse.button == 1){
                    if(botaoJogar.estaSelecionado())
                    	estado = jogando;
                    else if(botaoSair.estaSelecionado())
                    	acabou = true;
                }
            }
		} // fim menu

		else if(estado == jogando){
			ALLEGRO_EVENT evento;
			al_wait_for_event(filaEventos, &evento);

			if(evento.type == ALLEGRO_EVENT_TIMER){
				
				atualizaWave(wave, timerWave, listaInimigos, largura-200, altura);
				timerExplosao.update();
				
				// atualiza player
				player.andar(tecla[W]||tecla[UP], tecla[S]||tecla[DOWN], tecla[A]||tecla[LEFT], tecla[D]||tecla[RIGHT], largura-200, altura, mouse);
				player.atualizaAngulo(mouse);
				player.atualizaTimers();

				if(mouseBotao[M1])
					player.atirar(listaProjeteisPlayer, mouse, false);

				// atualiza projeteis
				for(int i = 0; i < listaProjeteisPlayer.getTam(); i++){
					listaProjeteisPlayer[i].andar();
					listaProjeteisPlayer[i].atualizaAngulo();
					if(listaProjeteisPlayer[i].getPos().getX() < 0 || listaProjeteisPlayer[i].getPos().getX() > largura-190
					|| listaProjeteisPlayer[i].getPos().getY() < 0 || listaProjeteisPlayer[i].getPos().getY() > altura){
						listaProjeteisPlayer.remove(i);
						i--;
					}
				}
				for(int i = 0; i < listaProjeteisInimigos.getTam(); i++){
					listaProjeteisInimigos[i].andar();
					listaProjeteisInimigos[i].atualizaAngulo();
					if(listaProjeteisInimigos[i].getPos().getX() < 0 || listaProjeteisInimigos[i].getPos().getX() > largura-190
					|| listaProjeteisInimigos[i].getPos().getY() < 0 || listaProjeteisInimigos[i].getPos().getY() > altura){
						listaProjeteisInimigos.remove(i);																																							;
						i--;
					}
				}

				// atualiza inimigos
				for(int i = 0; i < listaInimigos.getTam(); i++){
					listaInimigos[i].andar(player.getPos(), largura-200, altura);
					listaInimigos[i].atualiza(listaProjeteisInimigos, player.getPos(), listaInimigos);
					listaInimigos[i].atualizaAngulo();
				}

				// colisao player e balas_inimigos
				for(int i = 0; i < listaProjeteisInimigos.getTam(); i++){
					if(colisao(player, listaProjeteisInimigos[i])){
						player.recebeDano(listaProjeteisInimigos[i].getDano());
						listaProjeteisInimigos.remove(i);
						i--;
						if(player.getVida() <= 0){
							estado = gameOver;
							break;
						}
					}
				}

				// colisao player e inimigo
				for(int i = 0; i < listaInimigos.getTam(); i++){
                    if(colisao(player, listaInimigos[i])){
                    	player.recebeDano(1);
					}
				}

				// colisao inimigos e balas_player
				for(int i = 0; i < listaInimigos.getTam(); i++){
					for(int j = 0; j < listaProjeteisPlayer.getTam(); j++){
						if(colisao(listaInimigos[i], listaProjeteisPlayer[j])){
							if(player.getArma() == rpg){
								timerExplosao.start();
								xExplosao = listaProjeteisPlayer[j].getPos().getX();
								yExplosao = listaProjeteisPlayer[j].getPos().getY();
								explosao = true;
								listaProjeteisPlayer.remove(j);
								j--;
							}else{
								listaInimigos[i].recebeDano(listaProjeteisPlayer[j].getDano());
								listaProjeteisPlayer.remove(j);
								j--;
								if(listaInimigos[i].getVida() <= 0){
									listaInimigos.remove(i);
									player.setDinheiro(player.getDinheiro() + 5);
									i--;
									break;
								}
							}
							
						}
					}
				}
				
				if(explosao){
					explosao = false;
					for(int i = 0; i < listaInimigos.getTam(); i++){
						if(distancia(Ponto(xExplosao, yExplosao), listaInimigos[i].getPos()) < 100){
							listaInimigos[i].recebeDano(12);
							if(listaInimigos[i].getVida() <= 0){
								listaInimigos.remove(i);
								player.setDinheiro(player.getDinheiro() + 5);
								i--;
							}
						}
					}
				}

				//Renderizacao:
				al_draw_bitmap(fundoJogo, 0, 0, 0);

				switch(player.getArma()){
					case pistola:
						al_draw_bitmap(img_pistola, 791, 202, 0);
						break;
					case shotgun:
						al_draw_bitmap(img_shotgun, 791, 202, 0);
						break;
					case sub:
						al_draw_bitmap(img_sub, 791, 202, 0);
						break;
					case rpg:
						al_draw_bitmap(img_rpg, 791, 202, 0);
						break;
					case sniper:
						al_draw_bitmap(img_sniper, 791, 202, 0);
						break;
					case laser:
						al_draw_bitmap(img_laser, 791, 202, 0);
						break;
				}
				
				if(timerExplosao.estaAtivo())
					al_draw_filled_circle(xExplosao, yExplosao, 40, al_map_rgb(255, 255, 255));
				
				for(int i = 0; i < listaProjeteisPlayer.getTam(); i++){
					listaProjeteisPlayer[i].desenhar(mostrarHitboxProjeteis);
				}
				for(int i = 0; i < listaInimigos.getTam(); i++){
					listaInimigos[i].desenhar(mostrarHitboxInimigos);
				}
				for(int i = 0; i < listaInimigos.getTam(); i++){
					listaInimigos[i].desenhaBarraVida();
				}
				for(int i = 0; i < listaProjeteisInimigos.getTam(); i++){
					listaProjeteisInimigos[i].desenhar(mostrarHitboxProjeteis);
				}
				
				
				al_draw_textf(arial24, al_map_rgb(255,255,255), 824, 86, 0, "$ %d", player.getDinheiro());

				player.desenhar(tecla[W] || tecla[UP] || tecla[S] || tecla[DOWN] || tecla[A]||tecla[LEFT]|| tecla[D]||tecla[RIGHT], mostrarHitboxPlayer);

				for(int i = 0; i < player.getVidaMax()/2; i++){
					al_draw_scaled_bitmap(coracaoVazio, 0, 0, 80, 68, 780+i*90*0.35, 340, 80*0.35, 68*0.35, 0);
				}
				for(int i = 0; i < player.getVida()/2; i++){
					al_draw_scaled_bitmap(coracaoCheio, 0, 0, 80, 68, 780+i*90*0.35, 340, 80*0.35, 68*0.35, 0);
				}
				for(int i = 0; i < (player.getVida()+1)/2; i++){
					al_draw_scaled_bitmap(coracaoMetade, 0, 0, 80, 68, 780+i*90*0.35, 340, 80*0.35, 68*0.35, 0);
				}

				al_flip_display();
				al_clear_to_color(al_map_rgb(255, 255, 255));
			}
			else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
				acabou = true;
			}
			else if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
				switch(evento.keyboard.keycode){
					case ALLEGRO_KEY_UP:
						tecla[UP] = true;
						break;
					case ALLEGRO_KEY_DOWN:
						tecla[DOWN] = true;
						break;
					case ALLEGRO_KEY_LEFT:
						tecla[LEFT] = true;
						break;
					case ALLEGRO_KEY_RIGHT:
						tecla[RIGHT] = true;
						break;
					case ALLEGRO_KEY_W:
						tecla[W] = true;
						break;
					case ALLEGRO_KEY_A:
						tecla[A] = true;
						break;
					case ALLEGRO_KEY_S:
						tecla[S] = true;
						break;
					case ALLEGRO_KEY_D:
						tecla[D] = true;
						break;
					case ALLEGRO_KEY_I:
						if(cheat)
							spawnarInimigo(listaInimigos, aranha, largura-200, altura, 5, 2, 180);
						break;
					case ALLEGRO_KEY_O:
						if(cheat)
							spawnarInimigo(listaInimigos, bp, largura-200, altura, 14, 2, 180);
						break;
					case ALLEGRO_KEY_P:
						if(cheat)
							spawnarInimigo(listaInimigos, palhaco, largura-200, altura, 20, 2, 180);
						break;
					case ALLEGRO_KEY_U:
						if(cheat)
							spawnarInimigo(listaInimigos, foguinho, largura-200, altura, 26, 2, 180);
						break;
                    case ALLEGRO_KEY_L:
                        estado = loja;
						for(int i = 0; i < N_TECLAS; i++) tecla[i] = false;
						for(int i = 0; i < N_MOUSE_BOTOES; i++) mouseBotao[i] = false;
                        break;
                    case ALLEGRO_KEY_Z:
                    	player.atirar(listaProjeteisPlayer, mouse, true);
                    	break;
                    case ALLEGRO_KEY_X:
                    	if(cheat){
                    		mostrarHitboxInimigos = !mostrarHitboxInimigos;
                    		mostrarHitboxPlayer = !mostrarHitboxPlayer;
                    		mostrarHitboxProjeteis = !mostrarHitboxProjeteis;
						}
                    	tecla[X] = true;
                    	break;
                    case ALLEGRO_KEY_B:
                    	if(cheat)
                    		spawnarInimigo(listaInimigos, boss1, largura-200, altura, 719, 2, 1);
                    	break;
                    case ALLEGRO_KEY_V:
                    	break;
                    case ALLEGRO_KEY_N:
                    	if(cheat)
                    		spawnarInimigo(listaInimigos, boss2, largura, altura-200, 1500, 0.3, 1);
                    	break;
                    case ALLEGRO_KEY_LCTRL:
                    	tecla[LCTRL] = true;
                    	break;
                    case ALLEGRO_KEY_LSHIFT:
                    	tecla[LSHIFT] = true;
                    	break;
                    case ALLEGRO_KEY_Q:
                    	if(tecla[LCTRL] && tecla[LSHIFT])
                    		cheat = !cheat;
                    	break;
                    case ALLEGRO_KEY_C:
                    	if(cheat)
                    		player.setDinheiro(player.getDinheiro()+100);
                    	break;
                    case ALLEGRO_KEY_H:
                    	if(cheat)
                    		player.setVida(player.getVidaMax());
                    	break;
				}
			}
			else if(evento.type == ALLEGRO_EVENT_KEY_UP){
				switch(evento.keyboard.keycode){
					case ALLEGRO_KEY_UP:
						tecla[UP] = false;
						break;
					case ALLEGRO_KEY_DOWN:
						tecla[DOWN] = false;
						break;
					case ALLEGRO_KEY_LEFT:
						tecla[LEFT] = false;
						break;
					case ALLEGRO_KEY_RIGHT:
						tecla[RIGHT] = false;
						break;
					case ALLEGRO_KEY_W:
						tecla[W] = false;
						break;
					case ALLEGRO_KEY_A:
						tecla[A] = false;
						break;
					case ALLEGRO_KEY_S:
						tecla[S] = false;
						break;
					case ALLEGRO_KEY_D:
						tecla[D] = false;
						break;
					case ALLEGRO_KEY_X:
                    	tecla[X] = false;
                    	break;
                    case ALLEGRO_KEY_LCTRL:
                    	tecla[LCTRL] = false;
                    	break;
                    case ALLEGRO_KEY_LSHIFT:
                    	tecla[LSHIFT] = false;
                    	break;
				}
			}
			else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				if(evento.mouse.button == 1){
					player.atirar(listaProjeteisPlayer, mouse, true);
					mouseBotao[M1] = true;
				}
				if(evento.mouse.button == 2){
					mouseBotao[M2] = true;
				}
				if(evento.mouse.button == 3){
					mouseBotao[M3] = true;
				}
			}
			else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
				if(evento.mouse.button == 1){
					mouseBotao[M1] = false;
				}
				if(evento.mouse.button == 2){
					mouseBotao[M2] = false;
				}
				if(evento.mouse.button == 3){
					mouseBotao[M3] = false;
				}
			}
			else if(evento.type == ALLEGRO_EVENT_MOUSE_AXES){
				mouse.setX(evento.mouse.x / sx);
				mouse.setY(evento.mouse.y / sy);
			}
			else if(evento.type == ALLEGRO_EVENT_DISPLAY_RESIZE){
				al_resize_display(tela, evento.display.width, evento.display.height);
				sx = al_get_display_width(tela) / (float)largura;
				sy = al_get_display_height(tela) / (float)altura;
				ALLEGRO_TRANSFORM trans;
				al_identity_transform(&trans);
				al_scale_transform(&trans, sx, sy);
				al_use_transform(&trans);
			}
		}
		else if(estado == loja){

			ALLEGRO_EVENT evento;
	        al_wait_for_event(filaEventos, &evento);

	    	if(evento.type == ALLEGRO_EVENT_TIMER){
	            btnComprarPistola.atualiza(mouse.getX(), mouse.getY());
				btnComprarShotgun.atualiza(mouse.getX(), mouse.getY());
				btnComprarSub.atualiza(mouse.getX(), mouse.getY());
				btnComprarRPG.atualiza(mouse.getX(), mouse.getY());
				btnComprarSniper.atualiza(mouse.getX(), mouse.getY());
				btnComprarLaser.atualiza(mouse.getX(), mouse.getY());
				btnComprarVida.atualiza(mouse.getX(), mouse.getY());
				btnComprarVelocidade.atualiza(mouse.getX(), mouse.getY());
				btnComprarMedicamento.atualiza(mouse.getX(), mouse.getY());

				al_draw_bitmap(fundoLoja, 0, 0, 0);

				switch(player.getArma()){
					case pistola:
						al_draw_bitmap(img_pistola, 791, 202, 0);
						break;
					case shotgun:
						al_draw_bitmap(img_shotgun, 791, 202, 0);
						break;
					case sub:
						al_draw_bitmap(img_sub, 791, 202, 0);
						break;
					case rpg:
						al_draw_bitmap(img_rpg, 791, 202, 0);
						break;
					case sniper:
						al_draw_bitmap(img_sniper, 791, 202, 0);
						break;
					case laser:
						al_draw_bitmap(img_laser, 791, 202, 0);
						break;
				}

				for(int i = 0; i < player.getVidaMax()/2; i++){
					al_draw_scaled_bitmap(coracaoVazio, 0, 0, 80, 68, 780+i*90*0.35, 340, 80*0.35, 68*0.35, 0);
				}
				for(int i = 0; i < player.getVida()/2; i++){
					al_draw_scaled_bitmap(coracaoCheio, 0, 0, 80, 68, 780+i*90*0.35, 340, 80*0.35, 68*0.35, 0);
				}
				for(int i = 0; i < (player.getVida()+1)/2; i++){
					al_draw_scaled_bitmap(coracaoMetade, 0, 0, 80, 68, 780+i*90*0.35, 340, 80*0.35, 68*0.35, 0);
				}

				al_draw_text(arial24, al_map_rgb(255,255,255), 200, 107, 0, "FREE");
				al_draw_textf(arial24, al_map_rgb(255,255,255), 450, 107, 0, "%d", preco_shotgun);
				al_draw_textf(arial24, al_map_rgb(255,255,255), 700, 107, 0, "%d", preco_sub);
				al_draw_textf(arial24, al_map_rgb(255,255,255), 200, 260, 0, "%d", preco_rpg);
				al_draw_textf(arial24, al_map_rgb(255,255,255), 450, 260, 0, "%d", preco_sniper);
				al_draw_textf(arial24, al_map_rgb(255,255,255), 700, 260, 0, "%d", preco_laser);
				al_draw_textf(arial24, al_map_rgb(255,255,255), 200, 416, 0, "%d", preco_vida);
				al_draw_textf(arial24, al_map_rgb(255,255,255), 450, 416, 0, "%d", preco_velocidade);
				al_draw_textf(arial24, al_map_rgb(255,255,255), 700, 416, 0, "%d", preco_kit);

				btnComprarPistola.desenhar();
				btnComprarShotgun.desenhar();
				btnComprarSub.desenhar();
				btnComprarRPG.desenhar();
				btnComprarSniper.desenhar();
				btnComprarLaser.desenhar();
				btnComprarVida.desenhar();
				btnComprarVelocidade.desenhar();
				btnComprarMedicamento.desenhar();

				al_draw_textf(arial24, al_map_rgb(255,255,255), 824, 86, 0, "%d", player.getDinheiro());

				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
	        }

	        else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){

	        	if(evento.mouse.button == 1){

	        		if(btnComprarPistola.estaSelecionado()){
						if(!comprouArma[pistola]){
							if(player.getDinheiro() >= preco_shotgun){
								player.setArma(pistola);
								player.setDinheiro(player.getDinheiro() - preco_pistola);
								comprouArma[pistola] = true;
								btnComprarPistola.setImagem(frescura);
							}else{
								cout << "dinheiro insuficiente" << endl;
							}
						}else{
							player.setArma(pistola);
						}
					}

					else if(btnComprarShotgun.estaSelecionado()){
						if(!comprouArma[shotgun]){
							if(player.getDinheiro() >= preco_shotgun){
								player.setArma(shotgun);
								player.setDinheiro(player.getDinheiro() - preco_shotgun);
								comprouArma[shotgun] = true;
								btnComprarShotgun.setImagem(img_usar);
							}else{
								cout << "dinheiro insuficiente" << endl;
							}
						}else{
							player.setArma(shotgun);
						}
					}
					else if(btnComprarSub.estaSelecionado()){
						if(!comprouArma[sub]){
							if(player.getDinheiro() >= preco_sub){
								player.setArma(sub);
								player.setDinheiro(player.getDinheiro() - preco_sub);
								comprouArma[sub] = true;
								btnComprarSub.setImagem(img_usar);
							}else{
								cout << "dinheiro insuficiente" << endl;
							}
						}else{
							player.setArma(sub);
						}
					}
					else if(btnComprarRPG.estaSelecionado()){
						if(!comprouArma[rpg]){
							if(player.getDinheiro() >= preco_rpg){
								player.setArma(rpg);
								player.setDinheiro(player.getDinheiro() - preco_rpg);
								comprouArma[rpg] = true;
								btnComprarRPG.setImagem(img_usar);
							}else{
								cout << "dinheiro insuficiente" << endl;
							}
						}else{
							player.setArma(rpg);
						}
					}
					else if(btnComprarSniper.estaSelecionado()){
						if(!comprouArma[sniper]){
							if(player.getDinheiro() >= preco_sniper){
								player.setArma(sniper);
								player.setDinheiro(player.getDinheiro() - preco_sniper);
								comprouArma[sniper] = true;
								btnComprarSniper.setImagem(img_usar);
							}else{
								cout << "dinheiro insuficiente" << endl;
							}
						}else{
							player.setArma(sniper);
						}
					}
					else if(btnComprarLaser.estaSelecionado()){
						if(!comprouArma[laser]){
							if(player.getDinheiro() >= preco_laser){
								player.setArma(laser);
								player.setDinheiro(player.getDinheiro() - preco_laser);
								comprouArma[laser] = true;
								btnComprarLaser.setImagem(img_usar);
							}else{
								cout << "dinheiro insuficiente" << endl;
							}
						}else{
							player.setArma(laser);
						}
					}
					else if(btnComprarVida.estaSelecionado()){
						if(player.getVidaMax() < 10){
							if(player.getDinheiro() >= preco_vida){
								player.setDinheiro(player.getDinheiro() - preco_vida);
								player.setVidaMax(player.getVidaMax() + 2);
								if(player.getVidaMax() == 10){
									btnComprarVida.setImagem(img_max);
								}
							}else{
								cout << "dinheiro insuficiente" << endl;
							}
						}else{
							cout << "ja esta no maximo" << endl;
						}

					}
					else if(btnComprarVelocidade.estaSelecionado()){
                        if(player.getVelocidade() < 5){
                            if(player.getDinheiro() >= preco_velocidade){
                            	player.setDinheiro(player.getDinheiro() - preco_velocidade);
                            	player.setVelocidade(player.getVelocidade() + 0.5);
                            	if(player.getVelocidade() >= 5){
									btnComprarVida.setImagem(img_max);
								}
                            }else{
                                cout << "dinheiro insuficiente" << endl;
                            }
                        }
                        else{
                            btnComprarVelocidade.setImagem(img_max);
                            cout << "ja esta no maximo" << endl;
                        }

					}
					else if(btnComprarMedicamento.estaSelecionado()){
						if(player.getVida() < player.getVidaMax()){
							if(player.getDinheiro() >= preco_kit){
								player.setDinheiro(player.getDinheiro() - preco_kit);
								player.setVida(player.getVida()+2);
								if(player.getVida() > player.getVidaMax()){
									player.setVida(player.getVidaMax());
								}
							}else{
								cout << "dinheiro insuficiente" << endl;
							}
						}else{
							cout << "a vida ja esta cheia" << endl;
						}
					}
				}

				if(evento.mouse.button == 2){

				}
				if(evento.mouse.button == 3){

				}
			}

			else if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
				switch(evento.keyboard.keycode){
					case ALLEGRO_KEY_L:
						estado = jogando;
						break;
					case ALLEGRO_KEY_ESCAPE:
						estado = jogando;
						break;
				}
			}


	        else if(evento.type == ALLEGRO_EVENT_MOUSE_AXES){
	            mouse.setX(evento.mouse.x / sx);
	            mouse.setY(evento.mouse.y / sy);
	        }
		}// fim loja

        else if(estado == gameOver){
            al_draw_bitmap(fundoGameOver, 0, 0, 0);
            al_flip_display();

        }
	}



	return 0;
}

bool colisao(Player p, Projetil b){
	Vetor normals[4] = {
		Vetor(p.getVertice(0), p.getVertice(1)),
		Vetor(p.getVertice(1), p.getVertice(2)),
		Vetor(b.getVertice(0), b.getVertice(1)),
		Vetor(b.getVertice(1), b.getVertice(2))};
	Vetor vecs_box1[4];
	Vetor vecs_box2[4];
	for(int i = 0; i < 4; i++) {
	    vecs_box1[i] = Vetor(p.getVertice(i).getX(), p.getVertice(i).getY());
		vecs_box2[i] = Vetor(b.getVertice(i).getX(), b.getVertice(i).getY());
	}
	for(int i = 0; i < 4; i++){
		Vetor axis = normals[i];
		double min_proj_box1 = vecs_box1[0]*axis;
		double max_proj_box1 = vecs_box1[0]*axis;
		double min_proj_box2 = vecs_box2[0]*axis;
		double max_proj_box2 = vecs_box2[0]*axis;
		for(int j = 1; j < 4; j++){
			double curr_proj1 = vecs_box1[j]*axis;
		    double curr_proj2 = vecs_box2[j]*axis;
		    if(curr_proj1 < min_proj_box1){
		        min_proj_box1 = curr_proj1;
		    }
		    if(curr_proj1 > max_proj_box1){
		        max_proj_box1 = curr_proj1;
		    }
		    if(curr_proj2 < min_proj_box2){
		        min_proj_box2 = curr_proj2;
		    }
		    if(curr_proj2 > max_proj_box2){
		        max_proj_box2 = curr_proj2;
		    }
		}
		if(max_proj_box2 < min_proj_box1 || max_proj_box1 < min_proj_box2)
			return false;
	}
	return true;
}

bool colisao(Player p, Inimigo b){
	Vetor normals[4] = {
		Vetor(p.getVertice(0), p.getVertice(1)),
		Vetor(p.getVertice(1), p.getVertice(2)),
		Vetor(b.getVertice(0), b.getVertice(1)),
		Vetor(b.getVertice(1), b.getVertice(2))};
	Vetor vecs_box1[4];
	Vetor vecs_box2[4];
	for(int i = 0; i < 4; i++) {
	    vecs_box1[i] = Vetor(p.getVertice(i).getX(), p.getVertice(i).getY());
		vecs_box2[i] = Vetor(b.getVertice(i).getX(), b.getVertice(i).getY());
	}
	for(int i = 0; i < 4; i++){
		Vetor axis = normals[i];
		double min_proj_box1 = vecs_box1[0]*axis;
		double max_proj_box1 = vecs_box1[0]*axis;
		double min_proj_box2 = vecs_box2[0]*axis;
		double max_proj_box2 = vecs_box2[0]*axis;
		for(int j = 1; j < 4; j++){
			double curr_proj1 = vecs_box1[j]*axis;
		    double curr_proj2 = vecs_box2[j]*axis;
		    if(curr_proj1 < min_proj_box1){
		        min_proj_box1 = curr_proj1;
		    }
		    if(curr_proj1 > max_proj_box1){
		        max_proj_box1 = curr_proj1;
		    }
		    if(curr_proj2 < min_proj_box2){
		        min_proj_box2 = curr_proj2;
		    }
		    if(curr_proj2 > max_proj_box2){
		        max_proj_box2 = curr_proj2;
		    }
		}
		if(max_proj_box2 < min_proj_box1 || max_proj_box1 < min_proj_box2)
			return false;
	}
	return true;
}

bool colisao(Inimigo p, Projetil& b){
	Vetor normals[4] = {
		Vetor(p.getVertice(0), p.getVertice(1)),
		Vetor(p.getVertice(1), p.getVertice(2)),
		Vetor(b.getVertice(0), b.getVertice(1)),
		Vetor(b.getVertice(1), b.getVertice(2))};
	Vetor vecs_box1[4];
	Vetor vecs_box2[4];
	for(int i = 0; i < 4; i++) {
	    vecs_box1[i] = Vetor(p.getVertice(i).getX(), p.getVertice(i).getY());
		vecs_box2[i] = Vetor(b.getVertice(i).getX(), b.getVertice(i).getY());
	}
	for(int i = 0; i < 4; i++){
		Vetor axis = normals[i];
		double min_proj_box1 = vecs_box1[0]*axis;
		double max_proj_box1 = vecs_box1[0]*axis;
		double min_proj_box2 = vecs_box2[0]*axis;
		double max_proj_box2 = vecs_box2[0]*axis;
		for(int j = 1; j < 4; j++){
			double curr_proj1 = vecs_box1[j]*axis;
		    double curr_proj2 = vecs_box2[j]*axis;
		    if(curr_proj1 < min_proj_box1){
		        min_proj_box1 = curr_proj1;
		    }
		    if(curr_proj1 > max_proj_box1){
		        max_proj_box1 = curr_proj1;
		    }
		    if(curr_proj2 < min_proj_box2){
		        min_proj_box2 = curr_proj2;
		    }
		    if(curr_proj2 > max_proj_box2){
		        max_proj_box2 = curr_proj2;
		    }
		}
		if(max_proj_box2 < min_proj_box1 || max_proj_box1 < min_proj_box2){
			return false;
		}

	}
	return true;
}



void spawnarInimigo(Lista<Inimigo>& listaInimigos, int tipo, int larguraTela, int alturaTela, int vida, double velocidade, int cooldown){
	int x, y;
	switch(aleatorio(0, 3)){
		case 0:
			x = aleatorio(0, larguraTela);
			y = 0;
			break;
		case 1:
			x = 0;
			y = aleatorio(0, alturaTela);
			break;
		case 2:
			x = aleatorio(0, larguraTela);
			y = alturaTela;
			break;
		case 3:
			x = larguraTela;
			y = aleatorio(0, alturaTela);
			break;
	}
	switch(tipo){
		case palhaco:
			listaInimigos.insereNoInicio(Inimigo(Ponto(x, y), Vetor(1, 1), velocidade, vida, palhaco, cooldown));
			break;
		case bp:
			listaInimigos.insereNoInicio(Inimigo(Ponto(x, y), Vetor(1, 1), velocidade, vida, bp, cooldown));
			break;
		case aranha:
			listaInimigos.insereNoInicio(Inimigo(Ponto(x, y), Vetor(1, 1), velocidade, vida, aranha, cooldown));
			break;
		case foguinho:
			listaInimigos.insereNoInicio(Inimigo(Ponto(x, y), Vetor(1, 1), velocidade, vida, foguinho, cooldown));
			break;
		case mariposa:
			listaInimigos.insereNoInicio(Inimigo(Ponto(x, y), Vetor(1, 1), velocidade, vida, mariposa, cooldown));
			break;
		case boss1:
			listaInimigos.insereNoInicio(Inimigo(Ponto(0, 0), Vetor(1, 1), velocidade, vida, boss1, cooldown));
			break;
		case boss2:
			listaInimigos.insereNoInicio(Inimigo(Ponto(780, 270), Vetor(-1, 0), velocidade, vida, boss2, cooldown));
			break;
	}
}

void atualizaWave(int& wave, Timer& timerWave, Lista<Inimigo>& listaInimigos, int larguraTela, int alturaTela){
	timerWave.update();
	switch(wave){
		case 1:
			switch(timerWave.getContador()){
				case 34*60:
					break;
				case 33*60:
					break;
				case 32*60:
					break;
				case 31*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 30*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 29*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 28*60:
					break;
				case 27*60:
					break;
				case 26*60:
					break;
				case 25*60:
					break;
				case 24*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 23*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 22*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 21*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 20*60:
					break;
				case 19*60:
					break;
				case 18*60:
					break;
				case 17*60:
					break;
				case 16*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 15*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 14*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 13*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 12*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 11*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 10*60:
					break;
				case 9*60:
					break;
				case 8*60:
					break;
				case 7*60:
					break;
				case 6*60:
					break;
				case 5*60:
					break;
				case 4*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 3*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 2*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 1*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
			}
			if(!timerWave.estaAtivo() && listaInimigos.vazia()){
				timerWave.setMaximo(35*60);
				timerWave.start();
				wave++;
			}
			break;
		case 2:
			switch(timerWave.getContador()){
				case 34*60:
					break;
				case 33*60:
					break;
				case 32*60:
					break;
				case 31*60:
					break;
				case 30*60:
					break;
				case 29*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 28*60:
					break;
				case 27*60:
					break;
				case 26*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 25*60:
					break;
				case 24*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 23*60:
					break;
				case 22*60:
					break;
				case 21*60:
					break;
				case 20*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 19*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 18*60:
					break;
				case 17*60:
					break;
				case 16*60:
					break;
				case 15*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 14*60:
					break;
				case 13*60:
					break;
				case 12*60:
					break;
				case 11*60:
					break;
				case 10*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 9*60:
					break;
				case 8*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 7*60:
					break;
				case 6*60:
					break;
				case 5*60:
					break;
				case 4*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 3*60:
					break;
				case 2*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 1*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
			}
			if(!timerWave.estaAtivo() && listaInimigos.vazia()){
				timerWave.setMaximo(35*60);
				timerWave.start();
				wave++;
			}
			break;
		case 3:
			switch(timerWave.getContador()){
				case 34*60:
					break;
				case 33*60:
					break;
				case 32*60:
					break;
				case 31*60:
					break;
				case 30*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 29*60:
					break;
				case 28*60:
					break;
				case 27*60:
					break;
				case 26*60:
					break;
				case 25*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 24*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 23*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 22*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 21*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 20*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 19*60:
					break;
				case 18*60:
					break;
				case 17*60:
					break;
				case 16*60:
					break;
				case 15*60:
					break;
				case 14*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 13*60:
					break;
				case 12*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 11*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 10*60:
					break;
				case 9*60:
					break;
				case 8*60:
					break;
				case 7*60:
					break;
				case 6*60:
					break;
				case 5*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 4*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 3*60:
					break;
				case 2*60:
					break;
				case 1*60:
					break;
			}
			if(!timerWave.estaAtivo() && listaInimigos.vazia()){
				timerWave.setMaximo(35*60);
				timerWave.start();
				wave++;
			}
			break;
		case 4:
			switch(timerWave.getContador()){
				case 34*60:
					break;
				case 33*60:
					break;
				case 32*60:
					break;
				case 31*60:
					break;
				case 30*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 29*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 28*60:
					break;
				case 27*60:
					break;
				case 26*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 25*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 24*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 23*60:
					break;
				case 22*60:
					break;
				case 21*60:
					break;
				case 20*60:
					break;
				case 19*60:
					break;
				case 18*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 17*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 16*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 15*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 14*60:
					break;
				case 13*60:
					break;
				case 12*60:
					break;
				case 11*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 10*60:
					break;
				case 9*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 8*60:
					break;
				case 7*60:
					break;
				case 6*60:
					break;
				case 5*60:
					break;
				case 4*60:
					break;
				case 3*60:
					break;
				case 2*60:
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 1*60:
					break;
			}
			if(!timerWave.estaAtivo() && listaInimigos.vazia()){
				timerWave.setMaximo(35*60);
				timerWave.start();
				wave++;
			}
			break;
		case 5:
			switch(timerWave.getContador()){
				case 34*60:
					break;
				case 33*60:
					break;
				case 32*60:
					break;
				case 31*60:
					break;
				case 30*60:
					spawnarInimigo(listaInimigos, boss1, larguraTela, alturaTela, 719, 2, 1);
					break;
				case 29*60:
					break;
				case 28*60:
					break;
				case 27*60:
					break;
				case 26*60:
					break;
				case 25*60:
					break;
				case 24*60:
					break;
				case 23*60:
					break;
				case 22*60:
					break;
				case 21*60:
					break;
				case 20*60:
					break;
				case 19*60:
					break;
				case 18*60:
					break;
				case 17*60:
					break;
				case 16*60:
					break;
				case 15*60:
					break;
				case 14*60:
					break;
				case 13*60:
					break;
				case 12*60:
					break;
				case 11*60:
					break;
				case 10*60:
					break;
				case 9*60:
					break;
				case 8*60:
					break;
				case 7*60:
					break;
				case 6*60:
					break;
				case 5*60:
					break;
				case 4*60:
					break;
				case 3*60:
					break;
				case 2*60:
					break;
				case 1*60:
					break;
			}
			if(!timerWave.estaAtivo() && listaInimigos.vazia()){
				timerWave.setMaximo(35*60);
				timerWave.start();
				wave++;
			}
			break;
		case 6:
			switch(timerWave.getContador()){
				case 34*60:
					break;
				case 33*60:
					break;
				case 32*60:
					break;
				case 31*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 30*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 29*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 28*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 27*60:
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 26*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 25*60:
					break;
				case 24*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 23*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 22*60:
					break;
				case 21*60:
					break;
				case 20*60:
					break;
				case 19*60:
					break;
				case 18*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 17*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 16*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 15*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 14*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 13*60:
					break;
				case 12*60:
					break;
				case 11*60:
					break;
				case 10*60:
					break;
				case 9*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 8*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					break;
				case 7*60:
					break;
				case 6*60:
					break;
				case 5*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 4*60:
					break;
				case 3*60:
					break;
				case 2*60:
					break;
				case 1*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
			}
			if(!timerWave.estaAtivo() && listaInimigos.vazia()){
				timerWave.setMaximo(35*60);
				timerWave.start();
				wave++;
			}
			break;
		case 7:
			switch(timerWave.getContador()){
				case 34*60:
					break;
				case 33*60:
					break;
				case 32*60:
					break;
				case 31*60:
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 30*60:
					break;
				case 29*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 28*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 27*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 26*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 25*60:
					break;
				case 24*60:
					break;
				case 23*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 22*60:
					break;
				case 21*60:
					break;
				case 20*60:
					break;
				case 19*60:
					break;
				case 18*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 17*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
				case 16*60:
					break;
				case 15*60:
					break;
				case 14*60:
					break;
				case 13*60:
					break;
				case 12*60:
					break;
				case 11*60:
					break;
				case 10*60:
					break;
				case 9*60:
					break;
				case 8*60:
					break;
				case 7*60:
					break;
				case 6*60:
					break;
				case 5*60:
					break;
				case 4*60:
					break;
				case 3*60:
					break;
				case 2*60:
					break;
				case 1*60:
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					break;
			}
			if(!timerWave.estaAtivo() && listaInimigos.vazia()){
				timerWave.setMaximo(35*60);
				timerWave.start();
				wave++;
			}
			break;
		case 8:
			switch(timerWave.getContador()){
				case 34*60:
					break;
				case 33*60:
					break;
				case 32*60:
					break;
				case 31*60:
					break;
				case 30*60:
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 29*60:
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 28*60:
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 27*60:
					break;
				case 26*60:
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 25*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 24*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 23*60:
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 22*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 21*60:
					break;
				case 20*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 19*60:
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 18*60:
					break;
				case 17*60:
					break;
				case 16*60:
					break;
				case 15*60:
					break;
				case 14*60:
					break;
				case 13*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 12*60:
					break;
				case 11*60:
					break;
				case 10*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 9*60:
					break;
				case 8*60:
					break;
				case 7*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 6*60:
					break;
				case 5*60:
					break;
				case 4*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 3*60:
					break;
				case 2*60:
					break;
				case 1*60:
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
			}
			if(!timerWave.estaAtivo() && listaInimigos.vazia()){
				timerWave.setMaximo(35*60);
				timerWave.start();
				wave++;
			}
			break;
		case 9:
			switch(timerWave.getContador()){
				case 34*60:
					break;
				case 33*60:
					break;
				case 32*60:
					break;
				case 31*60:
					break;
				case 30*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 29*60:
					break;
				case 28*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 27*60:
					break;
				case 26*60:
					break;
				case 25*60:
					break;
				case 24*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 23*60:
					break;
				case 22*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 21*60:
					break;
				case 20*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 19*60:
					break;
				case 18*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 17*60:
					break;
				case 16*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 15*60:
					break;
				case 14*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					break;
				case 13*60:
					break;
				case 12*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 11*60:
					break;
				case 10*60:
					break;
				case 9*60:
					break;
				case 8*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					spawnarInimigo(listaInimigos, foguinho, larguraTela, alturaTela, 26, 2, 180);
					spawnarInimigo(listaInimigos, mariposa, larguraTela, alturaTela, 40, 2, 180);
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
				case 7*60:
					break;
				case 6*60:
					break;
				case 5*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, aranha, larguraTela, alturaTela, 5, 2, 180);
					break;
				case 4*60:
					break;
				case 3*60:
					break;
				case 2*60:
					break;
				case 1*60:
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					spawnarInimigo(listaInimigos, bp, larguraTela, alturaTela, 14, 2, 180);
					spawnarInimigo(listaInimigos, palhaco, larguraTela, alturaTela, 20, 2, 180);
					break;
			}
			if(!timerWave.estaAtivo() && listaInimigos.vazia()){
				timerWave.setMaximo(35*60);
				timerWave.start();
				wave++;
			}
			break;
		case 10:
			switch(timerWave.getContador()){
				case 34*60:
					break;
				case 33*60:
					break;
				case 32*60:
					break;
				case 31*60:
					spawnarInimigo(listaInimigos, boss2, larguraTela, alturaTela, 1500, 0.3, 1);
					break;
				case 30*60:
					break;
				case 29*60:
					break;
				case 28*60:
					break;
				case 27*60:
					break;
				case 26*60:
					break;
				case 25*60:
					break;
				case 24*60:
					break;
				case 23*60:
					break;
				case 22*60:
					break;
				case 21*60:
					break;
				case 20*60:
					break;
				case 19*60:
					break;
				case 18*60:
					break;
				case 17*60:
					break;
				case 16*60:
					break;
				case 15*60:
					break;
				case 14*60:
					break;
				case 13*60:
					break;
				case 12*60:
					break;
				case 11*60:
					break;
				case 10*60:
					break;
				case 9*60:
					break;
				case 8*60:
					break;
				case 7*60:
					break;
				case 6*60:
					break;
				case 5*60:
					break;
				case 4*60:
					break;
				case 3*60:
					break;
				case 2*60:
					break;
				case 1*60:
					break;
			}
			if(!timerWave.estaAtivo() && listaInimigos.vazia()){
				timerWave.setMaximo(35*60);
				timerWave.start();
				wave++;
			}
			break;
	}
}
