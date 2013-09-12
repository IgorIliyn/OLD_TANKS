#include "variables.h"

ALLEGRO_DISPLAY                *t_display             = NULL;
ALLEGRO_FONT                   *t_menu_font_normal    = NULL;
ALLEGRO_FONT                   *t_menu_font_light     = NULL;
ALLEGRO_EVENT_QUEUE            *t_event_queue         = NULL;
ALLEGRO_TIMER                  *t_timer               = NULL;
ALLEGRO_BITMAP                 *t_bitmap_logo         = NULL;
ALLEGRO_BITMAP                 *t_bitmap_tank         = NULL;
CURSOR_POSITION                 t_cursor_position;
ALLEGRO_SAMPLE                 *t_menu_sound          = NULL;
ALLEGRO_SAMPLE_INSTANCE        *t_menu_sound_instance = NULL; 
int                             t_x_menu_position     = NULL;
int                             GAME_STATE            = T_MENU;
int                             t_bitmap_tank_width   = NULL;
int                             t_bitmap_tank_height  = NULL;
int                             t_FPS                 = 60; 
int                             DIFFICULT             = NORMAL;
bool                            t_redraw              = true;
bool                            user_exit             = true;


void t_init_main();
int  t_menu();
int  t_new();
int  t_load();
int  t_settings();
int  t_run();
int  t_exit();

int main()
{
	t_init_main();
	while(user_exit)
	{
		al_play_sample_instance(t_menu_sound_instance);
		switch(GAME_STATE)
		{
		case T_MENU:
			GAME_STATE = t_menu();
			break;
		case T_NEW:
			GAME_STATE = t_new();
			break;
		case T_LOAD:
			GAME_STATE = t_load();
			break;
		case T_SETTINGS:
			GAME_STATE = t_settings();
			break;
		case T_RUN:
			al_stop_sample_instance(t_menu_sound_instance);
			GAME_STATE = t_run();
			break;
		case  T_EXIT:
			GAME_STATE = t_exit();
			if(GAME_STATE == T_EXIT)
			{
				al_destroy_sample(t_menu_sound);
				al_destroy_sample_instance(t_menu_sound_instance);
				al_destroy_display(t_display);
				user_exit = false;
			}
			break;
		}
	}
	return 0;
}

void t_init_main()
{
	//---Music initialization--------------------------------------------------------------------------------------------------------
	if (!al_init())//Error check
	{
		al_show_native_message_box(NULL,"Tanks","Error al_init!!!","Cant initialization allegro 5!!!",NULL,ALLEGRO_MESSAGEBOX_ERROR);
	}
	//---Initialization devices--------
	al_init_acodec_addon();
	al_install_audio();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();
	//--------------------------------

	//---Assignments---------------------------------------------- 
	al_reserve_samples(2);
	t_menu_sound  = al_load_sample("Source/SP0000.WAV");
	t_menu_sound_instance = al_create_sample_instance(t_menu_sound);
	al_set_sample_instance_playmode(t_menu_sound_instance,ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(t_menu_sound_instance,0.7);
	al_attach_sample_instance_to_mixer(t_menu_sound_instance,al_get_default_mixer());
	al_play_sample_instance(t_menu_sound_instance);

	//---Display initialization-------------------------------------------------------------------------------------------------------------
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	t_display = al_create_display(T_WINDOW_WIDTH,T_WINDOW_HEIGHT);
	al_set_window_position(t_display,0,0);
	al_set_window_title(t_display,"Tanks");
	if (!t_display)//Error check
	{
		al_show_native_message_box(NULL,"Tanks","Error al_create_display","Cant create display allegro 5!!!",NULL,ALLEGRO_MESSAGEBOX_ERROR);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
}

int t_menu()
{
	t_x_menu_position = 200;
	if (!al_init())//Error check
	{
		al_show_native_message_box(NULL,"Tanks","Error al_init!!!","Cant initialization allegro 5!!!",NULL,ALLEGRO_MESSAGEBOX_ERROR);
	}

	//---Assignments---------------------------------------------- 
	t_menu_font_normal = al_load_font("Source/Micra Bold.ttf",15,NULL);
	t_menu_font_light  = al_load_font("Source/Micra Bold.ttf",18,NULL);
	t_event_queue = al_create_event_queue();
	t_timer       = al_create_timer(1.0 / t_FPS);
	t_bitmap_logo = al_load_bitmap("Source/IOGS.png");
	t_bitmap_tank = al_load_bitmap("Source/Tygr_Model_800.jpg");
	t_bitmap_tank_width  = al_get_bitmap_width(t_bitmap_tank);
	t_bitmap_tank_height = al_get_bitmap_height(t_bitmap_tank);


	//---Registration events--------------------------------------------------------
	al_register_event_source(t_event_queue,al_get_mouse_event_source());
	al_register_event_source(t_event_queue,al_get_keyboard_event_source());
	al_register_event_source(t_event_queue,al_get_display_event_source(t_display));
	al_register_event_source(t_event_queue,al_get_timer_event_source(t_timer));
	//------------------------------------------------------------------------------

	al_start_timer(t_timer);
	bool user_cycle = true;
	int LOCAL_STATE_GAME = NULL;
	//---Main user cycle-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while(user_cycle)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(t_event_queue,&event);

		//---Catch exit action-------------------------------
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			LOCAL_STATE_GAME = T_EXIT;
			user_cycle = false;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				LOCAL_STATE_GAME = T_EXIT;
				user_cycle = false;
			}
		}

		//---Catch mouse action------------------------------------------------------------------------------------------------------------------------------------------------------------------
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"new game")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 200) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 200) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_NEW;
				user_cycle = false;
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"load game")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 170) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 170) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_LOAD;
				user_cycle = false;
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"settings")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_SETTINGS;
				user_cycle = false;
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"exit game")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 110) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 110) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_EXIT;
				user_cycle = false;
			}
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			t_redraw = true;
		}

		//---Draw menu-----------------------------------------------------------------------------------------------------------
		if (t_redraw)
		{
			t_redraw = false;
			al_get_mouse_cursor_position(&t_cursor_position.x,&t_cursor_position.y);
			//al_draw_textf(t_menu_font_normal,al_map_rgb(255,0,0),50,50,ALLEGRO_ALIGN_LEFT,"X = %i Y = %i",t_cursor_position.x,t_cursor_position.y);

			al_draw_bitmap(t_bitmap_logo,20,T_WINDOW_HEIGHT - 190,NULL);
			al_draw_bitmap(t_bitmap_tank,T_WINDOW_WIDTH / 2 - t_bitmap_tank_width / 2,10,NULL);


			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"new game")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 200) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 200) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 200,ALLEGRO_ALIGN_LEFT,"new game");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 200,ALLEGRO_ALIGN_LEFT,"new game");
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"load game")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 170) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 170) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 170,ALLEGRO_ALIGN_LEFT,"load game");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 170,ALLEGRO_ALIGN_LEFT,"load game");
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"settings")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"settings");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"settings");
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"exit game")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 110) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 110) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 110,ALLEGRO_ALIGN_LEFT,"exit game");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 110,ALLEGRO_ALIGN_LEFT,"exit game");			

			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	}
	//---Deallocation memory--------------
	al_destroy_font(t_menu_font_normal);
	al_destroy_font(t_menu_font_light);
	al_destroy_event_queue(t_event_queue);
	al_destroy_timer(t_timer);
	al_destroy_bitmap(t_bitmap_tank);
	al_destroy_bitmap(t_bitmap_logo);
	//------------------------------------

	return LOCAL_STATE_GAME;
}

int t_exit()
{
	t_x_menu_position = 200;
	if (!al_init())//Error check
	{
		al_show_native_message_box(NULL,"Tanks","Error al_init!!!","Cant initialization allegro 5!!!",NULL,ALLEGRO_MESSAGEBOX_ERROR);
	}

	//---Assignments---------------------------------------------- 
	t_menu_font_normal = al_load_font("Source/Micra Bold.ttf",15,NULL);
	t_menu_font_light  = al_load_font("Source/Micra Bold.ttf",18,NULL);
	t_event_queue = al_create_event_queue();
	t_timer       = al_create_timer(1.0 / t_FPS);
	t_bitmap_logo = al_load_bitmap("Source/IOGS.png");
	t_bitmap_tank = al_load_bitmap("Source/Tygr_Model_800.jpg");
	t_bitmap_tank_width  = al_get_bitmap_width(t_bitmap_tank);
	t_bitmap_tank_height = al_get_bitmap_height(t_bitmap_tank);


	//---Registration events--------------------------------------------------------
	al_register_event_source(t_event_queue,al_get_mouse_event_source());
	al_register_event_source(t_event_queue,al_get_keyboard_event_source());
	al_register_event_source(t_event_queue,al_get_display_event_source(t_display));
	al_register_event_source(t_event_queue,al_get_timer_event_source(t_timer));
	//------------------------------------------------------------------------------

	al_start_timer(t_timer);
	bool user_cycle = true;
	int LOCAL_STATE_GAME = NULL;
	//---Main user cycle-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while(user_cycle)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(t_event_queue,&event);

		//---Catch exit action-------------------------------
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			LOCAL_STATE_GAME = T_EXIT;
			user_cycle = false;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				LOCAL_STATE_GAME = T_EXIT;
				user_cycle = false;
			}
		}
		//---------------------------------------------------

		//---Catch mouse action------------------------------------------------------------------------------------------------------------------------------------------------------------------
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"YES")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_EXIT;
				user_cycle = false;
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position + 100) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position + 100) + al_get_text_width(t_menu_font_normal,"NO")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_MENU;
				user_cycle = false;
			}
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			t_redraw = true;
		}

		//---Draw menu-----------------------------------------------------------------------------------------------------------
		if (t_redraw)
		{
			t_redraw = false;
			al_get_mouse_cursor_position(&t_cursor_position.x,&t_cursor_position.y);
			//al_draw_textf(t_menu_font_normal,al_map_rgb(255,0,0),50,50,ALLEGRO_ALIGN_LEFT,"X = %i Y = %i",t_cursor_position.x,t_cursor_position.y);

			al_draw_bitmap(t_bitmap_logo,20,T_WINDOW_HEIGHT - 190,NULL);
			al_draw_bitmap(t_bitmap_tank,T_WINDOW_WIDTH / 2 - t_bitmap_tank_width / 2,10,NULL);

			al_draw_text(t_menu_font_light,al_map_rgb(150,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 200,ALLEGRO_ALIGN_LEFT,"exit game?");

			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"YES")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"YES");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"YES");
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position + 100) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position + 100) + al_get_text_width(t_menu_font_normal,"NO")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position + 100,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"NO");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position + 100,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"NO");			

			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	}
	//---Deallocation memory--------------
	al_destroy_font(t_menu_font_normal);
	al_destroy_font(t_menu_font_light);
	al_destroy_event_queue(t_event_queue);
	al_destroy_timer(t_timer);
	al_destroy_bitmap(t_bitmap_tank);
	al_destroy_bitmap(t_bitmap_logo);
	//------------------------------------

	return LOCAL_STATE_GAME;
}

int t_load()
{
	t_x_menu_position = 200;
	if (!al_init())//Error check
	{
		al_show_native_message_box(NULL,"Tanks","Error al_init!!!","Cant initialization allegro 5!!!",NULL,ALLEGRO_MESSAGEBOX_ERROR);
	}

	//---Assignments---------------------------------------------- 
	t_menu_font_normal = al_load_font("Source/Micra Bold.ttf",15,NULL);
	t_menu_font_light  = al_load_font("Source/Micra Bold.ttf",18,NULL);
	t_event_queue = al_create_event_queue();
	t_timer       = al_create_timer(1.0 / t_FPS);
	t_bitmap_logo = al_load_bitmap("Source/IOGS.png");
	t_bitmap_tank = al_load_bitmap("Source/Tygr_Model_800.jpg");
	t_bitmap_tank_width  = al_get_bitmap_width(t_bitmap_tank);
	t_bitmap_tank_height = al_get_bitmap_height(t_bitmap_tank);


	//---Registration events--------------------------------------------------------
	al_register_event_source(t_event_queue,al_get_mouse_event_source());
	al_register_event_source(t_event_queue,al_get_keyboard_event_source());
	al_register_event_source(t_event_queue,al_get_display_event_source(t_display));
	al_register_event_source(t_event_queue,al_get_timer_event_source(t_timer));
	//------------------------------------------------------------------------------

	al_start_timer(t_timer);
	bool user_cycle = true;
	int LOCAL_STATE_GAME = NULL;
	//---Main user cycle-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while(user_cycle)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(t_event_queue,&event);

		//---Catch exit action-------------------------------
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			LOCAL_STATE_GAME = T_EXIT;
			user_cycle = false;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				LOCAL_STATE_GAME = T_MENU;
				user_cycle = false;
			}
		}
		//---------------------------------------------------

		//---Catch mouse action------------------------------------------------------------------------------------------------------------------------------------------------------------------
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			/*if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"load game")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 170) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 170) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_LOAD;
				user_cycle = false;
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"settings")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_SETTINGS;
				user_cycle = false;
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"exit game")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 110) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 110) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_EXIT;
				user_cycle = false;
			}*/
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			t_redraw = true;
		}

		//---Draw menu-----------------------------------------------------------------------------------------------------------
		if (t_redraw)
		{
			t_redraw = false;
			al_get_mouse_cursor_position(&t_cursor_position.x,&t_cursor_position.y);
			//al_draw_textf(t_menu_font_normal,al_map_rgb(255,0,0),50,50,ALLEGRO_ALIGN_LEFT,"X = %i Y = %i",t_cursor_position.x,t_cursor_position.y);

			al_draw_bitmap(t_bitmap_logo,20,T_WINDOW_HEIGHT - 190,NULL);
			al_draw_bitmap(t_bitmap_tank,T_WINDOW_WIDTH / 2 - t_bitmap_tank_width / 2,10,NULL);

			al_draw_text(t_menu_font_light,al_map_rgb(255,150,0),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 200,ALLEGRO_ALIGN_LEFT,"stages");
			
			/*if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"load game")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 170) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 170) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 170,ALLEGRO_ALIGN_LEFT,"load game");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 170,ALLEGRO_ALIGN_LEFT,"load game");
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"settings")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"settings");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"settings");
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"exit game")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 110) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 110) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 110,ALLEGRO_ALIGN_LEFT,"exit game");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 110,ALLEGRO_ALIGN_LEFT,"exit game");			

			}*/
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	}
	//---Deallocation memory--------------
	al_destroy_font(t_menu_font_normal);
	al_destroy_font(t_menu_font_light);
	al_destroy_event_queue(t_event_queue);
	al_destroy_timer(t_timer);
	al_destroy_bitmap(t_bitmap_tank);
	al_destroy_bitmap(t_bitmap_logo);
	//------------------------------------

	return LOCAL_STATE_GAME;
}

int t_new()
{
	t_x_menu_position = 300;
	if (!al_init())//Error check
	{
		al_show_native_message_box(NULL,"Tanks","Error al_init!!!","Cant initialization allegro 5!!!",NULL,ALLEGRO_MESSAGEBOX_ERROR);
	}

	//---Assignments---------------------------------------------- 
	t_menu_font_normal = al_load_font("Source/Micra Bold.ttf",15,NULL);
	t_menu_font_light  = al_load_font("Source/Micra Bold.ttf",18,NULL);
	t_event_queue = al_create_event_queue();
	t_timer       = al_create_timer(1.0 / t_FPS);
	t_bitmap_logo = al_load_bitmap("Source/IOGS.png");
	t_bitmap_tank = al_load_bitmap("Source/Tygr_Model_800.jpg");
	t_bitmap_tank_width  = al_get_bitmap_width(t_bitmap_tank);
	t_bitmap_tank_height = al_get_bitmap_height(t_bitmap_tank);


	//---Registration events--------------------------------------------------------
	al_register_event_source(t_event_queue,al_get_mouse_event_source());
	al_register_event_source(t_event_queue,al_get_keyboard_event_source());
	al_register_event_source(t_event_queue,al_get_display_event_source(t_display));
	al_register_event_source(t_event_queue,al_get_timer_event_source(t_timer));
	//------------------------------------------------------------------------------

	al_start_timer(t_timer);
	bool user_cycle = true;
	int LOCAL_STATE_GAME = NULL;
	//---Main user cycle-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while(user_cycle)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(t_event_queue,&event);

		//---Catch exit action-------------------------------
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			LOCAL_STATE_GAME = T_EXIT;
			user_cycle = false;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				LOCAL_STATE_GAME = T_MENU;
				user_cycle = false;
			}
		}
		//---------------------------------------------------

		//---Catch mouse action------------------------------------------------------------------------------------------------------------------------------------------------------------------
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position + 100) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position + 100) + al_get_text_width(t_menu_font_normal,"YES")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_RUN;
				user_cycle = false;
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position + 200) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position + 200) + al_get_text_width(t_menu_font_normal,"NO")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_MENU;
				user_cycle = false;
			}
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			t_redraw = true;
		}

		//---Draw menu-----------------------------------------------------------------------------------------------------------
		if (t_redraw)
		{
			t_redraw = false;
			al_get_mouse_cursor_position(&t_cursor_position.x,&t_cursor_position.y);
			//al_draw_textf(t_menu_font_normal,al_map_rgb(255,0,0),50,50,ALLEGRO_ALIGN_LEFT,"X = %i Y = %i",t_cursor_position.x,t_cursor_position.y);

			al_draw_bitmap(t_bitmap_logo,20,T_WINDOW_HEIGHT - 190,NULL);
			al_draw_bitmap(t_bitmap_tank,T_WINDOW_WIDTH / 2 - t_bitmap_tank_width / 2,10,NULL);

			al_draw_text(t_menu_font_light,al_map_rgb(50,50,130),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 200,ALLEGRO_ALIGN_LEFT,"start new game?");

			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position + 100) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position + 100) + al_get_text_width(t_menu_font_normal,"YES")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position + 100,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"YES");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position + 100,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"YES");
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position + 200) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position + 200) + al_get_text_width(t_menu_font_normal,"NO")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position + 200,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"NO");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position + 200,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"NO");			

			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	}
	//---Deallocation memory--------------
	al_destroy_font(t_menu_font_normal);
	al_destroy_font(t_menu_font_light);
	al_destroy_event_queue(t_event_queue);
	al_destroy_timer(t_timer);
	al_destroy_bitmap(t_bitmap_tank);
	al_destroy_bitmap(t_bitmap_logo);
	//------------------------------------

	return LOCAL_STATE_GAME;
}

int t_settings()
{
	t_x_menu_position = 200;
	if (!al_init())//Error check
	{
		al_show_native_message_box(NULL,"Tanks","Error al_init!!!","Cant initialization allegro 5!!!",NULL,ALLEGRO_MESSAGEBOX_ERROR);
	}

	//---Assignments---------------------------------------------- 
	t_menu_font_normal = al_load_font("Source/Micra Bold.ttf",15,NULL);
	t_menu_font_light  = al_load_font("Source/Micra Bold.ttf",18,NULL);
	t_event_queue = al_create_event_queue();
	t_timer       = al_create_timer(1.0 / t_FPS);
	t_bitmap_logo = al_load_bitmap("Source/IOGS.png");
	t_bitmap_tank = al_load_bitmap("Source/Tygr_Model_800.jpg");
	t_bitmap_tank_width  = al_get_bitmap_width(t_bitmap_tank);
	t_bitmap_tank_height = al_get_bitmap_height(t_bitmap_tank);


	//---Registration events--------------------------------------------------------
	al_register_event_source(t_event_queue,al_get_mouse_event_source());
	al_register_event_source(t_event_queue,al_get_keyboard_event_source());
	al_register_event_source(t_event_queue,al_get_display_event_source(t_display));
	al_register_event_source(t_event_queue,al_get_timer_event_source(t_timer));
	//------------------------------------------------------------------------------

	al_start_timer(t_timer);
	bool user_cycle = true;
	int LOCAL_STATE_GAME = NULL;
	//---Main user cycle-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while(user_cycle)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(t_event_queue,&event);

		//---Catch exit action-------------------------------
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			LOCAL_STATE_GAME = T_EXIT;
			user_cycle = false;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				LOCAL_STATE_GAME = T_MENU;
				user_cycle = false;
			}
		}

		//---Catch mouse action------------------------------------------------------------------------------------------------------------------------------------------------------------------
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"easy")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 170) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 170) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_MENU;
				DIFFICULT = EASY;
				user_cycle = false;
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"normal")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_MENU;
				DIFFICULT = NORMAL;
				user_cycle = false;
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"hard")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 110) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 110) + al_get_font_line_height(t_menu_font_normal))))
			{
				LOCAL_STATE_GAME = T_MENU;
				DIFFICULT = HARD;
				user_cycle = false;
			}
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			t_redraw = true;
		}

		//---Draw menu-----------------------------------------------------------------------------------------------------------
		if (t_redraw)
		{
			t_redraw = false;
			al_get_mouse_cursor_position(&t_cursor_position.x,&t_cursor_position.y);
			//al_draw_textf(t_menu_font_normal,al_map_rgb(255,0,0),50,50,ALLEGRO_ALIGN_LEFT,"X = %i Y = %i",t_cursor_position.x,t_cursor_position.y);

			al_draw_bitmap(t_bitmap_logo,20,T_WINDOW_HEIGHT - 190,NULL);
			al_draw_bitmap(t_bitmap_tank,T_WINDOW_WIDTH / 2 - t_bitmap_tank_width / 2,10,NULL);

			al_draw_text(t_menu_font_light,al_map_rgb(100,150,100),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 200,ALLEGRO_ALIGN_LEFT,"difficult");

			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"easy")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 170) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 170) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 170,ALLEGRO_ALIGN_LEFT,"easy");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 170,ALLEGRO_ALIGN_LEFT,"easy");
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"normal")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 140) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 140) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"normal");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 140,ALLEGRO_ALIGN_LEFT,"normal");
			}
			if ((t_cursor_position.x > (T_WINDOW_WIDTH - t_x_menu_position) && t_cursor_position.x < ((T_WINDOW_WIDTH - t_x_menu_position) + al_get_text_width(t_menu_font_normal,"hard")))
				&& (t_cursor_position.y > (T_WINDOW_HEIGHT - 110) && t_cursor_position.y < ((T_WINDOW_HEIGHT - 110) + al_get_font_line_height(t_menu_font_normal))))
			{
				al_draw_text(t_menu_font_light,al_map_rgb(150,150,150),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 110,ALLEGRO_ALIGN_LEFT,"hard");
			}else{
				al_draw_text(t_menu_font_normal,al_map_rgb(50,50,50),T_WINDOW_WIDTH - t_x_menu_position,T_WINDOW_HEIGHT - 110,ALLEGRO_ALIGN_LEFT,"hard");			
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	}
	//---Deallocation memory--------------
	al_destroy_font(t_menu_font_normal);
	al_destroy_font(t_menu_font_light);
	al_destroy_event_queue(t_event_queue);
	al_destroy_timer(t_timer);
	al_destroy_bitmap(t_bitmap_tank);
	al_destroy_bitmap(t_bitmap_logo);
	//------------------------------------

	return LOCAL_STATE_GAME;
}
