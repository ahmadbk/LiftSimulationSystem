#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//Ahmad Bin Khalid
//Sesethu Mhlana

class Button
{
private:

	const int height;
	const int startX;
	const int startY;
	const int endX;
	const int endY;
	bool buttonPressed = false; //true--> pressed 
	const int buttonType;		//1 --> elevator button  and 2 --> floor button
	int FbuttonNumber;
	int EbuttonNumber;
	boolean FbuttonDirection;


public:
	Button(int sX, int sY, int eX, int eY, int h, int bT, int n) :startX(sX), startY(sY), endX(eX), endY(eY), height(h), buttonType(bT)
	{
		al_draw_rectangle(sX, sY, eX, eY, al_map_rgb(255, 0, 0), h);

		EbuttonNumber = n;

	};

	Button(int sX, int sY, int eX, int eY, int h, int bT, int n, boolean d) :startX(sX), startY(sY), endX(eX), endY(eY), height(h), buttonType(bT)
	{
		al_draw_rectangle(sX, sY, eX, eY, al_map_rgb(255, 0, 0), h);

		FbuttonNumber = n;
		FbuttonDirection = d;
	};

	~Button(){};
	boolean CheckButtonPressed(ALLEGRO_EVENT ec)
	{
		if (ec.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if (ec.mouse.x >= startX && ec.mouse.x <= endX && ec.mouse.y >= startY && ec.mouse.y < endY)	// If mouse presses is within these bounds, then choose button x
				buttonPressed = true;
			else
				buttonPressed = false;
		}
		return buttonPressed;
	}
	int getBType()
	{
		return buttonType;
	}
	int getBNum()
	{
		if (buttonType == 1)
			return EbuttonNumber;
		else
			return FbuttonNumber;
	}
	boolean getDirection()
	{
		if (buttonType == 2)
			return FbuttonDirection;
	}

	void illuminate(ALLEGRO_FONT *font4)
	{
		al_draw_filled_rectangle(startX, startY, endX, endY, al_map_rgb(127, 127, 0));
		if (buttonType == 2)
		{
			if (FbuttonDirection)
				al_draw_text(font4, al_map_rgb(255, 255, 255), startX+6, startY, ALLEGRO_ALIGN_LEFT, "U");
			else
				al_draw_text(font4, al_map_rgb(255, 255, 255), startX + 6, startY, ALLEGRO_ALIGN_LEFT, "D");
		}
		else
		{
			al_draw_textf(font4, al_map_rgb(255, 255, 255), startX + 6, startY, ALLEGRO_ALIGN_LEFT, "%d",EbuttonNumber);
		}

	}

	void CancelIlluminate(ALLEGRO_FONT *font4)
	{
		al_draw_filled_rectangle(startX, startY, endX, endY, al_map_rgb(0, 0, 255));
		if (buttonType == 2)
		{
			if (FbuttonDirection)
				al_draw_text(font4, al_map_rgb(255, 255, 255), startX + 6, startY, ALLEGRO_ALIGN_LEFT, "U");
			else
				al_draw_text(font4, al_map_rgb(255, 255, 255), startX + 6, startY, ALLEGRO_ALIGN_LEFT, "D");
		}
		else
		{
			al_draw_textf(font4, al_map_rgb(255, 255, 255), startX + 6, startY, ALLEGRO_ALIGN_LEFT, "%d", EbuttonNumber);
		}
	}
};

