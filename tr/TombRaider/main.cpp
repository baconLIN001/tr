//
//  main.cpp
//  TombRaider
//
//  Created by 張正昊 on 12/2/2016.
//  Copyright © 2016 hahaschool. All rights reserved.
//

#include <iostream>
#include <SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "TRGameController.hpp"
#include "TRTimer.hpp"
#include "TRBgm.h"
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
TRPathFinder *gPathFinder = NULL;
TRGrider *gGrider = NULL;
TRGameController *gGameController = NULL;
TRBgm *gBgm = NULL;

bool init(){
    //初始化成功标志
    bool success = true;

    gGameController = new TRGameController;

    gGrider = new TRGrider;

    gPathFinder = new TRPathFinder;



    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Error: SDL_Init Failed. SDL Error: %s\n",SDL_GetError());
        success = false;
    }else if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0){
        printf("SDL_mixer could not initialize!SDL_mixer Error:%s\n",Mix_GetError());
        success = false;
    }else if(TTF_Init()==-1)
    {
        return -1;
    }
    else{
        //线性贴图过滤
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        //创建窗口
        gWindow = SDL_CreateWindow("Tomb Raider", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL){
            printf("Error:Window creation failed. SDL_Error: %s\n",SDL_GetError());
            success = false;
        }else{
            //垂直同步
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(gRenderer == NULL){
                printf("Error:Render Creation Failed.SDL_Error : %s\n",SDL_GetError());
                success = false;
            }else{
                //设定渲染器默认颜色
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                //初始化PNG载入
                int imgFlag = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlag) & imgFlag)){
                    printf("Error:SDL_image init failed.SDL_image Error: %s\n",IMG_GetError());
                    success = false;
                }
            }
        }
    }

    gGameController -> linkRenderer(gRenderer);
    gGameController -> setCamera(SCREEN_HEIGHT, SCREEN_WIDTH);
    gGameController -> linkGrider(gGrider);
    gGameController -> linkPathfinder(gPathFinder);
    gGameController -> loadResources();

    gGrider -> tileHeight = 50;
    gGrider -> tileWidth = 50;

    gPathFinder -> setHeuristic(TRPathFinderHeuristicEuler);

    return success;
}

void close(){
    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystem
    IMG_Quit();
    SDL_Quit();
}

void apply_surface(int x,int y,SDL_Surface* source,SDL_Surface *destination)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(source,NULL,destination,&offset);
}

int main(int argc, char * argv[]) {
    //srand(time(NULL));
    if (!init()) {
        printf("Error:Failed to load.\n");
    }else{
        bool quit = false;
        SDL_Event e;

        //dubug
        //gGameController->createHp("blood",5,5,5,62);
        gGameController->loadMapFromFile("Resources/Mapdata/test.map");
        gGameController -> createHero("hahaschool", 50, 50, 48, 32);
        for(int i = 1; i <= 10; i++){
            for(int j = 1; j<= 5; j++){
                gGameController -> createEnemy("bat", 10*i+50, 10*j+50, 50, 50);
            }
        }
        gBgm->playMusic("Resources/Bgm/mummy_begin.wav",true);
        for(int i = 1;i <= 5; i++)
        {
            gGameController -> createEnemy("mummy",10*i+85,1000,50,50);
        }
        //
        TTF_Font *font;
        font = TTF_OpenFont("Resources/MTCORSVA.ttf",16);
        if(!font)
        {
            printf("TTF_OpenFont:Open MTCORSVA.ttf%s/n",TTF_GetError());
            return -1;
        }


        SDL_Surface *screen;
        SDL_Color black = {255,255,255};
        SDL_Color red = {255,0,0};
        SDL_Surface *message = TTF_RenderText_Solid(font,"I can do it",red);
        if(message)
        {
            apply_surface(0,0,message,screen);
        }


        SDL_FreeSurface(message);
        TTF_CloseFont(font);
        TTF_Quit();


        //
        gGameController -> startGame();

        //The frames per second timer
        TRTimer fpsTimer;

        //The frames per second cap timer
        TRTimer capTimer;

        //Start counting frames per second
        int countedFrames = 0;
        fpsTimer.start();

        while(!quit){
            //Start cap timer
            capTimer.start();
            if(gGameController -> isGameRunning() == false){
                quit = true;
            }
            gGameController -> handleEvent(e);
            //Calculate and correct fps
            float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
            if( avgFPS > 2000000 )
            {
                avgFPS = 0;
            }


            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );
            gGameController -> runFrame();
            SDL_RenderPresent( gRenderer );

            //If frame finished early
            int frameTicks = capTimer.getTicks();
            if( frameTicks < SCREEN_TICK_PER_FRAME )
            {
                //Wait remaining time
                SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
            }
        }
    }
    close();
    return 0;
}

/*

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "font.h"

int main(int argc,char * argv[])
{
    char a[] = "Hello World!";
    int quit = 0;
    wchar_t * p ;
    char tmp[]="世界，你好！";
    wchar_t msg[] =L"圣    旨";

   if(!init("文字","icon.bmp"))
   {
       exit(0);
   }

   //打开simfang.ttf 字库，设字体为20号
    font  = TTF_OpenFont("simhei.ttf",20);
    if(font == NULL)
    {
       fprintf(stderr,"font open failure %s\n",SDL_GetError());
       exit(-1);
    }

    gpBackground =  loadImage("background.jpg");
    applySurface(0,0,gpBackground,gpScreen);

    //TTF_SetFontStyle(font,TTF_STYLE_BOLD |  TTF_STYLE_ITALIC);

    //显示西文
    gpMessage[0] = TTF_RenderText_Solid(font,a,RGB_Black);
    gpMessage[1] = TTF_RenderText_Shaded(font,a,RGB_Black,RGB_White);
    gpMessage[2] = TTF_RenderText_Blended(font,a,RGB_Black);
    applySurface(80,120,gpMessage[0],gpScreen);
    applySurface(80,150,gpMessage[1],gpScreen);
    applySurface(80,180,gpMessage[2],gpScreen);
    SDL_Flip(gpScreen);


    //显示中文
    p = cstringToUnicode(tmp);
    gpChinese = TTF_RenderUNICODE_Solid( font, p, RGB_Black);
    applySurface(340,120,gpChinese,gpScreen);
    SDL_FreeSurface(gpChinese);
    gpChinese = TTF_RenderUNICODE_Shaded( font, p, RGB_Black,RGB_White);
    applySurface(340,150,gpChinese,gpScreen);
    SDL_FreeSurface(gpChinese);
    gpChinese = TTF_RenderUNICODE_Blended( font, p, RGB_Black);
    applySurface(340,180,gpChinese,gpScreen);
    SDL_FreeSurface(gpChinese);
    SDL_Flip(gpScreen);
    free(p);
    p = NULL;

    TTF_CloseFont(font);

    //重新打开simfang.ttf 字库，设字体为40
    font  = TTF_OpenFont("simhei.ttf",40);
    if(font == NULL)
    {
        fprintf(stderr,"font open failure %s\n",SDL_GetError());
        exit(-1);
    }
    gpChinese = TTF_RenderUNICODE_Solid( font, msg, RGB_Yellow);
    applySurface(260,50,gpChinese,gpScreen);
    SDL_Flip(gpScreen);
    SDL_FreeSurface(gpChinese);

    gpChinese = TTF_RenderUNICODE_Solid( font, L"C语言始终被模仿，从未被超越！", RGB_Red);
    applySurface(100,240,gpChinese,gpScreen);
    SDL_FreeSurface(gpChinese);

    gpChinese = TTF_RenderUNICODE_Solid( font, L"不懂C语言不要说自己是程序员！", RGB_Red);
    applySurface(100,300,gpChinese,gpScreen);
    SDL_FreeSurface(gpChinese);
    SDL_Flip(gpScreen);

   // 事件处理
   while(!quit)
   {
       if (SDL_PollEvent(&myEvent))
       {
           if (myEvent.type==SDL_QUIT)
           {
               quit = 1;
           }
       }
    }
    return 0;
}
*/

/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, math, strings, and string streams
/*
#include <SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

		//Pixel manipulators
		bool lockTexture();
		bool unlockTexture();
		void* getPixels();
		int getPitch();
		Uint32 getPixel32( unsigned int x, unsigned int y );

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		void* mPixels;
		int mPitch;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Our bitmap font
class LBitmapFont
{
    public:
		//The default constructor
		LBitmapFont();

		//Generates the font
		bool buildFont( LTexture *bitmap );

		//Shows the text
		void renderText( int x, int y, std::string text );

    private:
		//The font texture
		LTexture* mBitmap;

		//The individual characters in the surface
		SDL_Rect mChars[ 256 ];

		//Spacing Variables
		int mNewLine, mSpace;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gBitmapTexture;
LBitmapFont gBitmapFont;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Convert surface to display format
		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface, SDL_PIXELFORMAT_RGBA8888, NULL );
		if( formattedSurface == NULL )
		{
			printf( "Unable to convert loaded surface to display format! %s\n", SDL_GetError() );
		}
		else
		{
			//Create blank streamable texture
			newTexture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
			if( newTexture == NULL )
			{
				printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				//Enable blending on texture
				SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );

				//Lock texture for manipulation
				SDL_LockTexture( newTexture, &formattedSurface->clip_rect, &mPixels, &mPitch );

				//Copy loaded/formatted surface pixels
				memcpy( mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );

				//Get image dimensions
				mWidth = formattedSurface->w;
				mHeight = formattedSurface->h;

				//Get pixel data in editable format
				Uint32* pixels = (Uint32*)mPixels;
				int pixelCount = ( mPitch / 4 ) * mHeight;

				//Map colors
				Uint32 colorKey = SDL_MapRGB( formattedSurface->format, 0, 0xFF, 0xFF );
				Uint32 transparent = SDL_MapRGBA( formattedSurface->format, 0x00, 0xFF, 0xFF, 0x00 );

				//Color key pixels
				for( int i = 0; i < pixelCount; ++i )
				{
					if( pixels[ i ] == colorKey )
					{
						pixels[ i ] = transparent;
					}
				}

				//Unlock texture to update
				SDL_UnlockTexture( newTexture );
				mPixels = NULL;
			}

			//Get rid of old formatted surface
			SDL_FreeSurface( formattedSurface );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		mPixels = NULL;
		mPitch = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool LTexture::lockTexture()
{
	bool success = true;

	//Texture is already locked
	if( mPixels != NULL )
	{
		printf( "Texture is already locked!\n" );
		success = false;
	}
	//Lock texture
	else
	{
		if( SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch ) != 0 )
		{
			printf( "Unable to lock texture! %s\n", SDL_GetError() );
			success = false;
		}
	}

	return success;
}

bool LTexture::unlockTexture()
{
	bool success = true;

	//Texture is not locked
	if( mPixels == NULL )
	{
		printf( "Texture is not locked!\n" );
		success = false;
	}
	//Unlock texture
	else
	{
		SDL_UnlockTexture( mTexture );
		mPixels = NULL;
		mPitch = 0;
	}

	return success;
}

void* LTexture::getPixels()
{
	return mPixels;
}

int LTexture::getPitch()
{
	return mPitch;
}

Uint32 LTexture::getPixel32( unsigned int x, unsigned int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32*)mPixels;

    //Get the pixel requested
    return pixels[ ( y * ( mPitch / 4 ) ) + x ];
}

LBitmapFont::LBitmapFont()
{
    //Initialize variables
    mBitmap = NULL;
    mNewLine = 0;
    mSpace = 0;
}

bool LBitmapFont::buildFont( LTexture* bitmap )
{
	bool success = true;

	//Lock pixels for access
	if( !bitmap->lockTexture() )
	{
		printf( "Unable to lock bitmap font texture!\n" );
		success = false;
	}
	else
	{
		//Set the background color
		Uint32 bgColor = bitmap->getPixel32( 0, 0 );

		//Set the cell dimensions
		int cellW = bitmap->getWidth() / 16;
		int cellH = bitmap->getHeight() / 16;

		//New line variables
		int top = cellH;
		int baseA = cellH;

		//The current character we're setting
		int currentChar = 0;

		//Go through the cell rows
		for( int rows = 0; rows < 16; ++rows )
		{
			//Go through the cell columns
			for( int cols = 0; cols < 16; ++cols )
			{
				//Set the character offset
				mChars[ currentChar ].x = cellW * cols;
				mChars[ currentChar ].y = cellH * rows;

				//Set the dimensions of the character
				mChars[ currentChar ].w = cellW;
				mChars[ currentChar ].h = cellH;

				//Find Left Side
				//Go through pixel columns
				for( int pCol = 0; pCol < cellW; ++pCol )
				{
					//Go through pixel rows
					for( int pRow = 0; pRow < cellH; ++pRow )
					{
						//Get the pixel offsets
						int pX = ( cellW * cols ) + pCol;
						int pY = ( cellH * rows ) + pRow;

						//If a non colorkey pixel is found
						if( bitmap->getPixel32( pX, pY ) != bgColor )
						{
							//Set the x offset
							mChars[ currentChar ].x = pX;

							//Break the loops
							pCol = cellW;
							pRow = cellH;
						}
					}
				}

				//Find Right Side
				//Go through pixel columns
				for( int pColW = cellW - 1; pColW >= 0; --pColW )
				{
					//Go through pixel rows
					for( int pRowW = 0; pRowW < cellH; ++pRowW )
					{
						//Get the pixel offsets
						int pX = ( cellW * cols ) + pColW;
						int pY = ( cellH * rows ) + pRowW;

						//If a non colorkey pixel is found
						if( bitmap->getPixel32( pX, pY ) != bgColor )
						{
							//Set the width
							mChars[ currentChar ].w = ( pX - mChars[ currentChar ].x ) + 1;

							//Break the loops
							pColW = -1;
							pRowW = cellH;
						}
					}
				}

				//Find Top
				//Go through pixel rows
				for( int pRow = 0; pRow < cellH; ++pRow )
				{
					//Go through pixel columns
					for( int pCol = 0; pCol < cellW; ++pCol )
					{
						//Get the pixel offsets
						int pX = ( cellW * cols ) + pCol;
						int pY = ( cellH * rows ) + pRow;

						//If a non colorkey pixel is found
						if( bitmap->getPixel32( pX, pY ) != bgColor )
						{
							//If new top is found
							if( pRow < top )
							{
								top = pRow;
							}

							//Break the loops
							pCol = cellW;
							pRow = cellH;
						}
					}
				}

				//Find Bottom of A
				if( currentChar == 'A' )
				{
					//Go through pixel rows
					for( int pRow = cellH - 1; pRow >= 0; --pRow )
					{
						//Go through pixel columns
						for( int pCol = 0; pCol < cellW; ++pCol )
						{
							//Get the pixel offsets
							int pX = ( cellW * cols ) + pCol;
							int pY = ( cellH * rows ) + pRow;

							//If a non colorkey pixel is found
							if( bitmap->getPixel32( pX, pY ) != bgColor )
							{
								//Bottom of a is found
								baseA = pRow;

								//Break the loops
								pCol = cellW;
								pRow = -1;
							}
						}
					}
				}

				//Go to the next character
				++currentChar;
			}
		}

		//Calculate space
		mSpace = cellW / 2;

		//Calculate new line
		mNewLine = baseA - top;

		//Lop off excess top pixels
		for( int i = 0; i < 256; ++i )
		{
			mChars[ i ].y += top;
			mChars[ i ].h -= top;
		}

		bitmap->unlockTexture();
		mBitmap = bitmap;
	}

	return success;
}

void LBitmapFont::renderText( int x, int y, std::string text )
{
    //If the font has been built
    if( mBitmap != NULL )
    {
		//Temp offsets
		int curX = x, curY = y;

        //Go through the text
        for( int i = 0; i < text.length(); ++i )
        {
            //If the current character is a space
            if( text[ i ] == ' ' )
            {
                //Move over
                curX += mSpace;
            }
            //If the current character is a newline
            else if( text[ i ] == '\n' )
            {
                //Move down
                curY += mNewLine;

                //Move back
                curX = x;
            }
            else
            {
                //Get the ASCII value of the character
                int ascii = (unsigned char)text[ i ];

                //Show the character
				mBitmap->render( curX, curY, &mChars[ ascii ] );

                //Move over the width of the character with one pixel of padding
                curX += mChars[ ascii ].w + 1;
            }
        }
    }
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Seed random
		srand( SDL_GetTicks() );

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load font texture
	if( !gBitmapTexture.loadFromFile( "lazyfont.png" ) )
	{
		printf( "Failed to load corner texture!\n" );
		success = false;
	}
	else
	{
		//Build font from texture
		gBitmapFont.buildFont( &gBitmapTexture );
	}

	return success;
}

void close()
{
	//Free loaded images
	gBitmapTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render test text
				gBitmapFont.renderText( 0, 0, "Bitmap Font:\nABDCEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n0123456789" );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
*/