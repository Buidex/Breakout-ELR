/*
 * Globais.c
 * 
 * Copyright 2016 Adriano Cruz <adriano@nce.ufrj.br>
 * 
 */

#include "Globais.h"

/* Variaveis globais */
int gTimer;
int gLevel;
int gPontos;
int gContaPontos;
int gVidas;
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
