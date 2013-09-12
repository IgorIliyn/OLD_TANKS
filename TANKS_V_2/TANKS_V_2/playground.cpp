#include "playground.h"
#include "variables.h"




int t_run()
{
	srand( time( NULL ) );
	init_run();
	al_start_timer(timer1);

	bool user_exit = false;

	while(!user_exit)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue,&event);
		al_get_mouse_cursor_position(&cursor_x,&cursor_y);
		//---Catch exit action-------------------------------
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			user_exit = true;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				al_stop_sample_instance(wait_inst);
				al_stop_sample_instance(driwe_inst);
				user_exit = true;
			}
		}

		//---Catch mouse action------------------------------
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if(event.mouse.button && 1)
			{
				al_play_sample(tank_shot,0.5,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
				al_play_sample(tank_reload,0.8,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
				enemy_tanks[random_btwn(0,DIFFICULT)].active = false;
			}
		}
		//---------------------------------------------------

		//---Catch keyboard action---------------------------
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(event.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
				keys[UP] = true;
				user_tank.direction = D_UP;
				break;
			case ALLEGRO_KEY_S:
				keys[DOWN] = true;
				user_tank.direction = D_DOWN;
				break;
			case ALLEGRO_KEY_A:
				keys[LEFT] = true;
				user_tank.direction = D_LEFT;
				break;
			case ALLEGRO_KEY_D:
				keys[RIGHT] = true;
				user_tank.direction = D_RIGHT;
				break;
			}
		}
		if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(event.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_S:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_A:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_D:
				keys[RIGHT] = false;
				break;
			}
		}
		
		
		//-----------------------------------------------------------
		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			A_I_action();
			t_driwe(user_tank,keys);
			redraw = true;
		}
		if (keys[UP] || keys[DOWN] || keys[LEFT] || keys[RIGHT])
		{
			if (!al_get_sample_instance_playing(driwe_inst))
			{
				al_play_sample_instance(driwe_inst);
				if (al_get_sample_instance_playing(wait_inst))
				{
					al_stop_sample_instance(wait_inst);
				}
			}
		}else{
			if (al_get_sample_instance_playing(driwe_inst))
			{
				al_stop_sample_instance(driwe_inst);
				if (!al_get_sample_instance_playing(wait_inst))
				{
					al_play_sample_instance(wait_inst);
				}
			}
		}
		//---Draw screen------------------------------------------------------------------------------------------------------------------------------------------
		if (redraw)
		{
			redraw = false;

			t_angles(user_tank,cursor_x,cursor_y,keys);

			al_draw_rotated_bitmap(user_body,user_tank.tank_width / 2,user_tank.tank_height / 2,user_tank.x,user_tank.y,user_tank.body_angle,NULL);
			al_draw_rotated_bitmap(user_turret,user_tank.tank_width / 2,user_tank.tank_height / 2,user_tank.x,user_tank.y,user_tank.turret_angle,NULL);
			for (int i = 0; i < DIFFICULT; i++)
			{
				if (enemy_tanks[i].active)
				{
					al_draw_rotated_bitmap(enemy_body,enemy_tanks[i].tank_width / 2,enemy_tanks[i].tank_height / 2,enemy_tanks[i].x,enemy_tanks[i].y,enemy_tanks[i].body_angle,NULL);
					al_draw_rotated_bitmap(enemy_turret,enemy_tanks[i].tank_width / 2,enemy_tanks[i].tank_height / 2,enemy_tanks[i].x,enemy_tanks[i].y,enemy_tanks[i].turret_angle,NULL);
				}
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		//---------------------------------------------------------------------------------------------------------------------------------------------------------
	}

	//---Deallocation memory--------------
	al_destroy_sample(back_music);
	al_destroy_sample(tank_driwe);
	al_destroy_sample(tank_wait);
	al_destroy_sample(tank_shot);
	al_destroy_sample(tank_reload);
	al_destroy_sample_instance(driwe_inst);
	al_destroy_sample_instance(wait_inst);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer1);
	al_destroy_bitmap(user_body);
	al_destroy_bitmap(user_turret);
	al_destroy_bitmap(enemy_body);
	al_destroy_bitmap(enemy_turret);	
	for (int i = 0; i < DIFFICULT; i++)
	{
		enemy_tanks.pop_back();
	}
	return T_MENU;
}

void t_driwe(TANK &tank, bool keys[4])
{
	if (keys[UP])
	{
		tank.direction = D_UP;
		if (free_way(tank))
		{
			tank.y -= keys[UP]    * 2;
		}
	}else if (keys[DOWN])
	{
		tank.direction = D_DOWN;
		if (free_way(tank))
		{
			tank.y += keys[DOWN]  * 2;
		}
	}else if (keys[LEFT])
	{
		tank.direction = D_LEFT;
		if (free_way(tank))
		{
			tank.x -= keys[LEFT]  * 2;
		}
	}else if (keys[RIGHT])
	{
		tank.direction = D_RIGHT;
		if (free_way(tank))
		{
			tank.x += keys[RIGHT] * 2;
		}
	}
}

void t_draiw_A_I(TANK &tank)
{
	switch(tank.direction)
	{
	case D_UP:
		tank.y -= tank_speed;
		tank.body_angle = 0.0f;
		break;
	case D_DOWN:
		tank.y += tank_speed;
		tank.body_angle = 3.1416f;
		break;
	case D_LEFT:
		tank.x -= tank_speed;
		tank.body_angle = -1.5708f;
		break;
	case D_RIGHT:
		tank.x += tank_speed;
		tank.body_angle = 1.5708f;
		break;
	}
}

void t_angles(TANK &tank, int cursor_x, int cursor_y, bool keys[4])
{
	if (cursor_y < tank.y && cursor_x < tank.x)
	{
		float a = (tank.y - cursor_y);
		float akv = a * a;
		float b = (tank.x - cursor_x);
		float bkv = b * b;
		float gipotenuza = sqrt(akv+bkv);
		float d = b / gipotenuza;
		tank.turret_angle = -(asin(d));
	}
	if (cursor_x >= tank.x && cursor_y <= tank.y)
	{
		float a = (tank.y - cursor_y);
		float akv = a * a;
		float b = (cursor_x - tank.x);
		float bkv = b * b;
		float gipotenuza = sqrt(akv+bkv);
		float d = b / gipotenuza;
		tank.turret_angle = (asin(d));
	}
	if (cursor_y >= tank.y && cursor_x < tank.x)
	{
		float a = (tank.x - cursor_x);
		float akv = a * a;
		float b = (cursor_y - tank.y);
		float bkv = b * b;
		float gipotenuza = sqrt(akv+bkv);
		float d = b / gipotenuza;
		tank.turret_angle = -(asin(d)) + (-1.57);
	}
	if (cursor_x >= tank.x && cursor_y > tank.y)
	{
		float a = (cursor_y - tank.y);
		float akv = a * a;
		float b = (cursor_x - tank.x);
		float bkv = b * b;
		float gipotenuza = sqrt(akv+bkv);
		float d = b / gipotenuza;
		tank.turret_angle = -(asin(d)) + (-3.14);
	}	

	if (keys[UP])
	{
		tank.body_angle = 0.0;
	}else if (keys[DOWN])
	{
		tank.body_angle = 3.1416;
	}else if (keys[LEFT])
	{
		tank.body_angle = -1.5708;
	}else if (keys[RIGHT])
	{
		tank.body_angle = 1.5708;
	}
}

void t_A_I_angles(TANK &tank,int cursor_x, int cursor_y)
{
	if (cursor_y < tank.y && cursor_x < tank.x)
	{
		float a = (tank.y - cursor_y);
		float akv = a * a;
		float b = (tank.x - cursor_x);
		float bkv = b * b;
		float gipotenuza = sqrt(akv+bkv);
		float d = b / gipotenuza;
		tank.turret_angle = -(asin(d));
	}
	if (cursor_x >= tank.x && cursor_y <= tank.y)
	{
		float a = (tank.y - cursor_y);
		float akv = a * a;
		float b = (cursor_x - tank.x);
		float bkv = b * b;
		float gipotenuza = sqrt(akv+bkv);
		float d = b / gipotenuza;
		tank.turret_angle = (asin(d));
	}
	if (cursor_y >= tank.y && cursor_x < tank.x)
	{
		float a = (tank.x - cursor_x);
		float akv = a * a;
		float b = (cursor_y - tank.y);
		float bkv = b * b;
		float gipotenuza = sqrt(akv+bkv);
		float d = b / gipotenuza;
		tank.turret_angle = -(asin(d)) + (-1.57);
	}
	if (cursor_x >= tank.x && cursor_y > tank.y)
	{
		float a = (cursor_y - tank.y);
		float akv = a * a;
		float b = (cursor_x - tank.x);
		float bkv = b * b;
		float gipotenuza = sqrt(akv+bkv);
		float d = b / gipotenuza;
		tank.turret_angle = -(asin(d)) + (-3.14);
	}	
}

void init_run()
{
	if (!al_init())//Error check
	{
		al_show_native_message_box(NULL,"Tanks","Error al_init!!!","Can't initialization allegro 5!!!",NULL,ALLEGRO_MESSAGEBOX_ERROR);
	}

	//---Assignments---------------------------------------------- 
	al_reserve_samples(7);
	event_queue   = al_create_event_queue();
	timer1        = al_create_timer(1.0 / FPS); 
	back_music    = al_load_sample("Source/back_music.wav");
	al_play_sample(back_music,0.7,0.0,1.0,ALLEGRO_PLAYMODE_LOOP,0);
	user_body     = al_load_bitmap("Source/tank_body1.png");
	user_turret   = al_load_bitmap("Source/tank_turret1.png");
	enemy_body    = al_load_bitmap("Source/enemy_tank.png");
	enemy_turret  = al_load_bitmap("Source/enemy_turret.png");
	tank_driwe    = al_load_sample("Source/tank_engine.WAV");
	tank_wait     = al_load_sample("Source/tank_engine.WAV");
	tank_shot     = al_load_sample("Source/shot.wav");
	tank_reload   = al_load_sample("Source/reload.wav");
	driwe_inst    = al_create_sample_instance(tank_driwe);
	wait_inst     = al_create_sample_instance(tank_wait);
	al_attach_sample_instance_to_mixer(driwe_inst,al_get_default_mixer());
	al_attach_sample_instance_to_mixer(wait_inst,al_get_default_mixer());
	al_set_sample_instance_gain(driwe_inst,0.5);
	al_set_sample_instance_gain(wait_inst,0.3);
	al_set_sample_instance_playmode(wait_inst,ALLEGRO_PLAYMODE_LOOP);
	//---Registration events--------------------------------------------------------
	al_register_event_source(event_queue,al_get_mouse_event_source());
	al_register_event_source(event_queue,al_get_keyboard_event_source());
	al_register_event_source(event_queue,al_get_display_event_source(t_display));
	al_register_event_source(event_queue,al_get_timer_event_source(timer1));
	//---Creating tanks-------------------------------------------------------------		
	user_tank.active      = true;
	user_tank.direction   = D_UP;
	user_tank.id          = USER_TANK;
	user_tank.lives       = 3;
	user_tank.x           = T_WINDOW_WIDTH / 2;
	user_tank.y           = T_WINDOW_HEIGHT - al_get_bitmap_height(user_body) / 2;
	user_tank.tank_width  = al_get_bitmap_width(user_body);
	user_tank.tank_height = al_get_bitmap_height(user_body);

	enemy_tank.active = true;
	enemy_tank.body_angle = 3.1416;
	enemy_tank.turret_angle = 3.1416;
	if (DIFFICULT == EASY)enemy_tank.lives = 2;
	if (DIFFICULT == NORMAL)enemy_tank.lives = 3;
	if (DIFFICULT == HARD)enemy_tank.lives = 4;
	enemy_tank.y = 64;
	enemy_tank.tank_width = al_get_bitmap_width(enemy_body);
	enemy_tank.tank_height = al_get_bitmap_height(enemy_body);

	for (int i = 0; i < DIFFICULT; i++)
	{
		enemy_tank.id = i;
		enemy_tanks.push_back(enemy_tank);
	}

	if (DIFFICULT == EASY)
	{
		enemy_tanks[0].x = 32;	
		enemy_tanks[1].x = 683;	
		enemy_tanks[2].x = 1334;	
	}
	if (DIFFICULT == NORMAL)
	{
		enemy_tanks[0].x = 64;	
		enemy_tanks[1].x = 385;	
		enemy_tanks[2].x = 685;
		enemy_tanks[3].x = 985;	
		enemy_tanks[4].x = 1302;
	}
	if (DIFFICULT == HARD)
	{
		enemy_tanks[0].x = 50;	
		enemy_tanks[1].x = 260;	
		enemy_tanks[2].x = 470;
		enemy_tanks[3].x = 680;	
		enemy_tanks[4].x = 890;
		enemy_tanks[5].x = 1100;	
		enemy_tanks[6].x = 1320;	
	}
	for (int i = 0; i < DIFFICULT; i++)
	{
		enemy_tanks[i].direction = random_btwn(D_UP,D_RIGHT);
		enemy_tanks[i].distance = random_btwn(100,500);
	}
}

void change_d_d(TANK &tank)
{
	if (tank.distance <= 0)
	{
		tank.distance = random_btwn(100,350);
		tank.direction = random_btwn(D_UP,T_NEW);
	}else{
		tank.direction = random_btwn(D_UP,T_NEW);
	}
}

bool free_way(TANK &tank)
{
	//----------rebuild code-------------------------------------------------------------------
	bool way = true;		
	switch(tank.direction)
	{
	case D_RIGHT:
		if (tank.x > 1310)
		{
			return !way;
		}
		switch (user_tank.direction)
		{
		case D_UP:
		case D_DOWN:
			if (abs(tank.y - user_tank.y) < 80 && (user_tank.x - tank.x) < 80 && user_tank.x > tank.x)
			{
				return !way;
			}
			break;
		case D_LEFT:
		case D_RIGHT:
			if (abs(tank.y - user_tank.y) < 64 && (user_tank.x - tank.x) < 95 && user_tank.x > tank.x)
			{
				return !way;
			}
			break;
		}
		for (int i = 0; i < DIFFICULT; i++)
		{
			if ((enemy_tanks[i].id == tank.id) || (tank.active == false))
			{
				continue;
			}
			switch(enemy_tanks[i].direction)
			{
			case D_RIGHT:
			case D_LEFT:
				if (abs(tank.y - enemy_tanks[i].y) < 64 && (enemy_tanks[i].x - tank.x) < 100 && enemy_tanks[i].x > tank.x)
				{
					return !way;
				}
				break;
			case D_UP:
			case D_DOWN:
				if (abs(tank.y - enemy_tanks[i].y) < 80 && (enemy_tanks[i].x - tank.x) < 80 && enemy_tanks[i].x > tank.x)
				{
					return !way;
				}
			}
		}
		break;
	case D_LEFT:
		if (tank.x < 57)
		{
			return !way;
		}
		switch (user_tank.direction)
		{
		case D_UP:
		case D_DOWN:
			if (abs(tank.y - user_tank.y) < 80 && (tank.x - user_tank.x) < 80 && tank.x > user_tank.x)
			{
				return !way;
			}
			break;
		case D_LEFT:
		case D_RIGHT:
			if (abs(tank.y - user_tank.y) < 64 && (tank.x - user_tank.x) < 95 && tank.x > user_tank.x)
			{
				return !way;
			}
			break;
		}
		for (int i = 0; i < DIFFICULT; i++)
		{
			if ((enemy_tanks[i].id == tank.id) || (tank.active == false))
			{
				continue;
			}
			switch(enemy_tanks[i].direction)
			{
			case D_RIGHT:
			case D_LEFT:
				if (abs(tank.y - enemy_tanks[i].y) < 64 && (tank.x - enemy_tanks[i].x) < 100 && tank.x > enemy_tanks[i].x)
				{
					return !way;
				}
				break;
			case D_UP:
			case D_DOWN:
				if (abs(tank.y - enemy_tanks[i].y) < 80 && (tank.x - enemy_tanks[i].x) < 80 && tank.x > enemy_tanks[i].x)
				{
					return !way;
				}
			}
		}
		break;
	case D_UP:
		if (tank.y < 57)
		{
			return !way;
		}
		switch (user_tank.direction)
		{
		case D_UP:
		case D_DOWN:
			if (abs(tank.x - user_tank.x) < 64 && (tank.y - user_tank.y) < 95 && user_tank.y < tank.y)
			{
				return !way;
			}
			break;
		case D_LEFT:
		case D_RIGHT:
			if (abs(tank.x - user_tank.x) < 80 && (tank.y -user_tank.y) < 80 && user_tank.y < tank.y)
			{
				return !way;
			}
			break;
		}
		for (int i = 0; i < DIFFICULT; i++)
		{
			if ((enemy_tanks[i].id == tank.id) || (tank.active == false))
			{
				continue;
			}
			switch(enemy_tanks[i].direction)
			{
			case D_RIGHT:
			case D_LEFT:
				if (abs(tank.x - enemy_tanks[i].x) < 80 && (tank.y - enemy_tanks[i].y) < 80 && enemy_tanks[i].y < tank.y)
				{
					return !way;
				}
				break;
			case D_UP:
			case D_DOWN:
				if (abs(tank.x - enemy_tanks[i].x) < 64 && (tank.y - enemy_tanks[i].y) < 100 && enemy_tanks[i].y < tank.y)
				{
					return !way;
				}
			}
		}
		break;
	case D_DOWN:
		if (tank.y > 711)
		{
			return !way;
		}
		switch (user_tank.direction)
		{
		case D_UP:
		case D_DOWN:
			if (abs(tank.x - user_tank.x) < 64 && (user_tank.y - tank.y) < 95 && user_tank.y > tank.y)
			{
				return !way;
			}
			break;
		case D_LEFT:
		case D_RIGHT:
			if (abs(tank.x - user_tank.x) < 80 && (user_tank.y - tank.y) < 80 && user_tank.y > tank.y)
			{
				return !way;
			}
			break;
		}
		for (int i = 0; i < DIFFICULT; i++)
		{
			if ((enemy_tanks[i].id == tank.id) || (tank.active == false))
			{
				continue;
			}
			switch(enemy_tanks[i].direction)
			{
			case D_RIGHT:
			case D_LEFT:
				if (abs(tank.x - enemy_tanks[i].x) < 80 && (enemy_tanks[i].y - tank.y) < 80 && enemy_tanks[i].y > tank.y)
				{
					return !way;
				}
				break;
			case D_UP:
			case D_DOWN:
				if (abs(tank.x - enemy_tanks[i].x) < 64 && (enemy_tanks[i].y - tank.y) < 100 && enemy_tanks[i].y > tank.y)
				{
					return !way;
				}
			}
		}
		break;
	}
	//--------------------------------------------------------------------------------------------
	return way;
}

void A_I_action()
{
	for (int i = 0; i < DIFFICULT; i++)
	{
		if (enemy_tanks[i].active == false)
		{
			enemy_tanks[i].y = 64;
			enemy_tanks[i].x = get_new_tank_position();
			enemy_tanks[i].active = true;
		}
	}
	for (int i = 0; i < DIFFICULT; i++)
	{
		if(free_way(enemy_tanks[i]) && enemy_tanks[i].distance > 0)
		{
			enemy_tanks[i].distance--;   
			t_draiw_A_I(enemy_tanks[i]);
		}else{
			change_d_d(enemy_tanks[i]);
		}		
	}
	for (int i = 0; i < DIFFICULT; i++)
	{
		if (enemy_tanks[i].active == true)
		{
			t_A_I_angles(enemy_tanks[i],user_tank.x,user_tank.y);
		}
	}
}

int random_btwn( const int min, const int max )
{
	int range = max - min;
	return rand() % range + min;
}

int get_new_tank_position()
{
	int x_x = 0;
	bool cicle = true;
	while (cicle)
	{
		bool enemy = false;
		x_x = random_btwn(32,1300);
		for (int i = 0; i < DIFFICULT; i++)
		{
			if (!((enemy_tanks[i].y >= 192) || (abs(enemy_tanks[i].x - x_x) >= 74)))
			{
				enemy = true;
				break;
			}
		}
		if (enemy == false)
		{
			if (abs(user_tank.x - x_x) >= 74 || user_tank.y >= 192)
			{
				cicle = false;
			}
		}		
	}
	return x_x;
}
