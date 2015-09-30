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

	int startX;
	int startY;
	int endX;
	int endY;
	boolean direction;		//true --> going up		false --> go down
	bool status;			//true --> moving		flase --> stationery
	int floor;		

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

	void setDirection(boolean d)
	{
		direction = d;
	}

	void setStatus(boolean d)
	{
		status = d;
	}

	void moveUp()
	{

		if (startY - 1 >= 80 && direction)
		{
			al_draw_filled_rectangle(startX, startY, endX, endY, al_map_rgb(0, 0, 0));
			startY--;
			endY--;
			al_draw_filled_rectangle(startX, startY, endX, endY, al_map_rgb(0, 0, 255));
			direction = true;
		}
	}

	void moveDown()
	{
		if (startY + 1 <= 620 && !direction)
		{
			al_draw_filled_rectangle(startX, startY, endX, endY, al_map_rgb(0, 0, 0));
			startY++;
			endY++;
			al_draw_filled_rectangle(startX, startY, endX, endY, al_map_rgb(0, 0, 255));
		}
	}


	int floorPosition()
	{
		int temp = 10;
		int str = 80;
		while (str >= 80 && str <= 620)
		{
			if (str == startY)
				break;
			str += 60;
			temp--;

		}
		if (temp != 0)
			floor = temp;
		return floor;
	}

};

