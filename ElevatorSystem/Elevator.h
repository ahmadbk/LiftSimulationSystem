#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class Elevator
{
private:

	const int startX;
	const int startY;
	const int endX;
	const int endY;
	boolean direction;		//true --> going up   false--->go down


public:
	Elevator(int sX, int sY, int eX, int eY) :startX(sX), startY(sY), endX(eX), endY(eY)
	{
		al_draw_filled_rectangle(sX, sY, eX, eY, al_map_rgb(0, 0, 255));

	};

	~Elevator(){};

	boolean getDirection()
	{
		return direction;
	}

	void moveUp()
	{
		direction = true;
	}

	void moveDown()
	{
		direction = false;
	}

};

