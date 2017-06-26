/*Usando as bibliotecas SDL, SDL_image, standard IO, e time*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

/* Variaveis globais */
int gTimer;
int gLevel = 6;
int gPontos = 0;
int gContaPontos = 0;
int gVidas;
int gVelMinBola; /*sem uso ainda*/
int gVelMaxBola; /*sem uso ainda*/
int gTeclaDir;
int gTeclaEsq;
int gNumBlocos = 0;

BLOCK gBlocos[NUM_BLOCOS];

/*A janela em que estaremos renderizando*/
SDL_Window* gWindow = NULL;

/*Os personagens*/
BALL bola;
PADDLE plataforma;
  
/*A superficie contida pela janela*/
SDL_Surface* gScreenSurface = NULL;

/*Imagem JPEG atualmente exibida*/
SDL_Surface* gBALLSurface = NULL;
SDL_Surface* gPADDLESurface = NULL;
SDL_Surface* gBLOCK1Surface = NULL;
SDL_Surface* gBLOCK2Surface = NULL;
SDL_Surface* gBLOCK3Surface = NULL;
SDL_Surface* gBLOCK4Surface = NULL;
SDL_Surface* gBLOCK5Surface = NULL;

/*
 * Prototipos de funcao
 */

/*Inicia o SDL e cria a janela*/
int init();

/*Carrega a midia*/
int loadMedia();

/*Libera a midia e encerra o SDL*/
void closing();

/*Carrega uma imagem individual*/
SDL_Surface* loadSurface( char *path );

/*Cria os blocos*/
void criaBlocos(BLOCK *gBlocos);

/*Gera os blocos*/
void geraBlocos(BLOCK *gBlocos);

/*Cria NPC*/
BALL criaBOLA( int posX, int posY, int velX, int velY, SDL_Surface *image);
PADDLE criaPLATAFORMA( int posX, int posY, int velX, SDL_Surface *image);

/*Move o personagem*/
void moveBALL(BALL *p); /*Move a bola*/
void movePADDLE(PADDLE *p); /*Move a plataforma*/

/*Checa colisao entre a bola e a plataforma*/
bool colisaoBolaPlat( BALL a, PADDLE b );

/*Colisao entre bola e bloco*/
bool colisaoBolaBlocoCima(BALL a, BLOCK *gBlocos);
bool colisaoBolaBlocoBaixo(BALL a, BLOCK *gBlocos);
bool colisaoBolaBlocoEsq(BALL a, BLOCK *gBlocos);
bool colisaoBolaBlocoDir(BALL a, BLOCK *gBlocos);
bool PontoNoBloco(int x, int y, int w, int z);
void DeletaBloco(BLOCK *gBlocos, int index);

/*Muda o level*/
void MudaLevel(BALL *a, PADDLE *b);

int main( int argc, char* args[] ) {
	
    SDL_Rect srcRect, dstRect;
    int quit;
    /*Manipulador de Eventos*/
    SDL_Event e;
    
    /*Inicia o SDL e cria a janela*/
    if( !init() ) {
        printf( "Falha ao inicializar!\n" );
    }
    else {
        /*Carrega a midia*/
        if( !loadMedia() ) {
            printf( "Falha ao carregar midia!\n" );
        }
        else {
			
            /*Cria os NPCs*/
            bola = criaBOLA(((SCREEN_WIDTH/2) - (BALL_WIDTH/2)),
                             ((SCREEN_WIDTH/2) - (BALL_HEIGHT/2)), 
                             0, 
                             0,
                             gBALLSurface);
           
            
            plataforma = criaPLATAFORMA(((SCREEN_WIDTH/2) - (PADDLE_WIDTH/2)),
									(SCREEN_HEIGHT - 2*PADDLE_HEIGHT ),
									VELOCIDADE_JOGADOR,
									gPADDLESurface);
			
			gVidas = NUM_VIDAS;
			
			criaBlocos(gBlocos);
             
            /*Flag do loop principal*/
            quit = false;

            /*Enquanto estiver em execucao*/           
            while( !quit ) {
				gTimer = SDL_GetTicks();
				while( SDL_PollEvent( &e ) != 0 ) {
					switch (e.type) {
						case SDL_QUIT:
							quit = true;
							break;
						case SDL_KEYDOWN:
							switch (e.key.keysym.sym) {
								case SDLK_ESCAPE:
									quit = true;
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
				SDL_FillRect( gScreenSurface, NULL, 
							  SDL_MapRGB( gScreenSurface->format, 
							  0, 0, 0 ) );
							  
				geraBlocos(gBlocos);
				
				/*Move os NPCs*/
				moveBALL(&bola);
				movePADDLE(&plataforma);

				srcRect.x = 0; srcRect.y = 0;
				srcRect.w = BALL_WIDTH;
				srcRect.h = BALL_HEIGHT;
				dstRect.x = bola.posX;
				dstRect.y = bola.posY;
				
				if( SDL_BlitSurface( bola.image, &srcRect, 
									 gScreenSurface, &dstRect ) < 0 ) {
					printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
					quit = true;
				}
					
				srcRect.x = 0; srcRect.y = 0;
				srcRect.w = PADDLE_WIDTH;
				srcRect.h = PADDLE_HEIGHT;
				dstRect.x = plataforma.posX;
				dstRect.y = plataforma.posY;
					
				if( SDL_BlitSurface( plataforma.image, &srcRect,
									 gScreenSurface, &dstRect ) < 0 ) {
					printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
					quit = true;
				}
				
				geraBlocos(gBlocos);
				
				/*Atualiza a superficie*/
				SDL_UpdateWindowSurface( gWindow );
					
				/*Adiciona um delay para desacelerar a execucao do programa*/
				if (FRAME_RATE > SDL_GetTicks() - gTimer) {
					SDL_Delay(FRAME_RATE - (SDL_GetTicks() - gTimer));
				}
			}
        }
    }

    /*Libera recursos e fecha o SDL*/
    closing();

    return 0;
}

void moveBALL(BALL *p) {
	
    p->posX += p->velX;
    p->posY += p->velY;
    
    if ( (p->posX + BALL_WIDTH > SCREEN_WIDTH) || (p->posX < 0) ) {
        p->velX = -p->velX;
        p->posX += p->velX;
    }
    if ( p->posY < 0 ){
        p->velY = -p->velY;
        p->posY += p->velY;
    }
    
    if ( p->posY + BALL_HEIGHT > SCREEN_HEIGHT ) {
		p->velX = 0;
		p->velY = 0;
		p->posX = SCREEN_WIDTH/2 - BALL_WIDTH/2;
		p->posY = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
		gVidas--;
		printf("Morreu! -1 vida\n");
		printf("Vidas = %d\n", gVidas); /*print de teste*/
		/*if (gVidas == 0) Game over!*/
	}
		
	if (colisaoBolaPlat(bola, plataforma)) {
		
		int centroPlataforma = plataforma.posX + PADDLE_WIDTH/2;
		int centroBola = p->posX + BALL_WIDTH/2;
		int localPlataforma = centroBola - centroPlataforma;
		
		p->velX = localPlataforma/FATOR_VELOCIDADE;
		
		if ( p->velX > 0 && p->velX > VELOCIDADE_MAX_BOLA ) p->velX = VELOCIDADE_MAX_BOLA;
		if ( p->velX < 0 && -(p->velX) > VELOCIDADE_MAX_BOLA ) p->velX = -(VELOCIDADE_MAX_BOLA);
		/*printf("velX = %d\n", p->velX);*/ /*print de teste*/
		
		p->velY = -p->velY;
		p->posX += p->velX;
		p->posY += p->velY;
	}
	
	if (colisaoBolaBlocoCima(bola, gBlocos)) {
		p->velY = -p->velY;
		/*p->posY += BALL_WIDTH;*/
	}
		
	if (colisaoBolaBlocoBaixo(bola, gBlocos)) {
		p->velY = -p->velY;
		/*p->posY -= BALL_WIDTH;*/
	}
	
	if (colisaoBolaBlocoEsq(bola, gBlocos)) {
		p->velX = -p->velX;
		/*p->posX += BALL_WIDTH;*/
	}
	if (colisaoBolaBlocoDir(bola, gBlocos)) {
		p->velX = -p->velX;
		/*p->posX -= BALL_WIDTH;*/
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

void criaBlocos(BLOCK *gBlocos) {
	
	int lin, col;
	int i = 0;
	
	for (lin = 1; lin <= NUM_LINHAS; lin++) {
		for (col = 1; col <= NUM_COLUNAS; col++) {
			
			if (gLevel == 1) gBlocos[i].vidas = gLevel;
			if (i%2 == 0 && gLevel > 1 && gLevel < 6) gBlocos[i].vidas = gLevel-1;
			if (i%2 == 1 && gLevel > 1 && gLevel < 6) gBlocos[i].vidas = gLevel;
			if (gLevel >= 6) gBlocos[i].vidas = gLevel-1;
			gBlocos[i].posX = col*BLOCK_WIDTH - DIST_BLOCO_TELA;
			gBlocos[i].posY = lin*BLOCK_HEIGHT - DIST_BLOCO_TELA/2;
			gBlocos[i].imgW = BLOCK_WIDTH;
			gBlocos[i].imgH = BLOCK_HEIGHT;
			i++;
			gNumBlocos++;
		}
	}
	printf("Level: %d\n", gLevel);
	if (gLevel == 1) printf("Vidas: %d\n", gVidas);
	printf("Pontos: %d\n", gPontos);
	
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
								gScreenSurface, &dstRectBlock ) < 0 ) {
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
			printf("%d pontos! +1 vida\n", gPontos);
			printf("Vidas: %d\n", gVidas);
		}
		printf("Pontos: %d\n", gPontos);
	}
}

void MudaLevel(BALL *a, PADDLE *b) {
	
	gLevel++;
	a->velX = 0;
	a->velY = 0;
	a->posX = SCREEN_WIDTH/2 - BALL_WIDTH/2;
	a->posY = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
	b->posX = SCREEN_WIDTH/2 - PADDLE_WIDTH/2;
	b->posY = SCREEN_HEIGHT - 2*PADDLE_HEIGHT;
	gPontos += 1000;
	printf("Passou de level! +1000 pontos\n");
	gContaPontos += 1000;
	if (gContaPontos == 10000) {
		gContaPontos = 0;
		gVidas++;
		printf("%d pontos! +1 Vida\n", gPontos);
		printf("Vidas: %d\n", gVidas);
	}
	gNumBlocos = 0;
	criaBlocos(gBlocos);
}

int init() {
    /*Flag de inicializacao*/
    int success = true;
    srand(time(NULL));

    /*Inicializa o SDL(video e timer)*/
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ) {
        printf( "Nao foi possivel inicializar SDL! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else {
        /*Cria a janela*/
        gWindow = SDL_CreateWindow( "Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Nao foi possivel criar a janela! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            /*Inicializa o carregamento da imagem*/
            int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                printf( "Nao foi possivel inicializar SDL_image! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else {
                /*Recebe a superficie da janela*/
                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
        }
    }

    return success;
}

int loadMedia() {
    /*Flag de sucesso de carregamento*/
    int success = true;
    	
    /*Carrega a superficie JPEG*/
    gBALLSurface = loadSurface( "./ballBlue.png" );
    gPADDLESurface = loadSurface( "./paddleRed.png" );
    gBLOCK1Surface = loadSurface( "./element_red_rectangle.png" );
    gBLOCK2Surface = loadSurface( "./element_yellow_rectangle.png" );
    gBLOCK3Surface = loadSurface( "./element_blue_rectangle.png" );
    gBLOCK4Surface = loadSurface( "./element_purple_rectangle.png" );
    gBLOCK5Surface = loadSurface( "./element_green_rectangle.png" );
	
    if( gBALLSurface == NULL || gPADDLESurface == NULL ||
        gBLOCK1Surface == NULL || gBLOCK2Surface == NULL ||
        gBLOCK3Surface == NULL || gBLOCK4Surface == NULL ||
        gBLOCK5Surface == NULL ) {
        printf( "Falha ao carregar imagem! SDL Error: %s\n", SDL_GetError() );
        success = false;
    } 
    return success;
}

void closing() {
    /*Libera as imagens carregadas*/
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

    /*Destroi a janela*/
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    /*Encerra os subsistemas do SDL*/
    IMG_Quit();
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
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0, 0 ) );
		
        /*Converte superficie para o formato da tela*/
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimizedSurface == NULL ) {
            printf( "Nao foi possivel otimizar a imagem %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        /*Se desfaz da superficie carregada anteriormente*/
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}
