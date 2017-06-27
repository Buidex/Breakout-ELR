/*
 * funcoes.h
 * 
 * Copyright 2016 Adriano Cruz <adriano@nce.ufrj.br>
 * 
 */

#ifndef FUNCOES_H
#define FUNCOES_H

#include "Defs.h"
#include "Globais.h"

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
SDL_Surface* loadSurface( char * );

/*Carrega a superficie da fonte de texto*/
SDL_Surface* TTF_RenderText_Solid(TTF_Font *, const char *, SDL_Color);

/*Gera a tela inicial*/
bool gerarTelaInicial();

/*Cria a imagem na tela com uma posição definida*/
IMAGEM criarImagem(int , int , SDL_Surface *);

/*Cria os blocos*/
void criaBlocos(BLOCK *);

/*Gera os blocos*/
void geraBlocos(BLOCK *);

/*Gera o quadro de pontuacao*/
bool quadroDePontuacao();

/*Cria o personagem*/
BALL criaBOLA( int , int , int , int , SDL_Surface *);
PADDLE criaPLATAFORMA( int , int , int , SDL_Surface *);

/*Move o personagem*/
void moveBALL(BALL *); /*Move a bola e toca os efeitos sonoros apropriados*/
void movePADDLE(PADDLE *); /*Move a plataforma*/

/*Checa colisao entre a bola e a plataforma*/
bool colisaoBolaPlat(BALL , PADDLE);

/*Colisao entre bola e bloco*/
bool colisaoBolaBlocoCima(BALL , BLOCK *);
bool colisaoBolaBlocoBaixo(BALL , BLOCK *);
bool colisaoBolaBlocoEsq(BALL , BLOCK *);
bool colisaoBolaBlocoDir(BALL , BLOCK *);
bool PontoNoBloco(int , int , int , int );

/*Deleta blocos atingidos*/
void DeletaBloco(BLOCK *, int);

/*Muda o level*/
void MudaLevel(BALL *, PADDLE *);

/*Checa recordes e modifica arquivo se necessario*/
void checaRecordes();

/*Le os recordes e mostra na tela RECORDES*/
bool leituraRecordes();

#endif
