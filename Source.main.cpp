#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>   
#include <allegro5/allegro_acodec.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>

#include <time.h>
#include <math.h>
#include <stdio.h> 

#define width 1000
#define height 700
#define PI 3.14159265
#define GRAVITY 1;

#define FPS 60
#define Num_BOW 1
#define Num_TARGET 3
enum IDS { PLAYER, BOW, ENEMY };
enum KEYS { MOUSE1, MOUSE2, SPACE };
enum mousekeus { m1x, m1y, m2x, m2y };
enum kordinat { X, Y };

double y = 0;
double x = 0;

typedef struct
{
	double ID;
	double xpos;
	double ypos;
	double r;
	double boundx;
	double boundy;
	double lives;
	double scores;
	double speed;


}Characters;
typedef struct
{
	double ID;
	double xpos;
	double ypos;
	double xpos2;
	double ypos2;
	double r;
	bool live;
	double vx;
	double vy;
	int count;



}Bows;
typedef struct
{
	double ID;
	double x;
	double y;
	bool live;
	double speed;
	double boundx;
	double boundy;
}Targets;

void initCharacter(Characters &hero);
void initFunctions(Characters &hero, Bows bow[], Targets target[]);
void RegisterEventSource(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer, ALLEGRO_DISPLAY *display);
void drawArcher(Characters &hero);
void initBows(Bows bow[], int size);
void DrawBow(Bows bow[], int size);
void ThrowBows(Bows bow[], int size, Characters &hero);
void UpdateThrowBows(Bows bow[], int size, double x);
void InýtTarget(Targets target[], int size);
void DrawTarget(Targets target[], int size);
void StartTarget(Targets target[], int size);
void UpdateTarget(Targets target[], int size);
void ColliedBows(Bows bow[], int bSzize, Targets target[], int Csize, Characters &Archer);
void main_menu(bool &menu, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *queue, bool &stop, bool &gameover);

int main()
{
	al_init();
	ALLEGRO_DISPLAY *display = al_create_display(width, height);
RESTART:
	bool IsGameOver = false;
	bool stop = false;
	bool menu = true;
	bool draw = true, redraw = true;
	int sayac = 80;
	srand(time(NULL));

	Characters archer;
	Bows bow[Num_BOW];
	Targets target[Num_TARGET];

	double þiddet[2] = { 0,0 };
	double kordinatmouse[4];

	double mousex, mousey;
	double ret;
	bool keys[3] = { false,false,false };
	bool check = false;
	bool kontrol = false;//okun hareketini durdurmak icin



	al_init_primitives_addon();
	al_install_mouse();
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	initCharacter(archer);
	initBows(bow, Num_BOW);
	InýtTarget(target, Num_TARGET);


	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER *time = al_create_timer(1.0);
	ALLEGRO_FONT *font = al_load_font("arial.ttf", 20, 0);

	ALLEGRO_BITMAP *imageBow = al_load_bitmap("image/bow/bow.png");
	int ImageWidth = al_get_bitmap_width(imageBow);
	int ImageHeigth = al_get_bitmap_height(imageBow);
	int imgY = height / 2 - ImageHeigth / 2;


	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_timer_event_source(time));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_start_timer(timer);
	al_start_timer(time);

	while (!IsGameOver)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
			{

				menu = true;
				stop = true;
				al_stop_timer(timer);
				main_menu(menu, font, event_queue, stop, IsGameOver);
				if (!stop) { goto RESTART; }
				al_resume_timer(timer);

			}
		}
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (ev.timer.source == timer)//adý timer olan timerýn kontrol ettikleri 
			{
				redraw = true;
				if (!IsGameOver)
				{

					StartTarget(target, Num_TARGET);
					UpdateTarget(target, Num_TARGET);
					UpdateThrowBows(bow, Num_BOW, x);
					ColliedBows(bow, Num_BOW, target, Num_TARGET, archer);



				}
			}
			else//adý time olan timerin kontrol ettikleri
			{
				sayac--;
				if (sayac == 0) {
					break;
				}
			}

		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			IsGameOver = true;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mousex = ev.mouse.x;
			mousey = ev.mouse.y;
		}
		else if (bow[0].count == 0)
		{
			IsGameOver = true;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{

			kontrol = true;
			bow[0].count--;
			x = ev.mouse.x;
			y = ev.mouse.y;
			if (ret != 0)
				//printf("%d %d %lf", mousex, mousey, ret);


				if (ev.mouse.button & 1)
				{
					keys[MOUSE1] = true;
					kordinatmouse[m1x] = ev.mouse.x;
					kordinatmouse[m1y] = ev.mouse.y;
				}

				else if (ev.mouse.button & 2)
					keys[MOUSE2] = true;



		}
		else if (ev.type = ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			kontrol = false;

			if (ev.mouse.button & 1)

			{
				for (int i = 0; i < Num_BOW; i++)
				{
					keys[MOUSE1] = false;
					kordinatmouse[m2x] = ev.mouse.x;
					kordinatmouse[m2y] = ev.mouse.y;

					þiddet[X] = kordinatmouse[m1x] - kordinatmouse[m2x];
					þiddet[Y] = kordinatmouse[m1y] - kordinatmouse[m2y];

					bow[i].vx = þiddet[X] / 15;
					bow[i].vy = -þiddet[Y] / 15;
					check = true;

					ThrowBows(bow, Num_BOW, archer);
				}

			}
			else if (ev.mouse.button & 2)
			{
				keys[MOUSE2] = false;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			if (!kontrol)
				ret = atan2(mousey - imgY, mousex);


			al_draw_rotated_bitmap(imageBow, 0, ImageWidth / 2, 20, imgY, ret, 0);

			al_draw_textf(font, al_map_rgb(255, 0, 0), 25, 25, 0, "Time = %d & %d Bow Left  ", sayac, bow[0].count);
			al_draw_textf(font, al_map_rgb(255, 0, 0), 100, 50, 0, "Score = %d", archer.scores);

			drawArcher(archer);
			if (target)
				DrawTarget(target, Num_TARGET);
			DrawBow(bow, Num_BOW);
		}

		main_menu(menu, font, event_queue, stop, IsGameOver);
		al_flip_display();
		al_clear_to_color(al_map_rgb(255, 255, 255));
	}
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
}


void initCharacter(Characters &hero)
{
	hero.ID = PLAYER;
	hero.xpos = 30;
	hero.ypos = height / 2;
	hero.r = 30;
	hero.lives = 3;
	hero.speed = 7;
	hero.scores = 0;
	hero.boundx = 30;
	hero.boundy = 30;




}
void initFunctions(Characters &hero, Bows bow[], Targets target[])
{
	al_init();
	al_init_primitives_addon();
	al_install_mouse();
	al_install_keyboard();
	al_init_image_addon();

	initCharacter(hero);
	initBows(bow, Num_BOW);
	InýtTarget(target, Num_TARGET);
}
void RegisterEventSource(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer, ALLEGRO_DISPLAY *display)
{
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
}

void drawArcher(Characters &hero)
{
	/*al_draw_rectangle(hero.x, hero.y, hero.x + 50, hero.y + 100, al_map_rgb(255, 25, 255), 5);
	al_draw_rectangle(hero.x + 15, hero.y - 20, hero.x + 35, hero.y, al_map_rgb(255, 25, 255), 5);
	al_draw_circle(hero.x + 25, hero.y - 50, hero.r, al_map_rgb(255, 25, 255), 5);
	al_draw_rectangle(hero.x + 20, hero.y + 10, hero.x + 30, hero.y + 60, al_map_rgb(255, 25, 255), 5);
	al_draw_rectangle(hero.x + 10, hero.y + 100, hero.x + 20, hero.y + 200, al_map_rgb(255, 25, 255), 5);
	al_draw_rectangle(hero.x + 30, hero.y + 100, hero.x + 40, hero.y + 200, al_map_rgb(255, 25, 255), 5);*/


	//al_draw_circle(hero.x + 25, hero.y - 50, hero.r, al_map_rgb(255, 25, 255), 5);
}

void initBows(Bows bow[], int size)
{
	for (int i = 0; i < size; i++)
	{
		bow[i].ID = BOW;
		bow[i].live = false;
		bow[i].r = 15;
		bow[i].vx = 0;
		bow[i].vy = 0;
		bow[i].count = 20;





	}
}
void DrawBow(Bows bow[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bow[i].live)
			//al_draw_filled_circle(bow[i].xpos, bow[i].ypos + 170, bow[i].r, al_map_rgb(0, 0, 0));
			al_draw_line(bow[i].xpos, bow[i].ypos + 170, bow[i].xpos + 100, bow[i].ypos + 170, al_map_rgb(0, 0, 0), 2);


	}
}
void ThrowBows(Bows bow[], int size, Characters &hero)
{
	for (int i = 0; i < size; i++) {
		if (!bow[i].live) {
			bow[i].xpos = hero.xpos + 100;
			bow[i].ypos = hero.ypos / 2;
			bow[i].live = true;
			break;
		}
	}
}
void UpdateThrowBows(Bows bow[], int size, double x) {
	for (int i = 0; i < size; i++)
	{
		if (bow[i].live)
		{

			bow[i].xpos += bow[i].vx;
			bow[i].vy -= GRAVITY
				bow[i].ypos -= bow[i].vy;

			bow[i].xpos2 += bow[i].vx;
			bow[i].ypos2 -= bow[i].vy;







			if (bow[i].ypos >= height || bow[i].ypos <= 0)
			{
				//cisim ekrandan cýktý
				bow[i].xpos = bow[i].xpos2 = 20;
				bow[i].ypos = bow[i].ypos2 = height / 5;
				bow[i].vx = 0;
				bow[i].vy = 0;

				bow[i].live = false;
			}

			else if (bow[i].xpos >= width || bow[i].xpos <= 0)
			{
				bow[i].xpos = bow[i].xpos2 = 20;
				bow[i].ypos = bow[i].ypos2 = height / 5;
				bow[i].vx = 0;
				bow[i].vy = 0;

				bow[i].live = false;
			}
		}
	}


}
void InýtTarget(Targets target[], int size) {
	for (int i = 0; i < size; i++) {
		target[i].ID = ENEMY;
		target[i].live = false;
		target[i].speed = 5;
		target[i].boundx = 50;
		target[i].boundy = 50;
	}
}
void DrawTarget(Targets target[], int size) {
	for (int i = 0; i < size; i++) {
		if (target[i].live)
		{
			al_draw_filled_circle(target[i].x, target[i].y, 50, al_map_rgb(255, 0, 0));
			al_draw_filled_circle(target[i].x, target[i].y, 40, al_map_rgb(0, 255, 0));
			al_draw_filled_circle(target[i].x, target[i].y, 30, al_map_rgb(0, 0, 255));
		}
	}
}
void StartTarget(Targets target[], int size) {
	for (int i = 0; i < size; i++) {
		if (!target[i].live) {
			if (rand() % 500 == 0)
			{
				target[i].live = true;
				target[i].x = width - 100;
				target[i].y = 30 + rand() % (height - 60);

				break;
			}
		}
	}
}
void UpdateTarget(Targets target[], int size) {
	for (int i = 0; i < size; i++) {
		if (target[i].live)
		{
			target[i].y -= target[i].speed;
			if (target[i].y <= 0 || target[i].y >= height)
				target[i].speed = -target[i].speed;

		}
	}
}

void ColliedBows(Bows bow[], int bSzize, Targets target[], int Csize, Characters &Archer) {
	for (int i = 0; i < bSzize; i++) {
		if (bow[i].live) {
			for (int j = 0; j < Csize; j++)
			{
				if (target[j].live)
				{
					if (bow[i].xpos >(target[j].x - target[j].boundx) &&
						bow[i].xpos<(target[j].x + target[j].boundx) &&
						bow[i].ypos>(target[j].y - target[j].boundy) &&
						bow[i].ypos < (target[j].y + target[j].boundy))
					{
						bow[i].live = false;
						target[j].live = false;
						Archer.scores++;
						printf("You hit the target");
					}


				}
			}
		}
	}
}

void main_menu(bool &menu, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *queue, bool &stop, bool &gameover)
{
	while (menu)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);
		al_clear_to_color(al_map_rgb(255, 255, 255));
		if (!stop)
		{
			al_draw_text(font, al_map_rgb(46, 46, 46), width / 2, height / 2 - 200, ALLEGRO_ALIGN_CENTRE, "Start");
			al_draw_text(font, al_map_rgb(46, 46, 46), width / 2, height / 2, ALLEGRO_ALIGN_CENTRE, "Options");
			al_draw_text(font, al_map_rgb(46, 46, 46), width / 2, height / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Exit");
		}
		else if (stop)
		{
			al_draw_text(font, al_map_rgb(46, 46, 46), width / 2, height / 2 - 400, ALLEGRO_ALIGN_CENTRE, "New Game");
			al_draw_text(font, al_map_rgb(46, 46, 46), width / 2, height / 2 - 200, ALLEGRO_ALIGN_CENTRE, "Resume Game");
			//al_draw_text(font, al_map_rgb(46, 46, 46), width / 2, height / 2, ALLEGRO_ALIGN_CENTRE, "Options");
			al_draw_text(font, al_map_rgb(46, 46, 46), width / 2, height / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Exit");
		}
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.y >= height / 2 - 200 && ev.mouse.y < height / 2)
			{
				menu = false;
			}
			else if (ev.mouse.y >= height / 2 - 400 && ev.mouse.y < height / 2 - 200)
			{
				menu = false;
				stop = false;
			}
			else if (ev.mouse.y >= height / 2 + 200 && ev.mouse.y < height / 2 + 400)
			{
				gameover = true;
				menu = false;
			}
		}
		al_flip_display();
	}
}