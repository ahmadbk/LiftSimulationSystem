#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include"allegro5/allegro_image.h"

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <math.h>
#include <iostream>
#include <string>

#include "Button.h"
#include "Elevator.h"


#define PI 3.14159265359

using namespace std;

const float FPS = 45;
const int SCREEN_W = 640;
const int SCREEN_H = 700;

boolean makeObjects = false;		//To ensure Elevator only created once


enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};


int main(int argc, char **argv)
{

	srand(time(NULL));

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_SAMPLE *sample = NULL;
	ALLEGRO_SAMPLE *SAMMY = NULL;
	ALLEGRO_SAMPLE *thunder = NULL;


	//create stacks or queues here
	Button *b1[5];	//1st column - elevator buttons
	Button *b2[5];	//2nd column - elevator buttons
	Button *b3[10];	//1st column - floor up buttons
	Button *b4[10];	//2nd column - floor down buttons
	Elevator *lift = NULL;

	b4[9] = NULL;
	b3[9] = NULL;

	bool key[4] = { false, false, false, false };
	bool redraw = true;
	bool doexit = false;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "failed to initialize primitives addon!\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);

	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	if (!al_install_mouse()) {
		fprintf(stderr, "failed to initialize the mouse!\n");
		return -1;
	}

	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_FONT *font1 = al_load_ttf_font("OpenSans-Regular.ttf", 20, 0);
	ALLEGRO_FONT *font2 = al_load_ttf_font("OpenSans-Regular.ttf", 30, 0);
	ALLEGRO_FONT *font3 = al_load_ttf_font("OpenSans-Regular.ttf", 12, 0);
	ALLEGRO_FONT *font4 = al_load_ttf_font("OpenSans-Regular.ttf", 12, 0);


	if (!font1){
		fprintf(stderr, "Could not load font.\n");
		return -1;
	}

	if (!al_init_image_addon()) {
		fprintf(stderr, "Failed to initialize image addon!\n");
	}

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();
	al_start_timer(timer);


	bool menu = true;

	while (menu)
	{
		int numOfButtons = 1;
		int startx = 20;
		int starty = 20;
		int ButtonType;

		al_draw_text(font2, al_map_rgb(255, 0, 40), startx, starty, ALLEGRO_ALIGN_LEFT, "INSIDE ELEVATOR");
		al_draw_text(font2, al_map_rgb(255, 0, 40), startx + 320, starty, ALLEGRO_ALIGN_LEFT, "OUTSIDE ELEVATOR");
		al_draw_line(320, 0, 320, 700, al_map_rgb(255, 0, 40), 10);

		if (!makeObjects)
		{
			lift = new Elevator(400, 620, 450, 670);
			makeObjects = true;
		}

		//-----------------------------------------------------------------

		//1st column of blocks outside the elevator
		int sY = 70;
		int eY = 100;
		int height = 2;
		ButtonType = 1;		//Elevator Button
		for (int i = 0; i < 5; i++)
		{
			b1[i] = new Button(70, sY, 100, eY, height, ButtonType, (i + 1));
			int a = i + 1;
			char *c = (char*)a;
			al_draw_text(font1, al_map_rgb(0, 0, 255), 80, sY, ALLEGRO_ALIGN_LEFT, "1");
			sY += 50;
			eY += 50;
		}

		//------------------------------------------------------------------

		//2nd Column of blocks outside lift
		sY = 70;
		eY = 100;
		for (int i = 0; i < 5; i++)
		{
			b2[i] = new Button(150, sY, 180, eY, height, ButtonType, (i + 1 + 5));
			al_draw_text(font1, al_map_rgb(0, 0, 255), 160, sY, ALLEGRO_ALIGN_LEFT, "2");
			sY += 50;
			eY += 50;
		}

		//------------------------------------------------------------------

		//Draw Lines in between floors
		sY = 70;
		eY = 70;
		for (int i = 0; i < 11; i++)
		{
			al_draw_line(550, sY, 640, eY, al_map_rgb(255, 0, 40), 2);
			sY += 60;
			eY += 60;
		}

		//------------------------------------------------------------------

		//Floor Buttons
		sY = 100;
		eY = 120;
		ButtonType = 2;		//Floor Button
		for (int i = 0; i < 10; i++)					//Button(startX,startY,endX,endY,height,floor/elevator,floor number/direction)
		{
			if (i != 0)
			{
				b3[i] = new Button(570, sY, 590, eY, height, ButtonType, (10 - i), true);			//creates the first column of the floor button excluding last floor
				al_draw_text(font4, al_map_rgb(0, 0, 255), 576, sY, ALLEGRO_ALIGN_LEFT, "\c");		
			}
			if (i != 9)
			{
				b4[i] = new Button(610, sY, 630, eY, height, ButtonType, (10 - i), false);			//creates the second column of the floor buttons excluding first floor
				al_draw_text(font3, al_map_rgb(0, 0, 255), 616, sY, ALLEGRO_ALIGN_LEFT, "D");
			}
			sY += 60;
			eY += 60;
		}

		//------------------------------------------------------------------


		al_flip_display();
		ALLEGRO_EVENT ec;
		al_wait_for_event(event_queue, &ec);

		//Two types of buttons: Elevator and Floor 
		//Each type of buttons have two corresponding array of buttons set in columns on the GUI
		//Elevator buttons are in b[1] and b[2]
		//Floor buttons are in b[3] and b[4]
		//now to check which button was pressed: 1st check if there was a mouse click

		if (ec.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			//check all elevator buttons
			for (int i = 0; i < 5; i++)
			{
				boolean ff1 = false;					//1st flag to check 1st column
				boolean ff2 = false;					//second flag to check 2nd column
				ff1 = b1[i]->CheckButtonPressed(ec);	//the function tells us if the button was pressed
				ff2 = b2[i]->CheckButtonPressed(ec);
				
				if (ff1)								
				{
					printf("%d\n", b1[i]->getBNum());	//print the button that was pressed	
					b1[i]->illuminate();				//illuminate the corresponding button
				}
				if (ff2)
				{
					printf("%d\n", b2[i]->getBNum());
					b2[i]->illuminate();
				}
			}

			//check floor buttons
			for (int i = 0; i < 10; i++)
			{
				boolean ff1 = false;
				boolean ff2 = false;
				if (i != 0)															//1st column has no button on the the last floor 
					ff1 = b3[i]->CheckButtonPressed(ec);							//check if button pressed in the 1st column
				if (i != 9)
					ff2 = b4[i]->CheckButtonPressed(ec);							//second column doesnt have button the first floor

				if (ff1)
				{
					printf("(%d,%d)\n", b3[i]->getBNum(), b3[i]->getDirection());	//print what floor button was pressed and the corresponding direction
					b3[i]->illuminate();											//illuminate the button
				}
				if (ff2)
				{
					printf("(%d,%d)\n", b4[i]->getBNum(), b4[i]->getDirection());
					b4[i]->illuminate();
				}
			}

		}

		if (ec.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			exit(1);
		}

		//Work Here
		//-order the queue
		//-check the direction of the lift
		//-command the lift to move to desitination
		

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		printf("%d\n", lift->floorPosition());	//Print the current position of the floor

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (lift->floorPosition() != 10)	//I want the lift to go to the 5th floor
			{
				lift->setStatus(true);			//set the lift in motion
				lift->moveUp();					//move up 
				lift->moveDown();				//move down
			}
			else
			{
				lift->setStatus(false);
			}
												//accroding to the direction i have set the lift earlier, the lift will move in that direction								
		}										//and surpass the other method --> this happens in the methods
										
	}

	//while (!doexit)
	//{
	//	ALLEGRO_EVENT ev;
	//	al_wait_for_event(event_queue, &ev);

	//	if (ev.type == ALLEGRO_EVENT_TIMER)
	//	{
	//		//lift->drawElevator(yHeightS + 100, yHeightE + 100);
	//	}

	//	if (redraw && al_is_event_queue_empty(event_queue))
	//	{
	//	//	al_flip_display();
	//	}

	//}

	al_destroy_font(font1);
	al_destroy_font(font2);
	al_destroy_font(font3);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_sample(sample);

	return 0;
}