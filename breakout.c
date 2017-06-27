/*Usando as bibliotecas SDL, SDL_image, standard IO, e time*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <time.h>

#define NUM_LINHAS 4
#define NUM_COLUNAS 10
#define NUM_BLOCOS 40

#define FRAMES_PER_SECOND 60
#define FRAME_RATE 1000/FRAMES_PER_SECOND

/* Define o tipo bool */
typedef int bool;

/* Constantes */

/*Constantes de dimensoes da tela*/
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

/*Constantes Booleanas*/
const int true = 1;
const int false = 0;

/*Constantes de dimensoes de imagens*/
const int BALL_WIDTH = 22;
const int BALL_HEIGHT = 22;
const int PADDLE_WIDTH = 104;
const int PADDLE_HEIGHT = 24;
const int BLOCK_WIDTH = 80;
const int BLOCK_HEIGHT = 40;

/*Constantes de jogo*/
const int NUM_VIDAS = 5;
const int DIST_BLOCO_TELA = 80;
const int VELOCIDADE_JOGADOR = 7;
const int VELOCIDADE_INIC_BOLA = 5;
const int VELOCIDADE_MAX_BOLA = 6;
const int FATOR_VELOCIDADE = 5;

/*Estruturas*/

/*BOLA*/
typedef struct _BALL {
    int posX;
    int posY;
    int velX;
    int velY;
    SDL_Surface* image;
    int imgW;
    int imgH;
    int raio;
} BALL;

/*PLATAFORMA*/
typedef struct _PADDLE {
	int posX;
	int posY;
	int velX;
	SDL_Surface* image;
	int imgW;
	int imgH;
} PADDLE;

/*BLOCO*/
typedef struct _BLOCK {
	int posX;
	int posY;
	int vidas;
	SDL_Surface* image;
	int imgW;
	int imgH;
} BLOCK;

/*IMAGENS*/
typedef struct _IMAGEM{
	int posX;
	int posY;
	SDL_Surface* imagem;
} IMAGEM;

/* Variaveis globais */
int gTimer;
int gLevel;
int gPontos;
int gContaPontos;
int gVidas;
int gVelMinBola; /*sem uso ainda*/
int gVelMaxBola; /*sem uso ainda*/
int gTeclaDir;
int gTeclaEsq;
int gNumBlocos;
int gMuteSoundFlag;
int gQuitFlag;
BLOCK gBlocos[NUM_BLOCOS];

/*A janela em que estaremos renderizando*/
SDL_Window* gWindow = NULL;

/*Os personagens*/
BALL bola;
PADDLE plataforma;

/*As imagens*/
IMAGEM imagemJogar, imagemRecordes, imagemTexto;
  
/*As superficies do jogo*/
SDL_Surface* gSurfaceInicial = NULL;
SDL_Surface* gSurfaceJOGAR = NULL;
SDL_Surface* gSurfaceRECORDES = NULL;
SDL_Surface* gSurfaceJogo = NULL;
SDL_Surface* gSurfaceRecordes = NULL;
SDL_Surface* gTexto = NULL;


/*Imagens PNG atualmente exibidas*/
SDL_Surface* gBALLSurface = NULL;
SDL_Surface* gPADDLESurface = NULL;
SDL_Surface* gBLOCK1Surface = NULL;
SDL_Surface* gBLOCK2Surface = NULL;
SDL_Surface* gBLOCK3Surface = NULL;
SDL_Surface* gBLOCK4Surface = NULL;
SDL_Surface* gBLOCK5Surface = NULL;

/*Os efeitos sonoros que serao utilizados*/
Mix_Music *gMandela = NULL;
Mix_Music *gBaile = NULL;
Mix_Chunk *gEfeitoParedes = NULL;
Mix_Chunk *gEfeitoBlocos = NULL;

/*A fonte de texto a ser utilizada*/
TTF_Font *gFont = NULL;	

/*A cor da fonte de texto a ser utilizada*/
SDL_Color corPreta = {0,0,0};
SDL_Color corBranca = {0xFF, 0xFF, 0xFF};

/*
 * Prototipos de funcao
 */

/*Inicia o SDL e cria a janela*/
bool init();

/*Carrega a midia*/
bool loadMedia();

/*Carrega as musicas*/
bool loadMandela();
bool loadBaile();

/*Libera a midia e encerra o SDL*/
void closing();

/*Carrega uma imagem individual*/
SDL_Surface* loadSurface( char *path );

/*Carrega a superficie da fonte de texto*/
SDL_Surface* TTF_RenderText_Solid(TTF_Font *font, const char *text, SDL_Color fg);

/*Gera a tela inicial*/
bool gerarTelaInicial();

/*Cria a imagem na tela com uma posição definida*/
IMAGEM criarImagem(int posX, int posY, SDL_Surface* imagem);

/*Cria os blocos*/
void criaBlocos(BLOCK *gBlocos);

/*Gera os blocos*/
void geraBlocos(BLOCK *gBlocos);

/*Gera o quadro de pontuacao*/
bool quadroDePontuacao();

/*Cria o personagem*/
BALL criaBOLA( int posX, int posY, int velX, int velY, SDL_Surface *image);
PADDLE criaPLATAFORMA( int posX, int posY, int velX, SDL_Surface *image);

/*Move o personagem*/
void moveBALL(BALL *p); /*Move a bola e toca os efeitos sonoros apropriados*/
void movePADDLE(PADDLE *p); /*Move a plataforma*/

/*Checa colisao entre a bola e a plataforma*/
bool colisaoBolaPlat( BALL a, PADDLE b );

/*Colisao entre bola e bloco*/
bool colisaoBolaBlocoCima(BALL a, BLOCK *gBlocos);
bool colisaoBolaBlocoBaixo(BALL a, BLOCK *gBlocos);
bool colisaoBolaBlocoEsq(BALL a, BLOCK *gBlocos);
bool colisaoBolaBlocoDir(BALL a, BLOCK *gBlocos);
bool PontoNoBloco(int x, int y, int w, int z);

/*Deleta blocos atingidos*/
void DeletaBloco(BLOCK *gBlocos, int index);

/*Muda o level*/
void MudaLevel(BALL *a, PADDLE *b);

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
										quit1 = false; /*Deveria ser true?*/
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
																	(SCREEN_HEIGHT - 2*PADDLE_HEIGHT - 10 ),
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
										quit1 = false; /*Deveria ser true?*/
									}
									else {
										/*Troco a superfície inicial para a superfície de Jogo e ativo todos os componentes dessa nova superfície*/
										
										/*Captura a superficie da janela da tela inicial*/
										gSurfaceRecordes = SDL_GetWindowSurface(gWindow);
										SDL_FillRect (gSurfaceRecordes, NULL, SDL_MapRGB(gSurfaceRecordes ->format, 0xFF, 0xFF, 0XFF));
										
										/*leituraRecordes();*/
										
									}
								}
                            
								break;
							}
                    }
                }		
			
				/*Atualiza a superficie*/
				SDL_UpdateWindowSurface(gWindow);
				
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

/*Move a bola e toca os efeitos sonoros apropriados*/
void moveBALL(BALL *p) {
	
    p->posX += p->velX;
    p->posY += p->velY;
    
    if ( (p->posX + BALL_WIDTH > SCREEN_WIDTH) || (p->posX < 0) ) {
        p->velX = -p->velX;
        p->posX += p->velX;
        if ( !gMuteSoundFlag ) Mix_PlayChannel( -1, gEfeitoParedes, 0 );
    }
    if ( p->posY < 0 ){
        p->velY = -p->velY;
        p->posY += p->velY;
        if ( !gMuteSoundFlag ) Mix_PlayChannel( -1, gEfeitoParedes, 0 );
    }
    
    if ( p->posY + BALL_HEIGHT > SCREEN_HEIGHT ) {
		p->velX = 0;
		p->velY = 0;
		p->posX = SCREEN_WIDTH/2 - BALL_WIDTH/2;
		p->posY = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
		if ( !gMuteSoundFlag ) Mix_PlayChannel( -1, gEfeitoParedes, 0 );
		gVidas--;
		if (gVidas == 0) {
			/*Game Over*/
			gQuitFlag = true;
		}
	}
		
	if (colisaoBolaPlat(bola, plataforma)) {
		
		int centroPlataforma = plataforma.posX + PADDLE_WIDTH/2;
		int centroBola = p->posX + BALL_WIDTH/2;
		int localPlataforma = centroBola - centroPlataforma;
		
		p->velX = localPlataforma/FATOR_VELOCIDADE;
		
		if ( p->velX > 0 && p->velX > VELOCIDADE_MAX_BOLA ) p->velX = VELOCIDADE_MAX_BOLA;
		if ( p->velX < 0 && -(p->velX) > VELOCIDADE_MAX_BOLA ) p->velX = -(VELOCIDADE_MAX_BOLA);
		
		p->velY = -p->velY;
		p->posX += p->velX;
		p->posY += p->velY;
	}
	
	if (colisaoBolaBlocoCima(bola, gBlocos)) {
		p->velY = -p->velY;
		if ( !gMuteSoundFlag ) Mix_PlayChannel( -1, gEfeitoBlocos, 0 );
	}
		
	if (colisaoBolaBlocoBaixo(bola, gBlocos)) {
		p->velY = -p->velY;
		if ( !gMuteSoundFlag ) Mix_PlayChannel( -1, gEfeitoBlocos, 0 );
	}
	
	if (colisaoBolaBlocoEsq(bola, gBlocos)) {
		p->velX = -p->velX;
		if ( !gMuteSoundFlag ) Mix_PlayChannel( -1, gEfeitoBlocos, 0 );
	}
	if (colisaoBolaBlocoDir(bola, gBlocos)) {
		p->velX = -p->velX;
		if ( !gMuteSoundFlag ) Mix_PlayChannel( -1, gEfeitoBlocos, 0 );
	}
	if (gNumBlocos == 0) MudaLevel(&bola, &plataforma);	
}			

void movePADDLE(PADDLE *p) {
	
	if (!(p->posX + PADDLE_WIDTH > SCREEN_WIDTH)) {
		if (gTeclaDir) p->posX += p->velX;
	}
	if (!(p->posX < 0)) {
		if (gTeclaEsq) p->posX -= p->velX;
	}
}

bool colisaoBolaPlat( BALL a, PADDLE b ) {
	
	if ( (a.velY > 0) && (a.posY + BALL_HEIGHT >= b.posY) &&
		(a.posY + BALL_HEIGHT <= b.posY + PADDLE_HEIGHT) ) { /* acerto lateral */
			
			/* Se a bola estiver no alcance X da plataforma, retorne true. */
			if ( (a.posX <= b.posX + PADDLE_WIDTH) && (a.posX + BALL_WIDTH >= b.posX) ) {
				return true;
			}
	}
	return false;
}

/*Cria NPCs*/
BALL criaBOLA( int posX, int posY, int velX, int velY, SDL_Surface *image ) {
	
    BALL p;
    
    p.posX = posX;
    p.posY = posY;
    p.velX = velX;
    p.velY = velY;
    p.raio = BALL_WIDTH/2;
    p.image = image;
    return p;
}

PADDLE criaPLATAFORMA( int posX, int posY, int velX, SDL_Surface *image ) {
	
    PADDLE p;
    
    p.posX = posX;
    p.posY = posY;
    p.velX = velX;
    p.image = image;
    return p;
}

/*Cria imagens*/
IMAGEM criarImagem( int posX, int posY, SDL_Surface* imagem ) {
	
	IMAGEM img;
	
	img.posX = posX;
	img.posY = posY;
	img.imagem = imagem;
	return img;
}

void criaBlocos(BLOCK *gBlocos) {
	
	int lin, col;
	int i = 0;
	
	for (lin = 1; lin <= NUM_LINHAS; lin++) {
		for (col = 1; col <= NUM_COLUNAS; col++) {
			
			if (gLevel == 1) gBlocos[i].vidas = gLevel;
			if (i%2 == 0 && gLevel > 1 && gLevel < 6) gBlocos[i].vidas = gLevel-1;
			if (i%2 == 1 && gLevel > 1 && gLevel < 6) gBlocos[i].vidas = gLevel;
			if (gLevel == 6) gBlocos[i].vidas = gLevel-1;
			gBlocos[i].posX = col*BLOCK_WIDTH - DIST_BLOCO_TELA;
			gBlocos[i].posY = lin*BLOCK_HEIGHT - DIST_BLOCO_TELA/2;
			gBlocos[i].imgW = BLOCK_WIDTH;
			gBlocos[i].imgH = BLOCK_HEIGHT;
			i++;
			gNumBlocos++;
		}
	}	
}

void geraBlocos(BLOCK *gBlocos) {
	
	SDL_Rect srcRectBlock, dstRectBlock;
	int i;
	
	for (i = 0; i < gNumBlocos; i++) {
			
		srcRectBlock.x = 0; srcRectBlock.y = 0;
		srcRectBlock.w = gBlocos[i].imgW;
		srcRectBlock.h = gBlocos[i].imgH;
		dstRectBlock.x = gBlocos[i].posX;
		dstRectBlock.y = gBlocos[i].posY;
		
		switch (gBlocos[i].vidas) {
			case 1:
				gBlocos[i].image = gBLOCK1Surface;
				break;
			case 2:
				gBlocos[i].image = gBLOCK2Surface;
				break;
			case 3:
				gBlocos[i].image = gBLOCK3Surface;
				break;
			case 4:
				gBlocos[i].image = gBLOCK4Surface;
				break;
			case 5:
				gBlocos[i].image = gBLOCK5Surface;
				break;
		}
				
		if( SDL_BlitSurface( gBlocos[i].image, &srcRectBlock, 
								gSurfaceJogo, &dstRectBlock ) < 0 ) {
					printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
		}
	}
}

bool colisaoBolaBlocoCima(BALL a, BLOCK *gBlocos) {
	
	int bloco, cima;
	int cima_x = a.posX + BALL_WIDTH/2;
	int cima_y = a.posY;
	cima = false;
	
	for (bloco = 0; bloco < gNumBlocos; bloco++) {
		if ( PontoNoBloco(cima_x, cima_y, gBlocos[bloco].posX, gBlocos[bloco].posY ) ) {	
			cima = true;
			DeletaBloco(gBlocos, bloco);
		}
	}
	return cima;
}

bool colisaoBolaBlocoBaixo(BALL a, BLOCK *gBlocos) {
	
	int bloco, baixo;
	int baixo_x = a.posX + BALL_WIDTH/2;
	int baixo_y = a.posY + BALL_HEIGHT;
	baixo = false;
	
	for (bloco = 0; bloco < gNumBlocos; bloco++) {
		if ( PontoNoBloco(baixo_x, baixo_y, gBlocos[bloco].posX, gBlocos[bloco].posY ) ) {
			baixo = true;
			DeletaBloco(gBlocos, bloco);
		}
	}
	return baixo;
}

bool colisaoBolaBlocoEsq(BALL a, BLOCK *gBlocos) {
	
	int bloco, esq;
	int esq_x = a.posX;
	int esq_y = a.posY + BALL_HEIGHT/2;
	esq = false;
	
	for (bloco = 0; bloco < gNumBlocos; bloco++) {
		if ( PontoNoBloco(esq_x, esq_y, gBlocos[bloco].posX, gBlocos[bloco].posY ) ) {
			esq = true;
			DeletaBloco(gBlocos, bloco);
		}
	}
	return esq;
}

bool colisaoBolaBlocoDir(BALL a, BLOCK *gBlocos) {
	
	int bloco, dir;
	int dir_x = a.posX + BALL_WIDTH;
	int dir_y = a.posY + BALL_HEIGHT/2;
	dir = false;
	
	for (bloco = 0; bloco < gNumBlocos; bloco++) {
		if ( PontoNoBloco(dir_x, dir_y, gBlocos[bloco].posX, gBlocos[bloco].posY ) ) {
			dir = true;
			DeletaBloco(gBlocos, bloco);
		}
	}
	return dir;
}

bool PontoNoBloco(int x, int y, int w, int z) {
	
	if ( (x >= w) && (x <= w + BLOCK_WIDTH) && 
	   (y >= z) && (y <= z + BLOCK_HEIGHT) ) {
		return true;
	}
	return false;
}

void DeletaBloco(BLOCK *gBlocos, int index) {
	
	gBlocos[index].vidas--;
	
	if (gBlocos[index].vidas == 0) {
		gBlocos[index] = gBlocos[gNumBlocos-1];
		gNumBlocos--;
		gPontos += 100;
		gContaPontos += 100;
		if (gContaPontos == 10000) {
			gContaPontos = 0;
			gVidas++;
		}
	}
}

void MudaLevel(BALL *a, PADDLE *b) {
	
	gLevel++;
	a->velX = 0;
	a->velY = 0;
	a->posX = SCREEN_WIDTH/2 - BALL_WIDTH/2;
	a->posY = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
	b->posX = SCREEN_WIDTH/2 - PADDLE_WIDTH/2;
	b->posY = SCREEN_HEIGHT - 2*PADDLE_HEIGHT - 10;
	gPontos += 1000;
	gContaPontos += 1000;
	if (gContaPontos == 10000) {
		gContaPontos = 0;
		gVidas++;
	}
	gNumBlocos = 0;
	criaBlocos(gBlocos);
}

bool init() {
    /*Flag de inicializacao*/
    int success = true;
    atexit(SDL_Quit);
    srand(time(NULL));

    /*Inicializa o SDL(video, audio e timer)*/
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) < 0 ) {
        printf( "Nao foi possivel inicializar SDL! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    if(TTF_Init() < 0) {
		printf("Nao foi possivel inicializar TTF! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	}
    else {
        /*Cria a janela*/
        gWindow = SDL_CreateWindow( "Breakout Funk - The Ultimate Version", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Nao foi possivel criar a janela! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else {
            /*Inicializa o carregamento JPEG e PNG*/
            int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                printf( "Nao foi possivel inicializar SDL_image! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            /*Inicializa o SDL_mixer*/
            if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY , MIX_DEFAULT_FORMAT, 2, 1024 ) < 0 ) {
				printf( "Nao foi possivel inicializar SDL_mixer! SDL_mixer Error: %s\n", Mix_GetError() );
				success = false;
			}		
            else {
				/*Aloca canais de audio*/
				Mix_AllocateChannels( 16 );
                /*Recebe a superficie da janela*/
                gSurfaceInicial = SDL_GetWindowSurface( gWindow );
                
            }
        }
    }
	return success;
}

bool loadMedia() {
    /*Flag de sucesso de carregamento*/
    int success = true;
    	
    /*Carrega as superficies PNG*/
    gSurfaceJOGAR = loadSurface("./JOGAR.png");
    SDL_SetColorKey(gSurfaceJOGAR, SDL_TRUE, SDL_MapRGB(gSurfaceJOGAR->format, 0xFF, 0xFF, 0XFF));
    gSurfaceRECORDES = loadSurface("./RECORDES.png");
    SDL_SetColorKey(gSurfaceRECORDES, SDL_TRUE, SDL_MapRGB(gSurfaceRECORDES->format, 0xFF, 0xFF, 0XFF));
    gBALLSurface = loadSurface( "./ballBlue.png" );
    gPADDLESurface = loadSurface( "./paddleRed.png" );
    gBLOCK1Surface = loadSurface( "./element_red_rectangle.png" );
    gBLOCK2Surface = loadSurface( "./element_yellow_rectangle.png" );
    gBLOCK3Surface = loadSurface( "./element_blue_rectangle.png" );
    gBLOCK4Surface = loadSurface( "./element_purple_rectangle.png" );
    gBLOCK5Surface = loadSurface( "./element_green_rectangle.png" );
	
    if( gSurfaceJOGAR == NULL || gSurfaceRECORDES == NULL || gBALLSurface == NULL ||
		gPADDLESurface == NULL || gBLOCK1Surface == NULL ||
		gBLOCK2Surface == NULL || gBLOCK3Surface == NULL ||
		gBLOCK4Surface == NULL || gBLOCK5Surface == NULL ) {
        printf( "Falha ao carregar imagem! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    
    /*Carrega os efeitos sonoros WAV*/
    
    gEfeitoParedes = Mix_LoadWAV( "tutu.wav" );
	if (!gEfeitoParedes) {
		printf("Wav: SDL error=%s\n", SDL_GetError());
		success = false;
	}
				
	gEfeitoBlocos = Mix_LoadWAV( "ta.wav" );
	if (!gEfeitoBlocos) {
		printf("Wav: SDL error=%s\n", SDL_GetError());
		success = false;
	}
	
	gFont = TTF_OpenFont("ubuntu.medium.ttf", 24);
	if(!gFont) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		success = false;
	}
	
    return success;
}

bool loadMandela() {
	/*Variavel de controle do carregamento de áudios e músicas*/
    int success = true;
	
	/*Carrega a música do Mandela (tela do jogo)*/
    gMandela = Mix_LoadMUS( "mandela8bit.wav" ); 
    if(!gMandela) { 
		printf( "Falha ao carregar a música de 'MANDELA'! SDL_mixer Error: %s\n", Mix_GetError() ); 
		success = false; 
	}
	
	Mix_FreeMusic( gBaile ); 
	gBaile = NULL;
	
	return success;
}

bool loadBaile(){	
	/*Variavel de controle do carregamento de áudios e músicas*/
    int success = true;
	
	/*Carrega a música do Baile de Favela (tela do jogo)*/
    gBaile = Mix_LoadMUS( "bailedefavela8bit.wav" ); 
    if(!gBaile) { 
		printf( "Falha ao carregar a música de 'BAILE DE FAVELA'! SDL_mixer Error: %s\n", Mix_GetError() ); 
		success = false; 
	}
	
	Mix_FreeMusic( gMandela ); 
	gMandela = NULL;
	
	return success;
}

void closing() {
    /*Libera a memoria das imagens carregadas*/
    SDL_FreeSurface( gBALLSurface );
    gBALLSurface = NULL;
    SDL_FreeSurface ( gPADDLESurface );
    gPADDLESurface = NULL;
    SDL_FreeSurface ( gBLOCK1Surface );
    gBLOCK1Surface = NULL;
    SDL_FreeSurface ( gBLOCK2Surface );
    gBLOCK2Surface = NULL;
    SDL_FreeSurface ( gBLOCK3Surface );
    gBLOCK3Surface = NULL;
    SDL_FreeSurface ( gBLOCK4Surface );
    gBLOCK4Surface = NULL;
    SDL_FreeSurface ( gBLOCK5Surface );
    gBLOCK5Surface = NULL;
    SDL_FreeSurface( gSurfaceJOGAR );
    gSurfaceJOGAR = NULL;
    SDL_FreeSurface( gSurfaceRECORDES );
    gSurfaceRECORDES = NULL;
    SDL_FreeSurface(gTexto);
	gTexto = NULL;

    /*Libera a memoria das musicas e dos efeitos sonoros*/
    Mix_FreeMusic( gMandela );
    gMandela = NULL;
    Mix_FreeMusic( gBaile );
    gBaile = NULL;
    Mix_FreeChunk( gEfeitoParedes );
    gEfeitoParedes = NULL;
    Mix_FreeChunk( gEfeitoBlocos );
    gEfeitoBlocos = NULL;
    
    /*Libera a memoria das fontes carregadas*/
    TTF_CloseFont(gFont); 
    gFont = NULL;

    /*Destroi a janela*/
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    /*Encerra os subsistemas do SDL*/
    IMG_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}

SDL_Surface* loadSurface( char *path ) {
    /*A imagem final otimizada*/
    SDL_Surface* optimizedSurface = NULL;

    /*Carrega imagem no caminho especificado*/
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL ) {
        printf( "Nao foi possivel carregar a imagem %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else {
		
		/*Color keying para a cor preta*/
		/*SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0, 0 ) );*/
		
        /*Converte superficie para o formato da tela*/
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gSurfaceInicial->format, 0 );
        if( optimizedSurface == NULL ) {
            printf( "Nao foi possivel otimizar a imagem %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        /*Se desfaz da superficie carregada anteriormente*/
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

bool gerarTelaInicial() {
	
	SDL_Rect dstRect;
	
	/*Variavel de controle do carregamento de fontes de texto*/
	int success = true;
		
	imagemJogar = criarImagem((SCREEN_WIDTH-520), (SCREEN_HEIGHT-435), gSurfaceJOGAR);
	imagemRecordes = criarImagem((SCREEN_WIDTH-520), (SCREEN_HEIGHT-335), gSurfaceRECORDES);

	loadBaile();
	Mix_PlayMusic( gBaile, -1 );
				
	/*Pinta a superfície de branco*/
	SDL_FillRect (gSurfaceInicial, NULL, SDL_MapRGB(gSurfaceInicial->format, 0xFF, 0xFF, 0XFF));
			
	dstRect.x = imagemJogar.posX;
	dstRect.y = imagemJogar.posY;
                
	if(SDL_BlitSurface( imagemJogar.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
		printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}	
                
	dstRect.x = imagemRecordes.posX;
	dstRect.y = imagemRecordes.posY;
                
	if(SDL_BlitSurface( imagemRecordes.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
		printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
                                
	gTexto = TTF_RenderText_Solid(gFont, "BREAKOUT FUNK", corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-507), (SCREEN_HEIGHT-500), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
				
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
				
	gTexto = TTF_RenderText_Solid(gFont, "Comandos quando em jogo:", corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-670), (SCREEN_HEIGHT-215), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
				
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
				
	gTexto = TTF_RenderText_Solid(gFont, "Aperte S para ligar ou desligar os sons do jogo.", corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-620), (SCREEN_HEIGHT-175), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
				
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
				
	gTexto = TTF_RenderText_Solid(gFont, "Aperte M para ligar ou desligar a musica.", corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-620), (SCREEN_HEIGHT-135), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
				
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
				
	gTexto = TTF_RenderText_Solid(gFont, "Aperte ESC para voltar para a tela inicial.", corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-620), (SCREEN_HEIGHT-95), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
				
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
				
	gTexto = TTF_RenderText_Solid(gFont, "Grupo: MC's Elizeu Cappelli, Luiz Fernando Porto e Rodrigo Carvalho", corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-790), (SCREEN_HEIGHT-35), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
				
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
	
	return success;
}

bool quadroDePontuacao() {
	
	int success = true;
	SDL_Rect dstRect;
	
	char level[10], vidas[10], pontos[10], blocos[10];
	
	sprintf(level, "%d", gLevel);
	sprintf(vidas, "%d", gVidas);
	sprintf(pontos, "%d", gPontos);
	sprintf(blocos, "%d", gNumBlocos);
	
	gTexto = TTF_RenderText_Solid(gFont, "|| Level : ", corBranca);
	imagemTexto = criarImagem((SCREEN_WIDTH-790), (SCREEN_HEIGHT-30), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
	
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
	
	SDL_FreeSurface(gTexto);
	gTexto = NULL;
	
	gTexto = TTF_RenderText_Solid(gFont, level, corBranca);
	imagemTexto = criarImagem((SCREEN_WIDTH-690), (SCREEN_HEIGHT-30), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
	
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
	
	SDL_FreeSurface(gTexto);
	gTexto = NULL;
	
	gTexto = TTF_RenderText_Solid(gFont, "||   Vidas : ", corBranca);
	imagemTexto = criarImagem((SCREEN_WIDTH-670), (SCREEN_HEIGHT-30), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
	
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
	
	SDL_FreeSurface(gTexto);
	gTexto = NULL;
	
	gTexto = TTF_RenderText_Solid(gFont, vidas, corBranca);
	imagemTexto = criarImagem((SCREEN_WIDTH-550), (SCREEN_HEIGHT-30), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
	
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
	
	SDL_FreeSurface(gTexto);
	gTexto = NULL;
	
	gTexto = TTF_RenderText_Solid(gFont, "||   Pontos : ", corBranca);
	imagemTexto = criarImagem((SCREEN_WIDTH-510), (SCREEN_HEIGHT-30), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
	
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
	
	SDL_FreeSurface(gTexto);
	gTexto = NULL;
	
	gTexto = TTF_RenderText_Solid(gFont, pontos, corBranca);
	imagemTexto = criarImagem((SCREEN_WIDTH-380), (SCREEN_HEIGHT-30), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
	
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
	
	SDL_FreeSurface(gTexto);
	gTexto = NULL;
	
	gTexto = TTF_RenderText_Solid(gFont, "||  Blocos restantes :         ||", corBranca);
	imagemTexto = criarImagem((SCREEN_WIDTH-300), (SCREEN_HEIGHT-30), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
	
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
	
	SDL_FreeSurface(gTexto);
	gTexto = NULL;
	
	gTexto = TTF_RenderText_Solid(gFont, blocos, corBranca);
	imagemTexto = criarImagem((SCREEN_WIDTH-60), (SCREEN_HEIGHT-30), gTexto);
				
	dstRect.x = imagemTexto.posX;
	dstRect.y = imagemTexto.posY;
	
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
	else {
		if(SDL_BlitSurface( imagemTexto.imagem, NULL, gSurfaceInicial, &dstRect ) < 0 ) {
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
	
	SDL_FreeSurface(gTexto);
	gTexto = NULL;
	
	return success;
}
