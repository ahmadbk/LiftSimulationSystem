#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <vector>
#include <algorithm>    // For std::sort
#include <functional>	// For comparison with greator

class Elevator
{
private:

	int startX;
	int startY;
	int endX;
	int endY;
	int direction;		//1 --> going up		0 --> go down	-1 --> unallocated
	bool status;			//true --> moving		false --> stationary
	int floor;

public:
	Elevator(int sX, int sY, int eX, int eY) :startX(sX), startY(sY), endX(eX), endY(eY)
	{
		al_draw_filled_rectangle(sX, sY, eX, eY, al_map_rgb(0, 0, 255));
	};

	~Elevator(){};

	int getDirection()
	{
		return direction;
	}

	void setDirection(int d)
	{
		direction = d;
	}

	void setStatus(boolean d)
	{
		status = d;
	}

	bool getStatus()
	{
		return status;
	}

	void moveUp()
	{

		if (startY - 1 >= 80 && direction && status)
		{
			al_draw_filled_rectangle(startX, startY, endX, endY, al_map_rgb(0, 0, 0));
			startY-=4;
			endY-=4;
			al_draw_filled_rectangle(startX, startY, endX, endY, al_map_rgb(0, 0, 255));
			direction = true;
		}
	}

	void moveDown()
	{
		if (startY + 1 <= 620 && !direction && status)
		{
			al_draw_filled_rectangle(startX, startY, endX, endY, al_map_rgb(0, 0, 0));
			startY+=4;
			endY+=4;
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

	void allocateDirection(std::vector<int> & upReq, std::vector<int> downReq, std::vector<int> servReq, int currDest)
	{
		int dir;
		if (upReq.size() > 0 && nextUpAddress(upReq, currDest, 1) > floor)
		{
			direction = 1;
		}
		else if (downReq.size() > 0 && nextUpAddress(downReq, currDest, 0) > floor)
		{
			direction = 1;
		}
		else if (servReq.size() > 0 && nextUpAddress(servReq, currDest, 1) > floor)
		{
			direction = 1;
		}
		else if (downReq.size() > 0 && nextDownAddress(downReq, currDest, 0) < floor)
		{
			direction = 0;
		}
		else if (upReq.size() > 0 && nextDownAddress(upReq, currDest, 1) < floor)
		{
			direction = 0;
		}
		else if (servReq.size() > 0 && nextDownAddress(servReq, currDest, 1) < floor)
		{
			direction = 0;
		}
		else
		{
			direction = -1;
		}
	}

	int nextUpAddress(std::vector<int> & list, int currDest, int reqType)
	{
		int destFloor = currDest, it = 0;
		if (reqType == 1)												// If its an up request
		{
			if (list.size() > 1)										// This is only done if there are requests in the list
			{
				while (it < list.size())								// && floor >= reqList[it])	// Start from the beginning of the vector and search for the closest request to the 
				{
					destFloor = list[it];								// elevator's current floor
					if (destFloor < floor)
						it++;
					else it = list.size();		// Force termination
				}
			}
			else if (list.size() == 1)
			{
				destFloor = list[0];
			}
		}
		else if (reqType == 0)
			destFloor = list[0];

		return destFloor;
	}

	int nextDownAddress(std::vector<int> & list, int currDest, int reqType)
	{
		int destFloor = currDest, it = 0;
		if (reqType == 1 || list.size() == 1)												// If its an up request
		{
			destFloor = list[0];
		}
		else if (reqType == 0)
		{
			if (list.size() > 1)										// This is only done if there are requests in the list
			{
				while (it < list.size())								// && floor >= reqList[it])	// Start from the beginning of the vector and search for the closest request to the 
				{
					destFloor = list[it];								// elevator's current floor
					if (destFloor > floor)
						it++;
					else it = list.size();		// Force termination
				}
			}
		}
		return destFloor;
	}


	//// destination() accepts a vector as an argument and determines which floor to accept a request from. For now onl UP request are executed
	//int destination(std::vector<int> & reqList, int reqType, Elevator& lift)
	//{
	//	int destFloor = floor, it = 0;
	//	if (reqType == 1)				// If its an up request
	//	{
	//		if (reqList.size() > 1)				// This is only done if there are requests in the list
	//		{
	//			while (it < reqList.size()) // && floor >= reqList[it])	// Start from the beginning of the vector and search for the closest request to the 
	//			{
	//				destFloor = reqList[it];													// elevator's current floor
	//				if (destFloor <= floor)
	//					it++;
	//				else it = reqList.size();		// Force termination
	//			}
	//		}
	//		else if (reqList.size() == 1) destFloor = reqList[0];	// If there is one element in the list, return that element
	//		else if (reqList.size() == 0 || it >= reqList.size())
	//		{
	//			destFloor = -1;			// If there is no element in the list, return an invalid value or next destinstion is not obtained
	//			lift.setDirection(false);
	//		}
	//	}
	//	else							// If its a down request
	//	{
	//		if (reqList.size() > 1)				// This is only done if there are requests in the list
	//		{
	//			while (it < reqList.size()) // && floor >= reqList[it])	// Start from the beginning of the vector and search for the closest request to the 
	//			{
	//				destFloor = reqList[it];													// elevator's current floor
	//				if (destFloor >= floor)
	//					it++;
	//				else it = reqList.size();		// Force termination
	//			}
	//		}
	//		else if (reqList.size() == 1) destFloor = reqList[0];	// If there is one element in the list, return that element
	//		else if (reqList.size() == 0 || it >= reqList.size()) destFloor = -1;			// If there is no element in the list, return an invalid value or next destinstion is not obtained
	//	}
	//	return destFloor;
	//}

};

