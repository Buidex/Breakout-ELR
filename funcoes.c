/*
 * funcoes.c
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

#include "funcoes.h"

/* Funcoes */

/*Inicia o SDL e cria a janela inicial*/
bool init() {
    /*Flag de inicializacao*/
    int success = true;
    atexit(SDL_Quit);

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

/*Carrega uma imagem de um diretorio*/
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

/*Carrega as midias*/
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

/*Carrega a musica 'Mandela'*/
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

/*Carrega a musica 'Baile de Favela'*/
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

/*Gera o menu principal*/
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

/*Cria Blocos*/
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
			gBlocos[i].posY = lin*BLOCK_HEIGHT;
			gBlocos[i].imgW = BLOCK_WIDTH;
			gBlocos[i].imgH = BLOCK_HEIGHT;
			i++;
			gNumBlocos++;
		}
	}	
}

/*Gera Blocos*/ 
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

/*Gera o quadro de pontuacao*/
bool quadroDePontuacao() {
	
	int success = true;
	SDL_Rect dstRect;
	
	char level[10], vidas[10], pontos[10], blocos[10];
	
	sprintf(level, "%d", gLevel);
	sprintf(vidas, "%d", gVidas);
	sprintf(pontos, "%d", gPontos);
	sprintf(blocos, "%d", gNumBlocos);
	
	gTexto = TTF_RenderText_Solid(gFont, "|| Nivel : ", corBranca);
	imagemTexto = criarImagem((SCREEN_WIDTH-790), 5, gTexto);
				
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
	imagemTexto = criarImagem((SCREEN_WIDTH-690), 5, gTexto);
				
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
	imagemTexto = criarImagem((SCREEN_WIDTH-670), 5, gTexto);
				
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
	imagemTexto = criarImagem((SCREEN_WIDTH-550), 5, gTexto);
				
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
	imagemTexto = criarImagem((SCREEN_WIDTH-510), 5, gTexto);
				
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
	imagemTexto = criarImagem((SCREEN_WIDTH-380), 5, gTexto);
				
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
	imagemTexto = criarImagem((SCREEN_WIDTH-300), 5, gTexto);
				
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
	imagemTexto = criarImagem((SCREEN_WIDTH-60), 5, gTexto);
				
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
	
	gTexto = TTF_RenderText_Solid(gFont, "___________________________________________________________________", corBranca);
	imagemTexto = criarImagem( 0, 12, gTexto);
				
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

/*Muda de nivel*/
void MudaLevel(BALL *a, PADDLE *b) {
	
	gLevel++;
	a->velX = 0;
	a->velY = 0;
	a->posX = SCREEN_WIDTH/2 - BALL_WIDTH/2;
	a->posY = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
	b->posX = SCREEN_WIDTH/2 - PADDLE_WIDTH/2;
	b->posY = SCREEN_HEIGHT - 2*PADDLE_HEIGHT;
	gPontos += 1000;
	gContaPontos += 1000;
	if (gContaPontos == 10000) {
		gContaPontos = 0;
		gVidas++;
	}
	gNumBlocos = 0;
	criaBlocos(gBlocos);
	if (gLevel == 7) gQuitFlag = true;
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

/*Checa colisao entre bola e bloco*/
bool PontoNoBloco(int x, int y, int w, int z) {
	
	if ( (x >= w) && (x <= w + BLOCK_WIDTH) && 
	   (y >= z) && (y <= z + BLOCK_HEIGHT) ) {
		return true;
	}
	return false;
}

/*Deleta o bloco atingido*/
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

/*Checa colisao na parte de cima do bloco*/
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

/*Checa colisao na parte de baixo do bloco*/
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

/*Checa colisao na lateral esquerda do bloco*/
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

/*Checa colisao na lateral direita do bloco*/
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

/*Move a bola e toca os efeitos sonoros apropriados*/
void moveBALL(BALL *p) {
	
    p->posX += p->velX;
    p->posY += p->velY;
    
    if ( (p->posX + BALL_WIDTH > SCREEN_WIDTH) || (p->posX < 0) ) {
        p->velX = -p->velX;
        p->posX += p->velX;
        if ( !gMuteSoundFlag ) Mix_PlayChannel( -1, gEfeitoParedes, 0 );
    }
    if ( p->posY < BLOCK_HEIGHT ){
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

void checaRecordes(){

	char* inFileName = "recordes.txt";
	char* outFileName = "temp.txt";
	FILE* inFile = fopen(inFileName, "r");
	FILE* outFile = fopen(outFileName, "w+");
	char jogador[MAX], jogador1[MAX],jogador2[MAX],jogador3[MAX],jogador4[MAX],jogador5[MAX],placar1[MAX],placar2[MAX],placar3[MAX],placar4[MAX],placar5[MAX];
	char barraN[5] = "\n";
	char* ptr;
	int pont,ret;
	int pont1 = 0;
	int pont2 = 0;
	int pont3 = 0;
	int pont4 = 0;
	int pont5 = 0;
	
	pont = gPontos;

	if( inFile == NULL || outFile == NULL )
	{
		printf("Erro ao abrir arquivo\n.");
	}
	
	while( fgets(jogador1, MAX, inFile) != NULL )
	{
		jogador1[strlen(jogador1)-1] = '\0';
		fgets(placar1, MAX, inFile);
		fgets(jogador2, MAX, inFile);
		jogador2[strlen(jogador2)-1] = '\0';
		fgets(placar2, MAX, inFile);
		fgets(jogador3, MAX, inFile);
		jogador3[strlen(jogador3)-1] = '\0';
		fgets(placar3, MAX, inFile);
		fgets(jogador4, MAX, inFile);
		jogador4[strlen(jogador4)-1] = '\0';
		fgets(placar4, MAX, inFile);
		fgets(jogador5, MAX, inFile);
		jogador5[strlen(jogador5)-1] = '\0';
		fgets(placar5, MAX, inFile);
		
		pont1 = strtol(placar1, &ptr, 10);
		pont2 = strtol(placar2, &ptr, 10);
		pont3 = strtol(placar3, &ptr, 10);
		pont4 = strtol(placar4, &ptr, 10);
		pont5 = strtol(placar5, &ptr, 10);
		
		if(pont > pont1){
			
			puts("Parabens! Voce bateu o recorde do jogo e entrou na primeira colocacao!");
			puts("Insira seu nome.");
			fgets(jogador, MAX, stdin);
			jogador[strlen(jogador)-1] = '\0';
			fprintf(outFile, "%s", jogador);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
		}
		
		else if (pont > pont2){
			puts("Parabens! Voce bateu um recorde do jogo e entrou na segunda colocacao!");
			puts("Insira seu nome.");
			fgets(jogador, MAX, stdin);
			jogador[strlen(jogador)-1] = '\0';
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
		}
	
		else if (pont > pont3){
			puts("Parabens! Voce bateu um recorde do jogo e entrou na terceira colocacao!");
			puts("Insira seu nome.");
			fgets(jogador, MAX, stdin);
			jogador[strlen(jogador)-1] = '\0';
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
		}
	
		else if (pont > pont4){
			puts("Parabens! Voce bateu um recorde do jogo e entrou na quarta colocacao!");
			puts("Insira seu nome.");
			fgets(jogador, MAX, stdin);
			jogador[strlen(jogador)-1] = '\0';
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
		}
	
		else if(pont > pont5){
			puts("Parabens! Voce bateu um recorde do jogo e entrou na quinta colocacao!");
			puts("Insira seu nome.");
			fgets(jogador, MAX, stdin);
			jogador[strlen(jogador)-1] = '\0';
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador5);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont5);fprintf(outFile, "%s", barraN);
		}
		else{
			puts("Voce nao teve pontuacao sucifiente para entrar nos recordes.");
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador5);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont5);fprintf(outFile, "%s", barraN);
		}
	}

	fclose(inFile);
	fclose(outFile);
	
    ret = remove(inFileName);

    if(ret != 0) 
    {
       printf("Erro: nao foi possivel deletar arquivo.\n");
    }
	
	ret = rename(outFileName, inFileName);
	
    if(ret != 0) 
    {
       printf("Erro: nao foi possivel renomear arquivo.\n");
    }
}

bool leituraRecordes() {
	/*Variavel de controle do carregamento de fontes de texto*/
	int success = true;
	
	SDL_Rect dstRect;
	
	char jogador1[MAX],jogador2[MAX],jogador3[MAX],jogador4[MAX],jogador5[MAX],placar1[MAX],placar2[MAX],placar3[MAX],placar4[MAX],placar5[MAX];
	
	FILE *pfile = fopen("recordes.txt", "r");
	if (pfile == NULL){
		printf("Arquivo não pode ser aberto\n");
		success = false;
	}
		
	fgets(jogador1, MAX, pfile);
	jogador1[strlen(jogador1)-1] = '\0';
	fgets(placar1, MAX, pfile);
	placar1[strlen(placar1)-1] = '\0';
	
	fgets(jogador2, MAX, pfile);
	jogador2[strlen(jogador2)-1] = '\0';
	fgets(placar2, MAX, pfile);
	placar2[strlen(placar2)-1] = '\0';
	
	fgets(jogador3, MAX, pfile);
	jogador3[strlen(jogador3)-1] = '\0';
	fgets(placar3, MAX, pfile);
	placar3[strlen(placar3)-1] = '\0';
	
	fgets(jogador4, MAX, pfile);
	jogador4[strlen(jogador4)-1] = '\0';
	fgets(placar4, MAX, pfile);
	placar4[strlen(placar4)-1] = '\0';
	
	fgets(jogador5, MAX, pfile);
	jogador5[strlen(jogador5)-1] = '\0';
	fgets(placar5, MAX, pfile);
	placar5[strlen(placar5)-1] = '\0';
	
	gTexto = TTF_RenderText_Solid(gFont, "REIS DO BREAKOUT FUNK", corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-530), (SCREEN_HEIGHT-485), gTexto);
				
    dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
				
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
				
    else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceInicial,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
    
	gTexto = TTF_RenderText_Solid(gFont, jogador1, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-560), (SCREEN_HEIGHT-415), gTexto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	gTexto = TTF_RenderText_Solid(gFont, placar1, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH - 680), (SCREEN_HEIGHT-413), gTexto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}

	gTexto = TTF_RenderText_Solid(gFont, jogador2, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-560), (SCREEN_HEIGHT-355), gTexto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	gTexto = TTF_RenderText_Solid(gFont, placar2, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-680), (SCREEN_HEIGHT-353), gTexto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	gTexto = TTF_RenderText_Solid(gFont, jogador3, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-560), (SCREEN_HEIGHT-295), gTexto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	gTexto = TTF_RenderText_Solid(gFont, placar3, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-680), (SCREEN_HEIGHT-293), gTexto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	gTexto = TTF_RenderText_Solid(gFont, jogador4, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-560), (SCREEN_HEIGHT-235), gTexto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	gTexto = TTF_RenderText_Solid(gFont, placar4, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-680), (SCREEN_HEIGHT-233), gTexto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	gTexto = TTF_RenderText_Solid(gFont, jogador5, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-560), (SCREEN_HEIGHT-175), gTexto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	gTexto = TTF_RenderText_Solid(gFont, placar5, corPreta);
	imagemTexto = criarImagem((SCREEN_WIDTH-680), (SCREEN_HEIGHT-173), gTexto);
	dstRect.x = imagemTexto.posX;
    dstRect.y = imagemTexto.posY;
	if(!gTexto) {
		printf("Erro ao renderizar a fonte! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	} 
	else {
		if(SDL_BlitSurface(imagemTexto.imagem,NULL,gSurfaceRecordes,&dstRect)){
			printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
			success = true;
		}
	}
	
	SDL_FreeSurface(gTexto);
	gTexto = NULL;
	
	fclose(pfile);
	
	return success;
}

/*Encerra o SDL*/
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
