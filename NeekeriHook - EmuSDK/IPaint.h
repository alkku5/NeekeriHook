#pragma once
#include "SDK.h"

enum FontFeature
{
	FONT_FEATURE_ANTIALIASED_FONTS = 1,
	FONT_FEATURE_DROPSHADOW_FONTS = 2,
	FONT_FEATURE_OUTLINE_FONTS = 6,
};

enum FontDrawType
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

enum FontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

	typedef DWORD HFont;
	typedef unsigned int VPANEL;

	class ISurface
	{
	public:
		void DrawSetColor(Color c)
		{
			FUNCTION_GUARD;
			typedef void(__thiscall* _DrawSetColor)(void*, Color);
			return g_pUtils->Emulate<_DrawSetColor>(this, 14)(this, c);
		}

		void DrawRect(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
			return g_pUtils->Emulate< OriginalFn >(this, 16)(this, x0, y0, x1, y1);
		}

		void DrawLine(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
			return g_pUtils->Emulate< OriginalFn >(this, 19)(this, x0, y0, x1, y1);
		}

		void DrawOutlinedRect(int X, int Y, int W, int H)
		{
			FUNCTION_GUARD;
			typedef void(__thiscall* _DrawOutlinedRect)(void*, int, int, int, int);
			return g_pUtils->Emulate<_DrawOutlinedRect>(this, 18)(this, X, Y, W, H);
		}

		void DrawSetTextFont(int Font)
		{
			FUNCTION_GUARD;
			typedef void(__thiscall* _DrawSetTextFont)(void*, int);
			return g_pUtils->Emulate<_DrawSetTextFont>(this, 23)(this, Font);
		}

		void DrawSetTextColor(Color c)
		{
			FUNCTION_GUARD;
			typedef void(__thiscall* _DrawSetTextColor)(void*, Color);
			return g_pUtils->Emulate<_DrawSetTextColor>(this, 24)(this, c);
		}

		void DrawSetTextPos(int X, int Y)
		{
			FUNCTION_GUARD;
			typedef void(__thiscall* _DrawSetTextPos)(void*, int, int);
			return g_pUtils->Emulate<_DrawSetTextPos>(this, 26)(this, X, Y);
		}

		void DrawPrintText(wchar_t* _Input, int Length, int Type = 0)
		{
			FUNCTION_GUARD;
			typedef void(__thiscall* _DrawPrintText)(void*, wchar_t*, int, int);
			return g_pUtils->Emulate<_DrawPrintText>(this, 28)(this, _Input, Length, Type);
		}

		bool GetTextSize(int Font, const wchar_t* _Input, int& Wide, int& Tall)
		{
			FUNCTION_GUARD;
			typedef bool(__thiscall* _GetTextSize)(void*, int, const wchar_t*, int&, int&);
			return g_pUtils->Emulate< _GetTextSize >(this, 79)(this, Font, _Input, Wide, Tall);
		}

		HFont Create_Font()
		{
			FUNCTION_GUARD;
			typedef HFont(__thiscall* OriginalFn)(void*);
			return g_pUtils->Emulate< OriginalFn >(this, 71)(this);
		}

		bool SetFontGlyphSet(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0)
		{
			FUNCTION_GUARD;
			typedef bool(__thiscall* OriginalFn)(void*, HFont, const char*, int, int, int, int, int, int, int);
			g_pUtils->Log(" -- Resetting font '%s' at index 0x%x.", windowsFontName, font);
			return g_pUtils->Emulate< OriginalFn >(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
		}
	}; extern ISurface* g_pVGUISurface;
 

	struct matrix3x4_t;
	class IVDebugOverlay
	{
	private:
		virtual void            __unkn() = 0;
		virtual void            AddEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char *format, ...) = 0;
		virtual void            AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration) = 0;
		virtual void            AddSphereOverlay(const Vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
		virtual void            AddTriangleOverlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
		virtual void            AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
		virtual void            AddTextOverlay(const Vector& origin, float duration, const char *format, ...) = 0;
		virtual void            AddTextOverlay(const Vector& origin, int line_offset, float duration, const char *format, ...) = 0;
		virtual void            AddScreenTextOverlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char *text) = 0;
		virtual void            AddSweptBoxOverlay(const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const QAngle & angles, int r, int g, int b, int a, float flDuration) = 0;
		virtual void            AddGridOverlay(const Vector& origin) = 0;
		virtual void            AddCoordFrameOverlay(const matrix3x4_t& frame, float flScale, int vColorTable[3][3] = NULL) = 0;
	public:
		/* i use this for world2screen */
		virtual int             ScreenPosition(const Vector& point, Vector& screen) = 0;
	}; extern IVDebugOverlay* g_pDebugOverlay;


