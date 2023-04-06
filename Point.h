#pragma once

class Point
{

public:
	float x;
	float y;
	float z;


public:


	Point()
	{

	}
	Point(float ix , float iy, float iz)
	{
		x = ix;
		y = iy;
		z = iz;
	}

	Point(float ip[])
	{
		x = ip[0];
		y = ip[1];
		z = ip[2];
		
	}


	
private:


};

