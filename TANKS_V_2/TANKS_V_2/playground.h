#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <cmath>
#include <vector>
#include <ctime>

bool                            keys[4]               = {false,false,false,false};
bool                            redraw                = true;
int                             FPS                   = 60;
int                             cursor_x              = NULL;
int                             cursor_y              = NULL;
ALLEGRO_EVENT_QUEUE            *event_queue           = NULL;
ALLEGRO_TIMER                  *timer1                = NULL;
ALLEGRO_SAMPLE                 *back_music            = NULL;
ALLEGRO_SAMPLE                 *tank_driwe            = NULL;	
ALLEGRO_SAMPLE                 *tank_wait             = NULL; 	
ALLEGRO_SAMPLE                 *tank_shot             = NULL;
ALLEGRO_SAMPLE                 *tank_reload           = NULL;
ALLEGRO_SAMPLE_INSTANCE        *driwe_inst            = NULL;
ALLEGRO_SAMPLE_INSTANCE        *wait_inst             = NULL;
ALLEGRO_BITMAP                 *user_body             = NULL;
ALLEGRO_BITMAP                 *user_turret           = NULL;
ALLEGRO_BITMAP                 *enemy_body            = NULL;
ALLEGRO_BITMAP                 *enemy_turret          = NULL;
extern ALLEGRO_DISPLAY         *t_display;
extern int                      DIFFICULT;

struct TANK {
	int   lives;
	int   direction;
	int   distance;
	int   id;
	float x;
	float y;
	float body_angle;
	float turret_angle;
	float tank_width;
	float tank_height;
	bool  active;
};

std::vector<TANK> enemy_tanks;
TANK user_tank;
TANK enemy_tank;
const int  tank_speed = 1;

void init_run();
int  t_run();
void t_driwe(TANK &tank, bool keys[4]);
void t_draiw_A_I(TANK &tank);
void t_angles(TANK &tank, int cursor_x, int cursor_y, bool keys[4]);
void t_A_I_angles(TANK &tank,int cursor_x, int cursor_y);
void A_I_action();
int  get_new_tank_position();
void change_d_d(TANK &tank);
bool free_way(TANK &tank);

int random_btwn( const int min, const int max);