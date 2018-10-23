#include <graphics.h>
#include <stdio.h>
#include <panic.h>

FREELIST_DEFINE(font, 100);
FREELIST_DEFINE(fontString, 100);
FREELIST_DEFINE(linearPixmap, 100);
FREELIST_DEFINE(XYPixmap, 100);
FREELIST_DEFINE(displayList, 100);

//
// fontString methods
//

void font::font(char *_fontName, int _firstAscii, int _numChars, int _height,
					mem *_xPelArray, mem *_charOffsets, mem *_dataArray);
{
	fontName_ = _fontName;
	firstAscii_ = _firstAscii;
	numChars_ = _numChars;
	height_ = _height;
	xPelArray_ = _xPelArray;
	charOffsets_ = _charOffsets;
	dataArray_ = _dataArray;
}

void fontString::write(pixmap &dest, int destXoff, int destYoff,
									int srcXoff, int srcYoff)
{
	char *str = theString;

	if (dest.isA(e_onScreenPixmap))
	{
		font *cFont = theFont();
		COLOR0 = bgcol();
		COLOR1 = fgcol();
		DPTCH = dest.pitch();
		*CONVDP = lmo(DPTCH);
		OFFSET = dest.origin();
#if 0
		WSTART = dydx(0, 0);
		WEND = dydx(	ROTATE_SELECT(
								dest.yPels(),
								dest.xPels()) - 1,
						ROTATE_SELECT(
								dest.xPels(),
								dest.yPels()) - 1);
#endif
		setupControl(PM_SRC, 0, 0);
		while (*str)
		{	// pixblt b,xy
			unsigned long c = *str++ & 0xff;
			SADDR = cfont->charData(c);
			SPTCH = PAD16(xPels(c));
			DYDX = dydx(	ROTATE_SELECT(
									cFont->height(),
									cFont->xPels(c)),
							ROTATE_SELECT(
									cFont->xPels(c),
									cFont->height()));
			DADDR = dydx(	ROTATE_SELECT(
									destYoff,
									destXoff),
							ROTATE_SELECT(
									destXoff,
									dest.yPels() - 1 - destYoff));
			pixbltBXY();
			destXoff += theFontChar->xPels;
		}
	}
	else
		PANIC();
}

size_t fontString::backingStoreSize(void)
{
	PANIC();
}

int fontString::xPels(void)
{
	PANIC();
}

int fontString::yPels(void)
{
	PANIC();
}

int fontString::pitch(void)
{
	PANIC();
}

unsigned char *fontString::origin(void)
{
	PANIC();
}

//
// linearPixmap methods
//
void linearPixmap::write(pixmap &dest, int destXoff, int destYoff,
							int srcXoff, int srcYoff)
{
	if (dest.isA(e_linearPixmap))
	{	// Do a pixblt l,l
		DBASSERT(destXoff == 0);
		DBASSERT(destYoff == 0);
		DBASSERT(srcXoff == 0);
		DBASSERT(srcYoff == 0);
		SADDR = origin();
		SPTCH = pitch();
		DADDR = dest.origin();
		DPTCH = dest.pitch();
		DBASSERT(xPels() == dest.xPels());
		DBASSERT(yPels() == dest.yPels());
		DYDX = dydx(yPels(), xPels());
		setupControl(PM_SRC, 0, 0);
		pixbltLL();
	}
	else if (dest.isA(e_XYPixmap))
	{	// Do a pixblt l,xy
		DBASSERT(srcXoff == 0);
		DBASSERT(srcYoff == 0);
		SADDR = origin();
		SPTCH = pitch();
		DADDR = dydx(	ROTATE_SELECT(
								destYoff,
								destXoff),
						ROTATE_SELECT(
								destXoff,
								dest.yPels() - 1 - destYoff));
		DPTCH = dest.pitch();
		OFFSET = dest.origin();
#if 0
		WSTART = dydx(0, 0);
		WEND = dydx(	ROTATE_SELECT(
								dest.yPels(),
								dest.xPels()) - 1,
						ROTATE_SELECT(
								dest.xPels(),
								dest.yPels()) - 1);
#endif
		DYDX = dydx(	ROTATE_SELECT(
								yPels(),
								xPels()),
						ROTATE_SELECT(
								xPels(),
								yPels()));
		setupControl(PM_SRC, 0, 0);
		*CONVDP = lmo(dest.pitch());
		pixbltLXY();
	}
	else
		PANIC();
}

size_t linearPixmap::backingStoreSize(void)
{
	PANIC();
}

//
// XYPixmap methods
//

void XYPixmap::write(pixmap &dest, int destXoff, int destYoff,
							int srcXoff, int srcYoff)
{
	if (dest.isA(e_linearPixmap))
	{	// pixblt xy,l
		DBASSERT(destXoff == 0);
		DBASSERT(destYoff == 0);
		SADDR = dydx(	ROTATE_SELECT(
								srcYoff,
								srcXoff),
						ROTATE_SELECT(
								srcXoff,
								yPels() - 1 - srcYoff));
		SPTCH = pitch();
		DADDR = dest.origin();
		DPTCH = dest.pitch();
		OFFSET = origin();
		DYDX = dydx(	ROTATE_SELECT(
								yPels(),
								xPels()),
						ROTATE_SELECT(
								xPels(),
								yPels()));
		setupControl(PM_SRC, 0, 0);
		*CONVSP = lmo(pitch());
		*CONVSP;
		pixbltXYL();
	}
	else if (dest.isA(e_XYPixmap))
	{	// pixblt xy,xy
		DBASSERT(origin() == dest.origin());
		SADDR = dydx(	ROTATE_SELECT(
								srcYoff,
								srcXoff),
						ROTATE_SELECT(
								srcXoff,
								yPels() - 1 - srcYoff));
		SPTCH = pitch();
		DADDR = dydx(	ROTATE_SELECT(
								destYoff,
								destXoff),
						ROTATE_SELECT(
								destXoff,
								dest.yPels() - 1 - destYoff));
		DPTCH = dest.pitch();
		OFFSET = origin();
#if 0
		WSTART = dydx(0, 0);
		WEND = dydx(	ROTATE_SELECT(
								yPels(),
								xPels()) - 1,
						ROTATE_SELECT(
								xPels(),
								yPels()) - 1);
#endif
		DYDX = dydx(	ROTATE_SELECT(
								0,
								0),
						ROTATE_SELECT(
								0,
								0));
		setupControl(PM_SRC, 0, 0);
		*CONVSP = lmo(SPTCH);
		*CONVDP = lmo(DPTCH);
		*CONVDP;
		pixbltXYXY();
	}
	else
		PANIC();
}

size_t XYPixmap::backingStoreSize(void)
{
	PANIC();
}
