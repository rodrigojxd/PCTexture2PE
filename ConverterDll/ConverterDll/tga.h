#pragma once

//includes
#include <string>
#include <fstream>
#include <iostream>
#include "Color.h"

using namespace std;

//data structure

class TGAImage {

public:

	//Constructor
	TGAImage();

	//Overridden Constructor
	TGAImage(short width, short height);

	//Set all pixels at once
	void setAllPixels(Color *pixels);

	//set individual pixels
	void setPixel(Color inputcolor, int xposition, int yposition);

	void WriteImage(string filename);

private:

	//store the pixels
	Color *m_pixels;

	short m_height;
	short m_width;

	//convert 2D to 1D indexing
	int convert2dto1d(int x, int y);
};
