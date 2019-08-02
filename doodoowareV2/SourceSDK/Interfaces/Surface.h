#pragma once

#ifdef CreateFont
#undef CreateFont
#endif

class ISurface 
{
public:
	inline void DrawSetColor(int _red, int _green, int _blue, int _alpha);
	inline void DrawLine(int _x1, int _y1, int _x2, int _y2);
	inline void DrawOutlinedRect(int _x1, int _y1, int _x2, int _y2);
	inline void DrawFilledRect(int _x1, int _y1, int _x2, int _y2);
	inline HFont CreateFont();
	inline bool SetFontGlyphSet(HFont _font, const char* _fontName, int _tall, int _weight, int _blur, int _scanlines, int _flags, int _nRangeMin = 0, int _nRangeMax = 0);
	inline void DrawSetTextFont(HFont _font);
	inline void DrawSetTextColor(int _red, int _green, int _blue, int _alpha);
	inline void DrawSetTextPos(int _x, int _y);
	inline void DrawPrintText(const wchar_t* _text, int _textLen, int _drawType = 0);
	inline void UnlockCursor();
	inline void GetTextSize(HFont _font, const wchar_t* _text, int &_wide, int &_tall);
	inline void IPlaySound(const char * fileName);
	inline void SurfaceGetCursorPos(int &_x, int &_y);
	inline void DrawOutlinedCircle(int x, int y, int radius, int segments);
};

inline void ISurface::DrawSetColor(int _red, int _green, int _blue, int _alpha)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::DrawSetColor, _red, _green, _blue, _alpha);
}


inline void ISurface::DrawLine(int _x1, int _y1, int _x2, int _y2)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::DrawLine, _x1, _y1, _x2, _y2);
}

inline void ISurface::DrawOutlinedRect(int _x1, int _y1, int _x2, int _y2)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::DrawOutlinedRect, _x1, _y1, _x2, _y2);
}

inline void ISurface::DrawFilledRect(int _x1, int _y1, int _x2, int _y2)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::DrawFilledRect, _x1, _y1, _x2, _y2);
}

inline HFont ISurface::CreateFont()
{
	return gInterface.CallVirtualFunction<HFont>(this, Indexes::Surface::CreateFont);
}

inline bool ISurface::SetFontGlyphSet(HFont _font, const char* _fontName, int _tall, int _weight, int _blur, int _scanlines, int _flags, int _nRangeMin, int _nRangeMax)
{
	return gInterface.CallVirtualFunction<bool>(this, Indexes::Surface::SetFontGlyphSet, _font, _fontName, _tall, _weight, _blur, _scanlines, _flags, _nRangeMin, _nRangeMax);
}

inline void ISurface::DrawSetTextFont(HFont _font)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::DrawSetTextFont, _font);
}

inline void ISurface::DrawSetTextColor(int _red, int _green, int _blue, int _alpha)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::DrawSetTextColor, _red, _green, _blue, _alpha);
}

inline void ISurface::DrawSetTextPos(int _x, int _y)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::DrawSetTextPos, _x, _y);
}

inline void ISurface::DrawPrintText(const wchar_t* _text, int _textLen, int _drawType)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::DrawPrintText, _text, _textLen, _drawType);
}

inline void ISurface::UnlockCursor()
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::UnlockCursor);
}

inline void ISurface::GetTextSize(HFont _font, const wchar_t* _text, int &_wide, int &_tall)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::GetTextSize, _font, _text, std::ref(_wide), std::ref(_tall));
}

inline void ISurface::IPlaySound(const char* fileName)
{
	return gInterface.CallVirtualFunction<void>(this, 82, fileName);
}

inline void ISurface::SurfaceGetCursorPos(int &_x, int &_y)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::SurfaceGetCursorPos, std::ref(_x), std::ref(_y));
}

inline void ISurface::DrawOutlinedCircle(int x, int y, int radius, int segments)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::Surface::DrawOutlinedCircle, x, y, radius, segments);
}
