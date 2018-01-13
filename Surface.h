/*
Andrew's Ares Framework 2015
*/

#pragma once

#include "MiscDefinitions.h"
#include "ClientRecvProps.h"
#include "offsets.h"
#include "Vector.h"
#include "MiscClasses.h"
#include "Vector2D.h"

struct Vertex_t
{
	Vector2D	m_Position;
	Vector2D	m_TexCoord;

	Vertex_t() {}
	Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
};

typedef  Vertex_t FontVertex_t;

// Surface Drawing
class ISurface
{
public:
	void DrawSetColor(Color col)
	{
		typedef void(__thiscall* oDrawSetColor)(PVOID, Color);
		return call_vfunc< oDrawSetColor >(this, Offsets::VMT::Surface_DrawSetColorA)(this, col);
	}

	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_DrawSetColorB)(this, r, g, b, a);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_DrawFilledRect)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_DrawOutlinedRect)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_DrawLine)(this, x0, y0, x1, y1);
	}

	void DrawPolyLine(int *x, int *y, int count)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int *, int *, int);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_DrawLine + 1)(this, x, y, count);
	}

	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_DrawSetTextFont)(this, font);
	}

	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_DrawSetTextColorB)(this, r, g, b, a);
	}

	void DrawSetTextColor(Color col)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, Color);
		return call_vfunc< oDrawSetTextColor >(this, Offsets::VMT::Surface_DrawSetTextColorA)(this, col);
	}

	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_DrawSetTextPos)(this, x, y);
	}

	void DrawPrintText(const wchar_t* text, int textLen)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t*, int, int);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_DrawPrintText)(this, text, textLen, 0);
	}

	void DrawSetTexture(int textureID)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, int);
		return call_vfunc< oDrawSetTextColor >(this, 38)(this, textureID);
	}

	void DrawSetTextureRGBA(int textureID, unsigned char  const* colors, int w, int h)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, int, unsigned char  const*, int, int);
		return call_vfunc< oDrawSetTextColor >(this, 37)(this, textureID, colors, w, h);
	}

	int CreateNewTextureID(bool procedural)
	{
		typedef int(__thiscall* oDrawSetTextColor)(PVOID, bool);
		return call_vfunc< oDrawSetTextColor >(this, 43)(this, procedural);
	}

	void DrawTexturedPolygon(int vtxCount, FontVertex_t *vtx, bool bClipVertices = true)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, int, FontVertex_t*, bool);
		return call_vfunc< oDrawSetTextColor >(this, Offsets::VMT::Surface_DrawTexturedPolygon)(this, vtxCount, vtx, bClipVertices);
	}

	unsigned long FontCreate()
	{
		typedef unsigned int(__thiscall* OriginalFn)(PVOID);
		return call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_FontCreate)(this);
	}

	void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_SetFontGlyphSet)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const wchar_t*, int&, int&);
		call_vfunc< OriginalFn >(this, Offsets::VMT::Surface_GetTextSize)(this, font, text, wide, tall);
	}

	void DrawColoredCircle(int centerx, int centery, float radius, int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, float, int, int, int, int);
		getvfunc<OriginalFn>(this, 162)(this, centerx, centery, radius, r, g, b, a);
	}

	void DrawFilledCircle(int centerx, int centery, float radius, int r, int g, int b, int a)
	{
		for (int i = 0; i < radius; i++)
		{
			DrawColoredCircle(centerx, centery, i, r, g, b, a);
		}
	}

	void DrawOutlinedCircle(int x, int y, int r, int seg)
	{
		typedef void(__thiscall* oDrawOutlinedCircle)(PVOID, int, int, int, int);
		return call_vfunc< oDrawOutlinedCircle >(this, Offsets::VMT::Surface_DrawOutlinedCircle)(this, x, y, r, seg);
	}

	void SurfaceGetCursorPos(int &x, int &y)
	{
		typedef void(__thiscall* oSurfaceGetCursorPos)(PVOID, int&, int&);
		return call_vfunc< oSurfaceGetCursorPos >(this, Offsets::VMT::Surface_SurfaceGetCursorPos)(this, x, y);
	}

	inline void DrawTexturedRect(int x, int y, int w, int h) {
		typedef void(__thiscall *OrigFn)(void *, int, int, int, int);
		call_vfunc<OrigFn>(this, 41)(this, x, y, w, h);
	}

	void DrawT(int X, int Y, Color Color, int Font, bool Center, const char* _Input, ...)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };

		va_list Args;

		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);

		size_t Size = strlen(Buffer) + 1;

		wchar_t* WideBuffer = new wchar_t[Size];

		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		int Width = 0, Height = 0;

		if (Center)
		{
			GetTextSize(Font, WideBuffer, Width, Height);
		}

		DrawSetTextColor(Color.r(), Color.g(), Color.b(), Color.a());
		DrawSetTextFont(Font);
		DrawSetTextPos(X - (Width / 2), Y);
		DrawPrintText(WideBuffer, wcslen(WideBuffer));

		return;
	}
};