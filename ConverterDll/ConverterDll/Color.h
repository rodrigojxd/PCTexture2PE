#pragma once

class Color {
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}
	Color(unsigned char R, unsigned char G, unsigned char B)
	{
		r = R;
		g = G;
		b = B;
		a = 255;
	}
	Color()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
};