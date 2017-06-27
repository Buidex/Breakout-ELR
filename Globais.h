/*
 * Globais.h
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

#ifndef GLOBAIS_H
#define GLOBAIS_H

/* Variaveis globais */
extern int gTimer;
extern int gLevel;
extern int gPontos;
extern int gContaPontos;
extern int gVidas;
extern int gTeclaDir;
extern int gTeclaEsq;
extern int gNumBlocos;
extern int gMuteSoundFlag;
extern int gQuitFlag;
extern BLOCK gBlocos[NUM_BLOCOS];

/*A janela em que estaremos renderizando*/
extern SDL_Window* gWindow;

/*Os personagens*/
extern BALL bola;
extern PADDLE plataforma;

/*As imagens*/
extern IMAGEM imagemJogar, imagemRecordes, imagemTexto;
  
/*As superficies do jogo*/
extern SDL_Surface* gSurfaceInicial;
extern SDL_Surface* gSurfaceJOGAR;
extern SDL_Surface* gSurfaceRECORDES;
extern SDL_Surface* gSurfaceJogo;
extern SDL_Surface* gSurfaceRecordes;
extern SDL_Surface* gTexto;


/*Imagens PNG atualmente exibidas*/
extern SDL_Surface* gBALLSurface;
extern SDL_Surface* gPADDLESurface;
extern SDL_Surface* gBLOCK1Surface;
extern SDL_Surface* gBLOCK2Surface;
extern SDL_Surface* gBLOCK3Surface;
extern SDL_Surface* gBLOCK4Surface;
extern SDL_Surface* gBLOCK5Surface;

/*Os efeitos sonoros que serao utilizados*/
extern Mix_Music *gMandela;
extern Mix_Music *gBaile;
extern Mix_Chunk *gEfeitoParedes;
extern Mix_Chunk *gEfeitoBlocos;

/*A fonte de texto a ser utilizada*/
extern TTF_Font *gFont;

/*A cor da fonte de texto a ser utilizada*/
extern SDL_Color corPreta;
extern SDL_Color corBranca;

#endif
