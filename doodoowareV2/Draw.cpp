#include "Draw.h"

void Draw::drawPlayerBox(int left, int top, int right, int bottom, int thickness, Color c)
{
	int h = abs(bottom - top);
	int w = h * 0.5f;// abs(right - left);
	

	int x = left;
	int y = top;

	gInterface.surface->DrawSetColor(c.r(), c.g(), c.b(), c.a());
	gInterface.surface->DrawFilledRect(x, y, x + w, y + thickness);
	gInterface.surface->DrawFilledRect(x, y, x + thickness, y + h);
	gInterface.surface->DrawFilledRect(x + w - w, y + h - thickness, x + w, y + h);
	gInterface.surface->DrawFilledRect(x + w - thickness, y + h - h, x + w, y + h);

	//Outline
	gInterface.surface->DrawSetColor(0, 0, 0, 255);
	gInterface.surface->DrawOutlinedRect(x, y, x + w, y + h); //outside box
	gInterface.surface->DrawOutlinedRect(x + thickness, y + thickness, x + w - thickness, y + h - thickness); //inside box
	
}

void Draw::DrawFilledRect(int _x, int _y, int _width, int _height, Color _color)
{
	gInterface.surface->DrawSetColor(_color.r(), _color.g(), _color.b(), _color.a());
	gInterface.surface->DrawFilledRect(_x, _y, _x + _width, _y + _height);
}

void Draw::DrawHealth(int left, int top, int right, int bottom, int thickness, int hp)
{
	//if (health == 0) return;

	int x = left - 8;
	int h = fabs(top - bottom); //yes, i know...
	int y = top;
	int w = 4;

	float t = 1.0f - (min(max(hp, 0), 100) / 100.0f);
	int hpHeight = h * t;
	int green = int(hp * 2.55f);
	int red = 255 - green;

	gInterface.surface->DrawSetColor(red, green, 0, 255);
	//for (int i = 0; i < (hp / 10); i++)
	//{
		gInterface.surface->DrawFilledRect(x, y + (hpHeight), x + w, y+h);
	//}

	gInterface.surface->DrawSetColor(0, 0, 0, 255);
	gInterface.surface->DrawOutlinedRect(x, y, x + w, y + h);

}

void Draw::DrawString(int _x, int _y, char _alignment, const std::string &_text, HFont _font, Color _color)
{
	std::wstring wStr = std::wstring(_text.begin(), _text.end());
	const wchar_t* wcStr = wStr.c_str();

	int textWidth = 0;
	int textHeight = 0;
	gInterface.surface->GetTextSize(_font, wcStr, textWidth, textHeight);

	switch (_alignment)
	{
	case 'c':
		_x -= textWidth / 2;
		break;
	case 'r':
		_x -= textWidth;
		break;
	case 'l':
		break;
	}

	_y -= textHeight / 2;

	gInterface.surface->DrawSetTextFont(_font);
	gInterface.surface->DrawSetTextColor(_color.r(), _color.g(), _color.b(), _color.a());
	gInterface.surface->DrawSetTextPos(_x, _y);
	gInterface.surface->DrawPrintText(wcStr, _text.size());
}

void Draw::DrawLine(int x1, int y1, int x2, int y2, Color c) {
	gInterface.surface->DrawSetColor(c.r(), c.g(), c.b(), c.a());
	gInterface.surface->DrawLine(x1, y1, x2, y2);
}

void Draw::DrawOutlinedCircle(int _x, int _y, int _radius, int _segments, Color _color)
{
	gInterface.surface->DrawSetColor(_color.r(), _color.g(), _color.b(), _color.a());
	gInterface.surface->DrawOutlinedCircle(_x, _y, _radius, _segments);
}


HFont Draw::SetupFont(int _size, const std::string &_fontName, int _flags)
{
	HFont font = gInterface.surface->CreateFont();
	gInterface.surface->SetFontGlyphSet(font, _fontName.c_str(), _size, 0, 0, 0, _flags);

	return font;
}

HFont Draw::SetupFont(int _size, const std::string &_fontName, int weight, int _flags)
{
	HFont font = gInterface.surface->CreateFont();
	gInterface.surface->SetFontGlyphSet(font, _fontName.c_str(), _size, weight, 0, 0, _flags);

	return font;
}
