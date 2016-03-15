/*#ifndef FONT_H_2012_05_31
#define FONT_H_2012_05_31
#include <SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <Windows.h>

//���岼������
typedef  int BOOL;
#define TRUE  1
#define FALSE 0

//��Ļ�ֱ���
#define  SCREEN_WIDTH  718
#define  SCREEN_HEIGHT  419
#define  SCREEN_BPP  32

//��������
extern SDL_Surface *gpBackground; //��������
extern SDL_Surface *gpScreen; //��ʾ����
extern SDL_Surface *gpMessage[3]; //�������ֱ���
extern SDL_Surface *gpChinese;//�������ֱ���

//�¼�����
extern SDL_Event myEvent;

// ��������
extern TTF_Font *font;

extern const SDL_Color RGB_Black;
extern const SDL_Color RGB_Red;
extern const SDL_Color RGB_Green;
extern const SDL_Color RGB_Blue;
extern const SDL_Color RGB_Cyan;
extern const SDL_Color RGB_Magenta;
extern const SDL_Color RGB_Yellow;
extern const SDL_Color RGB_White;
extern const SDL_Color RGB_Gray;
extern const SDL_Color RGB_Grey;
extern const SDL_Color RGB_Maroon ;
extern const SDL_Color RGB_Darkgreen;
extern const SDL_Color RGB_Navy;
extern const SDL_Color RGB_Teal;
extern const SDL_Color RGB_Purple;
extern const SDL_Color RGB_Olive;
extern const SDL_Color RGB_Noname;


BOOL init(char* aCaption,char * aIcon);
SDL_Surface *loadImage( char * filename );
void applySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
void cleanup();
char *localeToUTF8(char *src);
wchar_t* cstringToUnicode(char * aSrc);

#endif
