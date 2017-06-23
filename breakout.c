/*
 * NOME: Elizeu Moreira Cappelli
 * DRE: 117053308
 * 06_moving_images.c
 * Este programa move uma imagem pela tela. Ao colidir com uma
 * parede a imagem irá ser refletida.
 * 2016/01/26
 * @adriano cruz
 */

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
 * Constants
 */

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int false = 0;
const int true = 1;

const int IMAGE_WIDTH = 22;
const int IMAGE_HEIGHT = 22;
const int BLOCK_WIDTH = 64;
const int BLOCK_HEIGHT = 32;


typedef struct _NPC
{
    int posX;
    int posY;
    int stepX;
    int stepY;
    SDL_Surface* image;
    int imgW;
    int imgH;
} NPC;

/*
 * Global Variables
 */

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The imagem character
NPC ball;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed PNG image
SDL_Surface* gJPGSurface = NULL;
SDL_Surface* gBlockSurface = NULL;

//The sound effects that will be used
Mix_Chunk *gSoundBot = NULL;
Mix_Chunk *gSoundSide = NULL;
Mix_Chunk *gSoundTop = NULL;

/*
 * function prototypes
 */

//Starts up SDL and creates window
int init();

//Loads media
int loadMedia();

//Frees media and shuts down SDL
void closing();

//Loads individual image
SDL_Surface* loadSurface( char *path );

//Create NPC
NPC createNPC( int posX, int posY, int stepX, int stepY, SDL_Surface *image);

//Move NPC
void moveNPC(NPC *p);


int main( int argc, char* args[] ) {
    SDL_Rect srcRect, dstRect;
    SDL_Rect srcRectBlock, dstRectBlock;
    int i, j;

    //Start up SDL and create window
    if( !init() ) {
        printf( "Failed to initialize!\n" );
    }
    else {
        //Load media
        if( !loadMedia() ) {
            printf( "Failed to load media!\n" );
        }
        else {
            //Create NPC
            ball = createNPC(rand() % (SCREEN_WIDTH - IMAGE_WIDTH),
                             rand() % (SCREEN_HEIGHT - IMAGE_HEIGHT),
                             rand() % 2 ? -1: 1,
                             rand() % 2 ? -1: 1,
                             gJPGSurface);


            //Main loop flag
            int quit = false;

            //Event handler
            SDL_Event e;

            //While application is running
            while( !quit ) {
                while( SDL_PollEvent( &e ) != 0 ) {
                    switch (e.type) {
                        case SDL_QUIT:
                            quit = true;
                            break;
                        case SDL_KEYDOWN:
                            if (e.key.keysym.sym == SDLK_ESCAPE) {
                                quit = true;
                            }
                        break;
                    }
                }

                //Fill the surface black
                SDL_FillRect( gScreenSurface, NULL,
                              SDL_MapRGB( gScreenSurface->format,
                              0x00, 0x00, 0x00 ) );

                moveNPC(&ball);

                srcRect.x = 0; srcRect.y = 0;
                srcRect.w = IMAGE_WIDTH;
                srcRect.h = IMAGE_HEIGHT;
                dstRect.x = ball.posX;
                dstRect.y = ball.posY;

                for (i=0; i < SCREEN_WIDTH; i+=64){
                  for(j=0; j < 4; j++){
                    srcRectBlock.x = 0;
                    srcRectBlock.y = 0;
                    srcRectBlock.w = BLOCK_WIDTH;
                    srcRectBlock.h = BLOCK_HEIGHT;
                    dstRectBlock.x = i;
                    dstRectBlock.y = j;
                    if( SDL_BlitSurface (gBlockSurface, &srcRectBlock, gScreenSurface, &dstRectBlock) < 0 ){
                        printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                        quit = true;
                    }
                  }
                }


                if( SDL_BlitSurface( ball.image, &srcRect,
                                     gScreenSurface, &dstRect ) < 0 ) {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    quit = true;
                }



                //Update the surface
                SDL_UpdateWindowSurface( gWindow );

                // Not so good solution, depends on your computer
                SDL_Delay(5);
            }
        }
    }

    //Free resources and closing SDL
    closing();

    return 0;
}


void moveNPC(NPC *p) {
    p->posX += p->stepX;
    p->posY += p->stepY;

    if ( (p->posX + IMAGE_WIDTH > SCREEN_WIDTH) ||
         (p->posX < 0) ) {
        p->stepX = -p->stepX;
        p->posX += p->stepX;
		    Mix_PlayChannel( -1, gSoundSide, 0 );
    }
    if ( (p->posY + IMAGE_HEIGHT > SCREEN_HEIGHT) ) {
        p->stepY = -p->stepY;
        p->posY += p->stepY;
		    Mix_PlayChannel( -1, gSoundTop, 0 );
    }
	  if ((p->posY < 0) ){
		    p->stepY = -p->stepY;
        p->posY += p->stepY;
		    Mix_PlayChannel( -1, gSoundBot, 0 );
	}
}


//Create NPC
NPC createNPC( int posX, int posY, int stepX, int stepY,
               SDL_Surface *image) {
    NPC p;

    p.posX = posX;
    p.posY = posY;
    p.stepX = stepX;
    p.stepY = stepY;
    p.image = image;
    return p;
}

int init() {
    //Initialization flag
    int success = true;

    srand(time(NULL));

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else {
        //Create window
        gWindow = SDL_CreateWindow( "Breakout - ELR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else{
            //Initialize JPG and PNG loading
            int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else {
                //Get window surface
                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
            //Initialize SDL_mixer
			if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) {
				printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
				success = false;
			}

        }
    }



    return success;
}

int loadMedia() {
    //Loading success flag
    int success = true;
    //uint32_t colorKey;

    //Load PNG surface
    gJPGSurface = loadSurface( "./ballGrey.png" );
    if( gJPGSurface == NULL ) {
      printf( "Failed to load image! SDL Error: %s\n", SDL_GetError() );
      success = false;
    }
    gBlockSurface = loadSurface( "./block.png" );
    if ( gBlockSurface == NULL ) {
      printf( "Failed to load image! SDL Error: %s\n", SDL_GetError() );
      success = false;
    }

	  //Load sound effects
    gSoundBot = Mix_LoadWAV ( "./bambam.wav" );
	  if ( gSoundBot == NULL ){
	    printf("Failed to load bot sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	    success = false;
	  }

    gSoundSide = Mix_LoadWAV ( "./miado.wav" );
	  if ( gSoundSide == NULL){
	    printf( "Failed to load side sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		  success = false;
	  }
    gSoundTop = Mix_LoadWAV ( "./aiai.wav" );
	  if ( gSoundTop == NULL ){
		  printf( "Failed to load top sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		  success = false;
	  }

    return success;
}

void closing() {
    //Free loaded image
    SDL_FreeSurface( gJPGSurface );
    SDL_FreeSurface( gBlockSurface );
    gJPGSurface = NULL;
    gBlockSurface = NULL;

	//Free the sound effects
	 Mix_FreeChunk( gSoundBot );
	 Mix_FreeChunk( gSoundSide );
	 Mix_FreeChunk( gSoundTop );
	 gSoundBot = NULL;
	 gSoundSide = NULL;
	 gSoundTop = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
	  Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* loadSurface( char *path ) {
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimizedSurface == NULL ) {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}
