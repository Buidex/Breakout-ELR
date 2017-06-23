/*
 * Inspirado no clássico jogo Breakout, com algumas mudanças
 * Programa feito por Elizeu, Luiz Fernando e Rodrigo
*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_mixer.h>


/* Constants*/

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int false = 0;
const int true = 1;

int x, y;

typedef struct _IMAGEM{
	int posX;
	int posY;
	SDL_Surface* imagem;
} IMAGENS;


/* Global Variables*/

	/*Janela inicial*/
SDL_Window* telaInicial = NULL;

	/*Superficie contida na janela inicial*/
SDL_Surface* surfaceInicial = NULL;

	/*Superficie da imagem "JOGAR"*/
SDL_Surface* surfaceJOGAR = NULL;

	/*Superficie da imagem "CRÉDITOS"*/
SDL_Surface* surfaceCREDITOS = NULL;

	/*Superficie da imagem "RECORDES"*/
SDL_Surface* surfaceRECORDES = NULL;

	/*Superficie da imagem "MEC"*/
SDL_Surface* surfaceMEC = NULL;

	/*Superficie contida na tela da janela do jogo*/
SDL_Surface* surfaceJogo = NULL;

	/*As músicas que serão utilizadas*/
Mix_Music *gMandela = NULL;
Mix_Music *gBaile = NULL;

/* Function Prototypes*/

	/*Inicia o SDL e cria a janela inicial*/
int init();

	/*Carrega imagens*/
int loadMedias();

	/*Carrega áudios e músicas*/
int loadAudio();

	/*Libera as midias e encerra o SDL*/
void closing();

	/*Busca a imagem no diretorio */
SDL_Surface* loadSurface(char *path);

	/*Cria o perfil das imagens*/
IMAGENS imagemJogar, imagemCreditos, imagemRecordes, imagemMEC;

	/*Cria a imagem na tela com uma posição definida*/
IMAGENS criarImagem(int posX, int posY, SDL_Surface* imagem);


int main(int argc, char* args[]){

	int quit;
	/*Evento de sair do jogo*/
	SDL_Event sair;
	
	SDL_Rect srcRect, dstRect;
	
	/*Inicia o SDL e cria a janela inicial*/
	if(!init()){
		printf("Falha ao inicializar!\n");
	}
	
	else{
		/*Carrega as ibagens*/
		if(!loadMedias()){
			printf("Falha ao carregar as ibagens!\n");
		}
		/*Carrega os áudios*/
		if(!loadAudio()){
			printf("Falha ao carregar os áudios!\n");
		}
		else{
			imagemJogar = criarImagem((SCREEN_WIDTH-520), (SCREEN_HEIGHT-435), surfaceJOGAR);
			imagemCreditos = criarImagem((SCREEN_WIDTH-520), (SCREEN_HEIGHT-335), surfaceCREDITOS);
			imagemRecordes = criarImagem((SCREEN_WIDTH-520), (SCREEN_HEIGHT-235), surfaceRECORDES);
			Mix_PlayMusic( gBaile, -1 );
			
			/*Variavel de controle do loop principal*/
			quit = false;
			
				/*Pinta a superfície de branco*/
				SDL_FillRect (surfaceInicial, NULL, SDL_MapRGB(surfaceInicial->format, 0xFF, 0xFF, 0XFF));
			
				srcRect.x = 0; srcRect.y = 0;
                srcRect.w = 232;
                srcRect.h = 232;
                dstRect.x = imagemJogar.posX;
                dstRect.y = imagemJogar.posY;
                
                if(SDL_BlitSurface( imagemJogar.imagem, &srcRect, 
                                     surfaceInicial, &dstRect ) < 0 ) {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    quit = true;
                }	
                
                srcRect.x = 0; srcRect.y = 0;
                srcRect.w = 232;
                srcRect.h = 232;
                dstRect.x = imagemCreditos.posX;
                dstRect.y = imagemCreditos.posY;
                
                if(SDL_BlitSurface( imagemCreditos.imagem, &srcRect, 
                                     surfaceInicial, &dstRect ) < 0 ) {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    quit = true;
                }	
                
                srcRect.x = 0; srcRect.y = 0;
                srcRect.w = 232;
                srcRect.h = 232;
                dstRect.x = imagemRecordes.posX;
                dstRect.y = imagemRecordes.posY;
                
                if(SDL_BlitSurface( imagemRecordes.imagem, &srcRect, 
                                     surfaceInicial, &dstRect ) < 0 ) {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    quit = true;
                }	
			
			/*Enquanto o programa esta rodando (enquanto o valor de 'quit' for falso/0)*/
            while(!quit) {
                while( SDL_PollEvent( &sair ) != 0 ) { 
					/*Essa função retorna '1' se houver um evento rolando e '0' se não houver*/
                    switch (sair.type) {
                        case SDL_QUIT:
                            quit = true;
                            break;
                            
                        case SDL_KEYDOWN:
                            if (sair.key.keysym.sym == SDLK_ESCAPE) {
                                quit = true;
                            }
                            /*if(sair.key.keysym.sym == SDLK_9){
								If there is no music playing 
								if( Mix_PlayingMusic() == 0 ) { 
									Play the music 
									Mix_PlayMusic( gMusic, -1 ); 
								} 
								If music is being played
								else { 
								If the music is paused
									if( Mix_PausedMusic() == 1 ) 
									{ 
										Resume the music
										Mix_ResumeMusic(); 
									} 
										If the music is playing
									else { 
										Pause the music
										Mix_PauseMusic(); 
									} 
								} 
							}*/
							
                        case SDL_MOUSEBUTTONDOWN:
							if(sair.button.button == SDL_BUTTON_LEFT && surfaceJOGAR != NULL){
								printf("BOTAO ESQUERDO DO MOUSE FOI APERTADO MEC\n");
								x = sair.button.x;
								y = sair.button.y;
								if((x>SCREEN_WIDTH-520) && (x<SCREEN_WIDTH-520+222) && (y>SCREEN_HEIGHT-435) && (y<SCREEN_HEIGHT-435+62)){
									printf("CLICOU EM CIMA DO 'JOGAR'\n");
									/*Libera a memória das imagens carregadas*/
									SDL_FreeSurface( surfaceJOGAR );
									surfaceJOGAR = NULL;
									
									/*Libera a memória das música da tela inicial*/
									Mix_FreeMusic( gBaile ); 
									gBaile = NULL;
					
									if( telaInicial == NULL ) {
										printf("A janela da tela inicial nao pode ser criada! SDL Error: %s\n", SDL_GetError());
										quit = false;
									}
									else {
										/*Troco a superfície inicial para a superfície de Jogo e ativo todos os componentes dessa nova superfície*/
										
										/*Captura a superficie da janela da tela inicial*/
										surfaceJogo = SDL_GetWindowSurface(telaInicial);
										SDL_FillRect (surfaceJogo, NULL, SDL_MapRGB(surfaceJogo->format, 0xFF, 0xFF, 0XFF));
										imagemMEC = criarImagem((SCREEN_WIDTH-630), (SCREEN_HEIGHT-485), surfaceMEC);
										Mix_PlayMusic( gMandela, -1 );
										
										srcRect.x = 0; srcRect.y = 0;
										srcRect.w = 480;
										srcRect.h = 360;
										dstRect.x = imagemMEC.posX;
										dstRect.y = imagemMEC.posY;
                
										if(SDL_BlitSurface( imagemMEC.imagem, &srcRect, 
											surfaceJogo, &dstRect ) < 0 ) {
											printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
											quit = true;
										}	
									
									}
								}
                            
                        break;
                    }
                }		
			
				/*Atualiza a superficie*/
				SDL_UpdateWindowSurface(telaInicial);
			}
		}
	}
	}
		/*Libera a memória de tudo e encerra o SDL */
		closing();
		
		return 0;
}

IMAGENS criarImagem(int posX, int posY, SDL_Surface* imagem){
	IMAGENS img;
	
	img.posX = posX;
	img.posY = posY;
	img.imagem = imagem;
	
	return img;
}


int init(){

	/*Variavel controle de inicializacao*/
    int success = true;

    /*Inicializa o SDL*/
    if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL não pode ser inicializado! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else 
    {
        /*Cria a janela inicial*/
        telaInicial = SDL_CreateWindow("Breakout Funk - The Ultimate Version", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if( telaInicial == NULL ) {
            printf("A janela da tela inicial não pode ser criada! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            /*Inicializa o caregamento JPG e PNG */
            int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
            if( !(IMG_Init(imgFlags) & imgFlags)) {
                printf("SDL_image não pode ser inicializado! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            }
            /*Inicializa o caregamento de áudios*/
            if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) 
            { 
				printf( "SDL_mixer não pode ser inicializado! SDL_mixer Error: %s\n", Mix_GetError() );
				success = false; 
			}
            else 
            {
                /*Captura a superficie da janela da tela inicial*/
                surfaceInicial = SDL_GetWindowSurface(telaInicial);
			}
		}
	}
	return success;
}

int loadMedias() {
    /*Variavel de controle do carregamento de imagens*/
    int success = true;
    
    /*Carrega a superficie da imagem de "JOGAR"*/
    surfaceJOGAR = loadSurface("./JOGAR.png");
    SDL_SetColorKey(surfaceJOGAR, SDL_TRUE, SDL_MapRGB(surfaceJOGAR->format, 0xFF, 0xFF, 0XFF));
    if(surfaceJOGAR == NULL){
        printf("Falha ao carregar a imagem de 'JOGAR'! SDL Error: %s\n", SDL_GetError());
        success = false;
    } 
    /*Carrega a superficie da imagem de "CRÉDITOS"*/
    surfaceCREDITOS = loadSurface("./CREDITOS.png");
    SDL_SetColorKey(surfaceCREDITOS, SDL_TRUE, SDL_MapRGB(surfaceCREDITOS->format, 0xFF, 0xFF, 0XFF));
    if(surfaceCREDITOS == NULL){
        printf("Falha ao carregar a imagem de 'CREDITOS'! SDL Error: %s\n", SDL_GetError());
        success = false;
    } 
    /*Carrega a superficie da imagem de "RECORDES"*/
    surfaceRECORDES = loadSurface("./RECORDES.png");
    SDL_SetColorKey(surfaceRECORDES, SDL_TRUE, SDL_MapRGB(surfaceRECORDES->format, 0xFF, 0xFF, 0XFF));
    if(surfaceRECORDES == NULL){
        printf("Falha ao carregar a imagem de 'RECORDES'! SDL Error: %s\n", SDL_GetError());
        success = false;
    } 
    
    surfaceMEC = loadSurface("./mec.jpg");
    SDL_SetColorKey(surfaceMEC, SDL_TRUE, SDL_MapRGB(surfaceMEC->format, 0xFF, 0xFF, 0XFF));
    if(surfaceMEC == NULL){
        printf("Falha ao carregar a imagem de 'MEC'! SDL Error: %s\n", SDL_GetError());
        success = false;
    } 
    
    return success;
}

int loadAudio() {
	/*Variavel de controle do carregamento de áudios e músicas*/
    int success = true;
	
	/*Carrega a música do Mandela (tela do jogo)*/
    gMandela = Mix_LoadMUS( "mandela8bit.wav" ); 
    if( gMandela == NULL ) 
    { 
		printf( "Falha ao carregar a música de 'MANDELA'! SDL_mixer Error: %s\n", Mix_GetError() ); 
		success = false; 
	}
	/*Carrega a música do Baile de Favela (tela do jogo)*/
    gBaile = Mix_LoadMUS( "bailedefavela8bit.wav" ); 
    if( gBaile == NULL ) 
    { 
		printf( "Falha ao carregar a música de 'BAILE DE FAVELA'! SDL_mixer Error: %s\n", Mix_GetError() ); 
		success = false; 
	}
	
	return success;
}

void closing() {
    /*Libera a memória das imagens carregadas*/
    SDL_FreeSurface( surfaceJOGAR );
    surfaceJOGAR = NULL;
    
    /*Libera a memória das imagens carregadas*/
    SDL_FreeSurface( surfaceCREDITOS );
    surfaceCREDITOS = NULL;
    
    /*Libera a memória das imagens carregadas*/
    SDL_FreeSurface( surfaceRECORDES );
    surfaceRECORDES = NULL;
    
    /*Libera a memória das imagens carregadas*/
    SDL_FreeSurface( surfaceMEC );
    surfaceMEC = NULL;
    
    /*Libera a memória das músicas carregadas*/ 
    Mix_FreeMusic( gMandela ); gMandela = NULL; 
    Mix_FreeMusic( gBaile ); gBaile = NULL; 
	
    /*Destroi as janelas*/
    SDL_DestroyWindow( telaInicial );
    telaInicial = NULL;

    /*Fecha os subsistemas do SDL*/
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

SDL_Surface* loadSurface( char *path ) {
    /*A imagem otimizada final*/
    SDL_Surface* optimizedSurface = NULL;

    /*Carrega uma imagem com um path específico*/
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL ) {
        printf( "Falha ao carregar a imagem %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else {
		
        /*Converte a superfície para o formato da tela*/
        optimizedSurface = SDL_ConvertSurface( loadedSurface, surfaceInicial->format, 0 );
        if( optimizedSurface == NULL ) {
            printf( "Erro ao otimizar imagem %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        /*Get rid of old loaded surface*/
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}
