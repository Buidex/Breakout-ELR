/*
 * Defs.h
 * 
 * Copyright 2016 Adriano Cruz <adriano@nce.ufrj.br>
 * 
 */

#ifndef DEFS_H
#define DEFS_H

/*DEFINES*/
#define NUM_LINHAS 4
#define NUM_COLUNAS 10
#define NUM_BLOCOS 40

#define MAX 80

#define FRAMES_PER_SECOND 60
#define FRAME_RATE 1000/FRAMES_PER_SECOND

/*Constantes de dimensoes da tela*/
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/*Constantes Booleanas*/
#define true 1
#define false 0

/*Constantes de dimensoes de imagens*/
#define BALL_WIDTH 22
#define BALL_HEIGHT 22
#define PADDLE_WIDTH 104
#define PADDLE_HEIGHT 24
#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 40

/*Constantes de jogo*/
#define NUM_VIDAS 5
#define DIST_BLOCO_TELA 80
#define VELOCIDADE_JOGADOR 7
#define VELOCIDADE_INIC_BOLA 5
#define VELOCIDADE_MAX_BOLA 6
#define FATOR_VELOCIDADE 5


/* Define o tipo bool */
typedef int bool;

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

#endif

