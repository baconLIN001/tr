//#include "font.h"
/*BOOL init(char* aCaption,char * aIcon)
{
    //��ʼ�� SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return FALSE;
    }

    //�������ͼ��
    SDL_WM_SetIcon(SDL_LoadBMP(aIcon), NULL);

    //��ʼ������
    gpScreen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE||SDL_HWSURFACE );
    if( gpScreen == NULL )//����Ƿ��ʼ���ɹ�
    {
        return FALSE;
    }
    atexit(cleanup);

    //��ʼ�������
    if( TTF_Init() == -1 )
        return FALSE;

    //���ô������ֺ�ͼ��
    SDL_WM_SetCaption(localeToUTF8(aCaption), NULL );
    return TRUE;
}

char *localeToUTF8(char *src)
{
    static char *buf = NULL;
    wchar_t *unicode_buf;
    int nRetLen;

    if(buf){
        free(buf);
        buf = NULL;
    }
    nRetLen = MultiByteToWideChar(CP_ACP,0,src,-1,NULL,0);
    unicode_buf = (wchar_t*)malloc((nRetLen+1)*sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP,0,src,-1,unicode_buf,nRetLen);
    nRetLen = WideCharToMultiByte(CP_UTF8,0,unicode_buf,-1,NULL,0,NULL,NULL);
    buf = (char*)malloc(nRetLen+1);
    WideCharToMultiByte(CP_UTF8,0,unicode_buf,-1,buf,nRetLen,NULL,NULL);
    free(unicode_buf);
    return buf;
}

wchar_t* cstringToUnicode(char * aSrc)
{
    int size;
    wchar_t *unicodestr = NULL;
    if(!aSrc)
    {
        return NULL;
    }
    size=MultiByteToWideChar(CP_ACP,0,aSrc,-1,NULL,0);
    unicodestr= malloc((size+1)*sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP,0,aSrc,-1,unicodestr,size);
    return unicodestr;
}
