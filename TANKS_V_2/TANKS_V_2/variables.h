#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
//---Defines--------------------------------
#define  UP                0
#define  DOWN              1
#define  LEFT              2
#define  RIGHT             3
#define  T_NEW             4
#define  T_MENU            5
#define  T_LOAD            6 
#define  T_SETTINGS        7
#define  T_RUN             8
#define  T_EXIT            9
#define  EASY              3
#define  NORMAL            5
#define  HARD              7
#define  T_WINDOW_WIDTH    1366
#define  T_WINDOW_HEIGHT   768
#define  D_UP              0
#define  D_DOWN            1
#define  D_LEFT            2
#define  D_RIGHT           3
#define  USER_TANK         18
//------------------------------------------
typedef struct {
	int x;
	int y;
}CURSOR_POSITION;