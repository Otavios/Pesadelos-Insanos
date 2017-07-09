#include "includes.h"

#include "geometria.h"
#include "player.h"
#include "projetil.h"
#include "inimigo.h"
#include "botao.h"
#include "lista.h"

#define preco_pistola 0
#define preco_shotgun 1
#define preco_sub 2
#define preco_rpg 25
#define preco_sniper 3
#define preco_laser 35
#define preco_vida 40
#define preco_velocidade 4
#define preco_kit 5

bool colisao(Player p, Projetil b);
bool colisao(Player p, Inimigo b);
bool colisao(Inimigo p, Projetil& b);

void spawnarInimigo(Lista<Inimigo>& listaInimigos, int tipo, int larguraTela, int alturaTela);

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

	Player player(Ponto(300, 300), 4, 13, 30, 6, 0, 0);
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
				// atualiza player
				player.andar(tecla[W]||tecla[UP], tecla[S]||tecla[DOWN], tecla[A]||tecla[LEFT], tecla[D]||tecla[RIGHT], largura-200, altura, mouse);
				player.atualizaAngulo(mouse);
				player.atualizaTimers();
				// player.atualizaTempoRecarga(); // ttttttttttttttt
				
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
							// nao muda para gameOver, apenas para testar:
							/*estado = gameOver;
							break;*/
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
				
				al_draw_textf(arial24, al_map_rgb(255,255,255), 824, 86, 0, "%d", player.getDinheiro());
				
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
						spawnarInimigo(listaInimigos, aranha, largura-200, altura);
						break;
					case ALLEGRO_KEY_O:
						spawnarInimigo(listaInimigos, bp, largura-200, altura);
						break;
					case ALLEGRO_KEY_P:
						spawnarInimigo(listaInimigos, palhaco, largura-200, altura);
						break;
					case ALLEGRO_KEY_U:
						spawnarInimigo(listaInimigos, foguinho, largura-200, altura);
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
                    	mostrarHitboxInimigos = !mostrarHitboxInimigos;
                    	mostrarHitboxPlayer = !mostrarHitboxPlayer;
                    	mostrarHitboxProjeteis = !mostrarHitboxProjeteis;
                    	tecla[X] = true;
                    	break;
                    case ALLEGRO_KEY_B:
                    	listaInimigos.insereNoInicio(Inimigo(Ponto(100, 100), Vetor(1, 1), 3, 1000, boss1, 1));
                    	break;
                    case ALLEGRO_KEY_V:
                    	break;
                    case ALLEGRO_KEY_N:
                    	listaInimigos.insereNoInicio(Inimigo(Ponto(780, 270), Vetor(-1, 0), 0.3, 1000, boss2, 1));
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
						if(player.getDinheiro() >= preco_velocidade){
							player.setDinheiro(player.getDinheiro() - preco_vida);
							player.setVelocidade(player.getVelocidade() + 1);
						}else{
							cout << "dinheiro insuficiente" << endl;
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
		}
	} // fim loja

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



void spawnarInimigo(Lista<Inimigo>& listaInimigos, int tipo, int larguraTela, int alturaTela){
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
			listaInimigos.insereNoInicio(Inimigo(Ponto(x, y), Vetor(1, 1), 3, 10, palhaco, 180));
			break;
		case bp:
			listaInimigos.insereNoInicio(Inimigo(Ponto(x, y), Vetor(1, 1), 3, 10, bp, 180));
			break;
		case aranha:
			listaInimigos.insereNoInicio(Inimigo(Ponto(x, y), Vetor(1, 1), 3, 10, aranha, 180));
			break;
		case foguinho:
			listaInimigos.insereNoInicio(Inimigo(Ponto(x, y), Vetor(1, 1), 3, 10, foguinho, 180));
			break;
	}
}
