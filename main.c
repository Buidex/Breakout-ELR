/*
 * main.c
 * 
 * Copyright 2016 Adriano Cruz <adriano@nce.ufrj.br>
 * 
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Defs.h"
#include "Globais.h"
#include "funcoes.h"

int main( int argc, char* args[] ) {
	
    SDL_Rect srcRect, dstRect;
    int quit1, quit2;
    int x, y;
    /*Manipulador de Eventos*/
    SDL_Event sair, e;
    
    /*Inicia o SDL e cria a janela*/
    if( !init() ) {
        printf( "Falha ao inicializar!\n" );
    }
    else {
        /*Carrega a midia*/
        if( !loadMedia() ) {
            printf( "Falha ao carregar midia!\n" );
        }
        /*Carrega os áudios*/
		if( !loadBaile() ) {
			printf("Falha ao carregar o áudio do Baile!\n");
		}
		if( !loadMandela() ) {
			printf("Falha ao carregar o áudio do Mandela!\n");
		}
        else {
			
			gPontos = 0;
			
			gerarTelaInicial();
			
			quit1 = false;
			gMuteSoundFlag = false;
			gQuitFlag = false;
            
            /*Enquanto o programa esta rodando (enquanto o valor de 'quit' for falso/0)*/
            while(!quit1) {
				gTimer = SDL_GetTicks();
                while( SDL_PollEvent( &sair ) != 0 ) {
					/*Essa função retorna '1' se houver um evento rolando e '0' se não houver*/
                    switch (sair.type) {
                        case SDL_QUIT:
                            quit1 = true;
                            break; 
                        case SDL_KEYDOWN:
                            if (sair.key.keysym.sym == SDLK_ESCAPE) {
								loadBaile();
                                gerarTelaInicial();
                            }
                            if(sair.key.keysym.sym == SDLK_m){
								if( Mix_PlayingMusic() == 0 ) { 	
									Mix_PlayMusic( gMandela, -1 ); 
									Mix_PlayMusic( gBaile, -1 ); 
								} 
								else { 
									if( Mix_PausedMusic() == 1 ) { 	
										Mix_ResumeMusic(); 
									} 
									else { 
										Mix_PauseMusic(); 
									} 
								} 
							}
							break;
                        case SDL_MOUSEBUTTONDOWN:
							if(sair.button.button == SDL_BUTTON_LEFT && gSurfaceJOGAR != NULL){
								x = sair.button.x;
								y = sair.button.y;
								if((x>SCREEN_WIDTH-520) && (x<SCREEN_WIDTH-520+222) && (y>SCREEN_HEIGHT-435) && (y<SCREEN_HEIGHT-435+62)){

									/*Libera a memória da música da tela inicial*/
									loadMandela();
					
									if( gWindow == NULL ) {
										printf("A janela da tela inicial nao pode ser criada! SDL Error: %s\n", SDL_GetError());
										quit1 = true;
									}
									else {
										/*Troco a superfície inicial para a superfície de Jogo e ativo todos os componentes dessa nova superfície*/
										
										/*Captura a superficie da janela da tela inicial*/
										gSurfaceJogo = SDL_GetWindowSurface(gWindow);
										SDL_FillRect (gSurfaceJogo, NULL, SDL_MapRGB(gSurfaceJogo->format, 0xFF, 0xFF, 0XFF));
										Mix_PlayMusic( gMandela, -1 );
										
										bola = criaBOLA(((SCREEN_WIDTH/2) - (BALL_WIDTH/2)),
														((SCREEN_HEIGHT/2) - (BALL_HEIGHT/2)), 
														0, 
														0,
														gBALLSurface);
           
            
										plataforma = criaPLATAFORMA(((SCREEN_WIDTH/2) - (PADDLE_WIDTH/2)),
																	(SCREEN_HEIGHT - 2*PADDLE_HEIGHT),
																	VELOCIDADE_JOGADOR,
																	gPADDLESurface);
										
										gLevel = 1;
										gVidas = NUM_VIDAS;
										gPontos = 0;
										gContaPontos = 0;
										gNumBlocos = 0;
										
										criaBlocos(gBlocos);
             
										/*Flag do loop principal*/
										quit2 = false;
										gQuitFlag = false;

										/*Enquanto estiver em execucao*/           
										while( !quit2 ) {
											gTimer = SDL_GetTicks();
											while( SDL_PollEvent( &e ) != 0 ) {
												switch (e.type) {
													case SDL_QUIT:
														quit2 = true;
														quit1 = true;
														break;
													case SDL_KEYDOWN:
														switch (e.key.keysym.sym) {
															case SDLK_ESCAPE:
																gQuitFlag = true;
																break;
															case SDLK_LEFT:
																gTeclaEsq = 1;
																break;
															case SDLK_RIGHT:
																gTeclaDir = 1;
																break;
															case SDLK_SPACE:
																if (bola.velY == 0) {
																	bola.velY = VELOCIDADE_INIC_BOLA;
																}
																break;
															case SDLK_m:
																if( Mix_PlayingMusic() == 0 ) { 	
																	Mix_PlayMusic( gMandela, -1 ); 
																	Mix_PlayMusic( gBaile, -1 ); 
																} 
																else { 
																	if( Mix_PausedMusic() == 1 ) { 	
																	Mix_ResumeMusic(); 
																	}
																	else { 
																	Mix_PauseMusic(); 
																	}
																}
																break;
															case SDLK_s:
																if (gMuteSoundFlag == false){
																	gMuteSoundFlag = true;
																}
																else gMuteSoundFlag = false;
																break;
														}
														break;
													case SDL_KEYUP:
														switch (e.key.keysym.sym) {
															case SDLK_LEFT:
																gTeclaEsq = 0;
																break;
															case SDLK_RIGHT:
																gTeclaDir = 0;
																break;
														}
														break;
												}
											}
											
											/*Preenche a superficie de preto*/
											SDL_FillRect( gSurfaceJogo, NULL, 
														  SDL_MapRGB( gSurfaceJogo->format, 
														  0, 0, 0 ) );
														  
											geraBlocos(gBlocos);
											quadroDePontuacao();
											
											/*Move os NPCs*/
											moveBALL(&bola);
											movePADDLE(&plataforma);

											srcRect.x = 0; srcRect.y = 0;
											srcRect.w = BALL_WIDTH;
											srcRect.h = BALL_HEIGHT;
											dstRect.x = bola.posX;
											dstRect.y = bola.posY;
											
											if( SDL_BlitSurface( bola.image, &srcRect, 
																 gSurfaceJogo, &dstRect ) < 0 ) {
												printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
												quit2 = true;
											}
												
											srcRect.x = 0; srcRect.y = 0;
											srcRect.w = PADDLE_WIDTH;
											srcRect.h = PADDLE_HEIGHT;
											dstRect.x = plataforma.posX;
											dstRect.y = plataforma.posY;
												
											if( SDL_BlitSurface( plataforma.image, &srcRect,
																 gSurfaceJogo, &dstRect ) < 0 ) {
												printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
												quit2 = true;
											}
											
											/*Atualiza a superficie*/
											SDL_UpdateWindowSurface( gWindow );
												
											/*Adiciona um delay para desacelerar a execucao do programa*/
											if (FRAME_RATE > SDL_GetTicks() - gTimer) {
												SDL_Delay(FRAME_RATE - (SDL_GetTicks() - gTimer));
											}
											
											if (gQuitFlag) {
												quit2 = true;
												gerarTelaInicial();
											}
										}
									}
								}
								
								
								if((x>SCREEN_WIDTH-520) && (x<SCREEN_WIDTH-520+222) && (y>SCREEN_HEIGHT-335) && (y<SCREEN_HEIGHT-335+62)){

									if( gWindow == NULL ) {
										printf("A janela da tela inicial nao pode ser criada! SDL Error: %s\n", SDL_GetError());
										quit1 = true;
									}
									else {
										/*Troco a superfície inicial para a superfície de Jogo e ativo todos os componentes dessa nova superfície*/
										
										/*Captura a superficie da janela da tela inicial*/
										gSurfaceRecordes = SDL_GetWindowSurface(gWindow);
										SDL_FillRect (gSurfaceRecordes, NULL, SDL_MapRGB(gSurfaceRecordes ->format, 0xFF, 0xFF, 0XFF));
										
										leituraRecordes();
										
									}
								}
                            
								break;
							}
                    }
                }		
			
				/*Atualiza a superficie*/
				SDL_UpdateWindowSurface(gWindow);
				
				if (gPontos > 0) {
					checaRecordes();
					gPontos = 0;
				}
				
				/*Adiciona um delay para desacelerar a execucao do programa*/
				if (FRAME_RATE > SDL_GetTicks() - gTimer) {
					SDL_Delay(FRAME_RATE - (SDL_GetTicks() - gTimer));
				}
			}
		}
	}
	/*Libera a memória de tudo e encerra o SDL */
	closing();
		
	return 0;
}

