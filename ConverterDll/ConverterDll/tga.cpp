//#include "stdafx.h"
#include "tga.h"

//Default Constructor
TGAImage::TGAImage() {

}

//Overridden Constructor
TGAImage::TGAImage(short width, short height) {
	m_width = width;
	m_height = height;
	m_pixels = new Color[m_width*m_height];
}

//Set all pixels at once
void TGAImage::setAllPixels(Color *pixels) {
	m_pixels = pixels;
}

//Set indivdual pixels
void TGAImage::setPixel(Color inputcolor, int x, int y) {
	m_pixels[convert2dto1d(x,y)] = inputcolor;
}

//Convert 2d array indexing to 1d indexing
int TGAImage::convert2dto1d(int x, int y) {
	return m_width * x + y;
}

void TGAImage::WriteImage(string filename) {

	//Error checking
	if (m_width <= 0 || m_height <= 0)
	{
		cout << "Image size is not set properly" << endl;
		return;
	}

	ofstream o(filename.c_str(), ios::out | ios::binary);

	//Write the header
	o.put(0);
   	o.put(0);
   	o.put(2);                         /* uncompressed RGB */
   	o.put(0); 		o.put(0);
   	o.put(0); 	o.put(0);
   	o.put(0);
   	o.put(0); 	o.put(0);           /* X origin */
   	o.put(0); 	o.put(0);           /* y origin */
   	o.put((m_width & 0x00FF));
   	o.put((m_width & 0xFF00) / 256);
   	o.put((m_height & 0x00FF));
   	o.put((m_height & 0xFF00) / 256);
   	o.put(32);                        /* 24 bit bitmap */
   	o.put(0);
   	
	//Write the pixel data
	for (int i=0;i<m_height*m_width;i++) {
		o.put(m_pixels[i].b);
		o.put(m_pixels[i].g);
		o.put(m_pixels[i].r);
		o.put(m_pixels[i].a);
	}   
	
	//close the file
	o.close();
	
}