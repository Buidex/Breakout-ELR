/*
 * Inspirado no clássico jogo Breakout, com algumas mudanças
 * Programa feito por Elizeu, Luiz Fernando e Rodrigo
*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define MAX 80

/* Constants*/

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int false = 0;
const int true = 1;

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

	/*Superficie da imagem "RECORDES"*/
SDL_Surface* surfaceRECORDES = NULL;

	/*Superficie contida na tela da janela do jogo*/
SDL_Surface* surfaceJogo = NULL;

	/*Superficie contida na tela da janela dos recordes*/
SDL_Surface* surfaceRecordes = NULL;

	/*Superficie dos textos*/
SDL_Surface* texto = NULL;

	/*As músicas que serão utilizadas*/
Mix_Music *gMandela = NULL;
Mix_Music *gBaile = NULL;

	/*A fonte de texto a ser utilizada*/
TTF_Font *font = NULL;	

	/*A cor da fonte de texto a ser utilizada*/
SDL_Color corPreta = {0,0,0};

/* Function Prototypes*/

	/*Inicia o SDL e cria a janela inicial*/
int init();

	/*Carrega imagens*/
int loadMedias();

	/*Carrega a música do Mandela*/
int loadMandela();

	/*Carrega a música do Baile de Favela*/
int loadBaile();

	/*Carrega fontes*/
int loadFont();

	/*Gera a superfície inicial*/
int gerarTelaInicial();

	/*Libera as midias e encerra o SDL*/
void closing();

	/*Lê o arquivo dos recordes*/
int leituraRecordes();

	/*Superficie da fonte de texto*/
SDL_Surface* TTF_RenderText_Solid(TTF_Font *font, const char *text, SDL_Color fg);

	/*Busca a imagem no diretorio */
SDL_Surface* loadSurface(char *path);

	/*Cria o perfil das imagens*/
IMAGENS imagemJogar, imagemRecordes, imagemTexto;

	/*Cria a imagem na tela com uma posição definida*/
IMAGENS criarImagem(int posX, int posY, SDL_Surface* imagem);

int x, y;

SDL_Rect dstRect;

char jogador1[MAX],jogador2[MAX],jogador3[MAX],jogador4[MAX],jogador5[MAX],placar1[MAX],placar2[MAX],placar3[MAX],placar4[MAX],placar5[MAX];



int main(int argc, char* args[]){

	int quit;
	/*Evento de sair do jogo*/
	SDL_Event sair;
	
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
		if(!loadBaile()){
			printf("Falha ao carregar o áudio do Baile!\n");
		}
		if(!loadMandela()){
			printf("Falha ao carregar o áudio do Mandela!\n");
		}
		/*Carrega as fontes*/
		if(!loadFont()){
			printf("Falha ao carregar as fontes!\n");
		}
		
		else{
			
			gerarTelaInicial();
			
			
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
                                loadBaile();
                                gerarTelaInicial();
                            }
                            if(sair.key.keysym.sym == SDLK_m){
								
								if( Mix_PlayingMusic() == 0 ) { 
									
									Mix_PlayMusic( gMandela, -1 ); 
									Mix_PlayMusic( gBaile, -1 ); 
								} 
								
								else { 
								
									if( Mix_PausedMusic() == 1 ) 
									{ 
										
										Mix_ResumeMusic(); 
									} 
										
									else { 
										
										Mix_PauseMusic(); 
									} 
								} 
							}
							
							/*if(sair.key.keysym.sym == SDLK_s){
								
								if( Mix_PlayingMusic() == 0 ) { 
									
									Mix_PlayMusic( gMandela, -1 ); 
									Mix_PlayMusic( gBaile, -1 ); 
								} 
								
								else { 
								
									if( Mix_PausedMusic() == 1 ) 
									{ 
										
										Mix_ResumeMusic(); 
									} 
										
									else { 
										
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
									
									/*Libera a memória das música da tela inicial*/
									loadMandela();
					
									if( telaInicial == NULL ) {
										printf("A janela da tela inicial nao pode ser criada! SDL Error: %s\n", SDL_GetError());
										quit = false;
									}
									else {
										/*Troco a superfície inicial para a superfície de Jogo e ativo todos os componentes dessa nova superfície*/
										
										/*Captura a superficie da janela da tela inicial*/
										surfaceJogo = SDL_GetWindowSurface(telaInicial);
										SDL_FillRect (surfaceJogo, NULL, SDL_MapRGB(surfaceJogo->format, 0xFF, 0xFF, 0XFF));
										Mix_PlayMusic( gMandela, -1 );
										
										/* CÓDIGO DO JOGO AQUI */
										
									}
								}
								
								if((x>SCREEN_WIDTH-520) && (x<SCREEN_WIDTH-520+222) && (y>SCREEN_HEIGHT-335) && (y<SCREEN_HEIGHT-335+62)){
									printf("CLICOU EM CIMA DO 'RECORDES'\n");
									/*Libera a memória das imagens carregadas*/
					
									if( telaInicial == NULL ) {
										printf("A janela da tela inicial nao pode ser criada! SDL Error: %s\n", SDL_GetError());
										quit = false;
									}
									else {
										/*Troco a superfície inicial para a superfície de Jogo e ativo todos os componentes dessa nova superfície*/
										
										/*Captura a superficie da janela da tela inicial*/
										surfaceRecordes = SDL_GetWindowSurface(telaInicial);
										SDL_FillRect (surfaceRecordes, NULL, SDL_MapRGB(surfaceRecordes ->format, 0xFF, 0xFF, 0XFF));
										
										leituraRecordes();
										
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
    if(TTF_Init() < 0) {
		printf("TTF não pode ser iniciado! SDL_TTF Error: %s\n", TTF_GetError());
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

    /*Carrega a superficie da imagem de "RECORDES"*/
    surfaceRECORDES = loadSurface("./RECORDES.png");
    SDL_SetColorKey(surfaceRECORDES, SDL_TRUE, SDL_MapRGB(surfaceRECORDES->format, 0xFF, 0xFF, 0XFF));
    if(surfaceRECORDES == NULL){
        printf("Falha ao carregar a imagem de 'RECORDES'! SDL Error: %s\n", SDL_GetError());
        success = false;
    }   
    
    return success;
}

int loadMandela() {
	/*Variavel de controle do carregamento de áudios e músicas*/
    int success = true;
	
	/*Carrega a música do Mandela (tela do jogo)*/
    gMandela = Mix_LoadMUS( "mandela8bit.wav" ); 
    if( gMandela == NULL ) 
    { 
		printf( "Falha ao carregar a música de 'MANDELA'! SDL_mixer Error: %s\n", Mix_GetError() ); 
		success = false; 
	}
	
	Mix_FreeMusic( gBaile ); 
	gBaile = NULL;
	
	return success;
}

int loadBaile(){	
	/*Variavel de controle do carregamento de áudios e músicas*/
    int success = true;
	
	/*Carrega a música do Baile de Favela (tela do jogo)*/
    gBaile = Mix_LoadMUS( "bailedefavela8bit.wav" ); 
    if( gBaile == NULL ) 
    { 
		printf( "Falha ao carregar a música de 'BAILE DE FAVELA'! SDL_mixer Error: %s\n", Mix_GetError() ); 
		success = false; 
	}
	
	Mix_FreeMusic( gMandela ); 
	gMandela = NULL;
	
	return success;
}

int loadFont() {
	/*Variavel de controle do carregamento de fontes de texto*/
	int success = true;

	font = TTF_OpenFont("ubuntu.medium.ttf", 24);
	if(!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		success = false;
	}

	return success;
}

int gerarTelaInicial() {
			/*Variavel de controle do carregamento de fontes de texto*/
			int success = true;
	
			imagemJogar = criarImagem((SCREEN_WIDTH-520), (SCREEN_HEIGHT-435), surfaceJOGAR);
			imagemRecordes = criarImagem((SCREEN_WIDTH-520), (SCREEN_HEIGHT-335), surfaceRECORDES);
			
			loadBaile();
			Mix_PlayMusic( gBaile, -1 );
			
				/*Pinta a superfície de branco*/
				SDL_FillRect (surfaceInicial, NULL, SDL_MapRGB(surfaceInicial->format, 0xFF, 0xFF, 0XFF));
			
                dstRect.x = imagemJogar.posX;
                dstRect.y = imagemJogar.posY;
                
                if(SDL_BlitSurface( imagemJogar.imagem, NULL, 
                                     surfaceInicial, &dstRect ) < 0 ) {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    success = true;
                }	
                
                dstRect.x = imagemRecordes.posX;
                dstRect.y = imagemRecordes.posY;
                
                if(SDL_BlitSurface( imagemRecordes.imagem, NULL, 
                                     surfaceInicial, &dstRect ) < 0 ) {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    success = true;
                }
                                
                texto = TTF_RenderText_Solid(font, "BREAKOUT FUNK", corPreta);
				imagemTexto = criarImagem((SCREEN_WIDTH-507), (SCREEN_HEIGHT-500), texto);
				
                dstRect.x = imagemTexto.posX;
                dstRect.y = imagemTexto.posY;
				
				if(!texto) {
					printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
					success = false;
				} 
				
                
				else {
					if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceInicial,&dstRect)){
						printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
						success = true;
					}
				}
				
				texto = TTF_RenderText_Solid(font, "Comandos quando em jogo:", corPreta);
				imagemTexto = criarImagem((SCREEN_WIDTH-670), (SCREEN_HEIGHT-215), texto);
				
                dstRect.x = imagemTexto.posX;
                dstRect.y = imagemTexto.posY;
				
				if(!texto) {
					printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
					success = false;
				} 
				
                
				else {
					if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceInicial,&dstRect)){
						printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
						success = true;
					}
				}
				
				texto = TTF_RenderText_Solid(font, "Aperte S para ligar ou desligar os sons do jogo.", corPreta);
				imagemTexto = criarImagem((SCREEN_WIDTH-620), (SCREEN_HEIGHT-175), texto);
				
                dstRect.x = imagemTexto.posX;
                dstRect.y = imagemTexto.posY;
				
				if(!texto) {
					printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
					success = false;
				} 
				
                
				else {
					if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceInicial,&dstRect)){
						printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
						success = true;
					}
				}
				
				texto = TTF_RenderText_Solid(font, "Aperte M para ligar ou desligar a musica.", corPreta);
				imagemTexto = criarImagem((SCREEN_WIDTH-620), (SCREEN_HEIGHT-135), texto);
				
                dstRect.x = imagemTexto.posX;
                dstRect.y = imagemTexto.posY;
				
				if(!texto) {
					printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
					success = false;
				} 
				
                
				else {
					if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceInicial,&dstRect)){
						printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
						success = true;
					}
				}
				
				texto = TTF_RenderText_Solid(font, "Aperte ESC para voltar para a tela inicial.", corPreta);
				imagemTexto = criarImagem((SCREEN_WIDTH-620), (SCREEN_HEIGHT-95), texto);
				
                dstRect.x = imagemTexto.posX;
                dstRect.y = imagemTexto.posY;
				
				if(!texto) {
					printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
					success = false;
				} 
				
                
				else {
					if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceInicial,&dstRect)){
						printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
						success = true;
					}
				}
				
				texto = TTF_RenderText_Solid(font, "Grupo: MC's Elizeu Cappelli, Luiz Fernando Porto e Rodrigo Carvalho", corPreta);
				imagemTexto = criarImagem((SCREEN_WIDTH-800), (SCREEN_HEIGHT-35), texto);
				
                dstRect.x = imagemTexto.posX;
                dstRect.y = imagemTexto.posY;
				
				if(!texto) {
					printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
					success = false;
				} 
				
                
				else {
					if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceInicial,&dstRect)){
						printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
						success = true;
					}
				}	
	
	
	return success;
}

void closing() {
    /*Libera a memória das imagens carregadas*/
    SDL_FreeSurface( surfaceJOGAR );
    surfaceJOGAR = NULL;
     
    /*Libera a memória das imagens carregadas*/
    SDL_FreeSurface( surfaceRECORDES );
    surfaceRECORDES = NULL;
    
	/*Libera a memória das fontes carregadas*/
	SDL_FreeSurface(texto);
	texto = NULL;
    
    /*Libera a memória das músicas carregadas*/ 
    Mix_FreeMusic( gMandela ); 
    gMandela = NULL; 
    
    Mix_FreeMusic( gBaile ); 
    gBaile = NULL; 
    
    /*Libera a memória das fontes carregadas*/ 
    TTF_CloseFont(font); 
    font = NULL;
	
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

int leituraRecordes(){
	/*Variavel de controle do carregamento de fontes de texto*/
	int success = true;
	
	FILE *pfile = fopen("recordes.txt", "r");
	if (pfile == NULL){
		printf("Arquivo não pode ser aberto\n");
		success = false;
	}
		
	fgets(jogador1, MAX, pfile);
	jogador1[strlen(jogador1)-1] = '\0';
	printf("1 - ");
	puts(jogador1);
	fgets(placar1, MAX, pfile);
	placar1[strlen(placar1)-1] = '\0';
	printf("Placar: ");
	puts(placar1);
	
	fgets(jogador2, MAX, pfile);
	jogador2[strlen(jogador2)-1] = '\0';
	printf("2 - ");
	puts(jogador2);
	fgets(placar2, MAX, pfile);
	placar2[strlen(placar2)-1] = '\0';
	printf("Placar: ");
	puts(placar2);
	
	fgets(jogador3, MAX, pfile);
	jogador3[strlen(jogador3)-1] = '\0';
	printf("3 - ");
	puts(jogador3);
	fgets(placar3, MAX, pfile);
	placar3[strlen(placar3)-1] = '\0';
	printf("Placar: ");
	puts(placar3);	
	
	fgets(jogador4, MAX, pfile);
	jogador4[strlen(jogador4)-1] = '\0';
	printf("4 - ");
	puts(jogador4);
	fgets(placar4, MAX, pfile);
	placar4[strlen(placar4)-1] = '\0';
	printf("Placar: ");
	puts(placar4);
	
	fgets(jogador5, MAX, pfile);
	jogador5[strlen(jogador5)-1] = '\0';
	printf("5 - ");
	puts(jogador5);
	fgets(placar5, MAX, pfile);
	placar5[strlen(placar5)-1] = '\0';
	printf("Placar: ");
	puts(placar5);

	texto = TTF_RenderText_Solid(font, "REIS DO BREAKOUT FUNK", corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-530), (SCREEN_HEIGHT-485), texto);
				
    dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
				
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
    else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceInicial,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
    
	texto = TTF_RenderText_Solid(font, jogador1, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-560), (SCREEN_HEIGHT-415), texto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	texto = TTF_RenderText_Solid(font, placar1, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH - 680), (SCREEN_HEIGHT-413), texto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}

	texto = TTF_RenderText_Solid(font, jogador2, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-560), (SCREEN_HEIGHT-355), texto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	texto = TTF_RenderText_Solid(font, placar2, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-680), (SCREEN_HEIGHT-353), texto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	texto = TTF_RenderText_Solid(font, jogador3, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-560), (SCREEN_HEIGHT-295), texto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	texto = TTF_RenderText_Solid(font, placar3, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-680), (SCREEN_HEIGHT-293), texto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	texto = TTF_RenderText_Solid(font, jogador4, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-560), (SCREEN_HEIGHT-235), texto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	texto = TTF_RenderText_Solid(font, placar4, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-680), (SCREEN_HEIGHT-233), texto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	texto = TTF_RenderText_Solid(font, jogador5, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-560), (SCREEN_HEIGHT-175), texto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	texto = TTF_RenderText_Solid(font, placar5, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-680), (SCREEN_HEIGHT-173), texto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!texto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,surfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	fclose(pfile);
	
	return success;
}
