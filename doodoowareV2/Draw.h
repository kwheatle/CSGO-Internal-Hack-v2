#pragma once
#include "hooks.h"

class Draw
{
public:
	void drawPlayerBox(int x1, int y1, int x2, int y2, int thickness ,Color c);
	void DrawFilledRect(int _x, int _y, int _width, int _height, Color _color);
	void DrawHealth(int left, int top, int right, int bottom, int thickness, int health);
	void DrawString(int _x, int _y, char _alignment, const std::string &_text, HFont _font, Color _color);
	void DrawLine(int x1, int y1, int x2, int y2, Color c);
	void DrawOutlinedCircle(int _x, int _y, int _radius, int _segments, Color _color);
	HFont SetupFont(int _size, const std::string & _fontName, int _flags);
	HFont SetupFont(int _size, const std::string & _fontName, int weight, int _flags);
	//void drawLine(int x1, int y1, int x2, int y2, int thickness, Color c);

private:

};
extern Draw draw;