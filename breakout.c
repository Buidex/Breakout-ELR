/*Usando as bibliotecas SDL, SDL_image, standard IO, e time*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>

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

/*Constantes de jogo*/
const int NUM_VIDAS = 5;
const int VELOCIDADE_JOGADOR = 4;
const int VELOCIDADE_INIC_BOLA = 2;
const int VELOCIDADE_MAX_BOLA = 4;
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

/* Variaveis globais */

int gVidas;
int gVelMinBola; /*sem uso ainda*/
int gVelMaxBola; /*sem uso ainda*/
int gTeclaDir;
int gTeclaEsq; 

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

/*Cria NPC*/
BALL criaBOLA( int posX, int posY, int velX, int velY, SDL_Surface *image);
PADDLE criaPLATAFORMA( int posX, int posY, int velX, SDL_Surface *image);

/*Move o personagem*/
void moveBALL(BALL *p); /*Move a bola*/
void movePADDLE(PADDLE *p); /*Move a plataforma*/

/*Checa colisao entre a bola e a plataforma*/
bool colisaoBolaPlat( BALL a, PADDLE b );

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
            bola = criaBOLA(rand() % (SCREEN_WIDTH - BALL_WIDTH), 
                            rand() % (SCREEN_HEIGHT - BALL_HEIGHT), 
                            VELOCIDADE_INIC_BOLA, 
                            VELOCIDADE_INIC_BOLA,
                            gBALLSurface);
           
            
            plataforma = criaPLATAFORMA(((SCREEN_WIDTH/2) - (PADDLE_WIDTH/2)),
					 (SCREEN_HEIGHT - 2*PADDLE_HEIGHT),
					  VELOCIDADE_JOGADOR,
					  gPADDLESurface);
			
			gVidas = NUM_VIDAS;
             
            /*Flag do loop principal*/
            quit = false;

            /*Enquanto estiver em execucao*/
            while( !quit ) {
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
				
				/*Preenche a superficie de branco*/
				SDL_FillRect( gScreenSurface, NULL, 
							  SDL_MapRGB( gScreenSurface->format, 
							  0xFF, 0xFF, 0xFF ) );

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
				
				/*Atualiza a superficie*/
				SDL_UpdateWindowSurface( gWindow );
					
				/*Adiciona um delay para desacelerar a execucao do programa*/
				SDL_Delay(10);
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
		p->velX = VELOCIDADE_INIC_BOLA;
		p->velY = VELOCIDADE_INIC_BOLA;
		p->posX = SCREEN_WIDTH/2 - BALL_WIDTH/2;
		p->posY = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
		gVidas--;
		printf("Vidas = %d\n", gVidas); /*print de teste*/
	}
		
	if (colisaoBolaPlat(bola, plataforma)) {
		
		int centroPlataforma = plataforma.posX + PADDLE_WIDTH/2;
		int centroBola = p->posX + BALL_WIDTH/2;
		int localPlataforma = centroBola - centroPlataforma;
		
		p->velX = localPlataforma/FATOR_VELOCIDADE;
		
		if ( p->velX > 0 && p->velX > VELOCIDADE_MAX_BOLA ) p->velX = VELOCIDADE_MAX_BOLA;
		if ( p->velX < 0 && -(p->velX) > VELOCIDADE_MAX_BOLA ) p->velX = -(VELOCIDADE_MAX_BOLA);
		printf("velX = %d\n", p->velX); /*print de teste*/
		
		p->velY = -p->velY;
		p->posX += p->velX;
		p->posY += p->velY;
	}
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
BALL criaBOLA( int posX, int posY, int velX, int velY, SDL_Surface *image) {
	
    BALL p;
    
    p.posX = posX;
    p.posY = posY;
    p.velX = velX;
    p.velY = velY;
    p.raio = BALL_WIDTH/2;
    p.image = image;
    return p;
}

PADDLE criaPLATAFORMA( int posX, int posY, int velX, SDL_Surface *image) {
	
    PADDLE p;
    
    p.posX = posX;
    p.posY = posY;
    p.velX = velX;
    p.image = image;
    return p;
}

int init() {
    /*Flag de inicializacao*/
    int success = true;
    srand(time(NULL));

    /*Inicializa o SDL*/
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
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

	
    if( gBALLSurface == NULL || gPADDLESurface == NULL ) {
        printf( "Falha ao carregar imagem! SDL Error: %s\n", SDL_GetError() );
        success = false;
    } 
    return success;
}

void closing() {
    /*Libera a imagem carregada*/
    SDL_FreeSurface( gBALLSurface );
    gBALLSurface = NULL;
    SDL_FreeSurface ( gPADDLESurface );
    gPADDLESurface = NULL;

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
