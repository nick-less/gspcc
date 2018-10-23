#include <freelist.h>
#include <mm.h>
#include <string.h>
#include <dllist.h>

#pragma interface

// An abstract base class for all types of pixmap
class pixmap
{
public:
	virtual int isA(pixmap_t type) = 0;
	virtual void write(pixmap &dest, int destXoff, int destYoff,
									int srcXoff, int srcYoff) = 0;
	virtual size_t backingStoreSize(void) = 0;
	virtual int xPels(void) = 0;
	virtual int yPels(void) = 0;
	virtual int pitch(void) = 0;
	virtual unsigned char *origin(void) = 0;
};

typedef dllist<pixmap>			pixmapList;
typedef dllistIter<pixmap>		pixmapIter;
typedef enum
	{	e_binaryPixmap,
		e_linearPixmap,
		e_XYPixmap,
		e_displayListPixmap }	pixmap_t;
typedef int						color_t;

// A descriptor for a particular font.
class font
{
	FREELIST_DECLARE(font);
private:
	char *fontName_;
	int firstAscii_;
	int numChars_;
	int height_;
	mem *xPelArray_;			// How wide each char is
	mem *charOffsets_;			// Where each char starts in bitArray_
	mem *dataArray_;			// Table of data for all chars
public:
	font(char *_fontName, int _firstAscii, int _numChars, int _height,
					mem *_xPelArray, mem *_charOffsets, mem *_dataArray);
	char *fontName(void)			{ return fontName_; }
	int firstAscii(void)			{ return firstAscii_; }
	int numChars(void)				{ return numChars_; }
	int height(void)				{ return height_; }
	unsigned char *xPelArray(void)
	{
		return (unsigned char *)xPelArray_->addr();
	}
	unsigned long *charOffsets(void)
	{
		return (unsigned long *)charOffsets_->addr();
	}
	unsigned char *dataArray(void)
	{
		return (unsigned char *)dataArray_->addr();
	}
	int xPels(int asciiCode)
	{
		if (asciiCode < firstAscii())
			PANIC();
		asciiCode -= firstAscii();
		if (asciiCode >= numChars())
			PANIC;
		return xPelArray()[asciiCode];
	}
	unsigned long charOffset(int asciiCode)
	{
		if (asciiCode < firstAscii())
			PANIC();
		asciiCode -= firstAscii();
		if (asciiCode >= numChars())
			PANIC;
		return charOffsets()[asciiCode];
	}
	unsigned char *charData(int asciiCode)
	{
		return dataArray() + charOffset(asciiCode);
	}
};

// A concrete class for strings of chars
class fontString : public pixmap
{
	FREELIST_DECLARE(fontString);
private:
	mem *theString_;				// Ascii data
	font *theFont_;					// Font it's drawn in
	color_t fgcol_;
	color_t bgcol_;
public:
	char *theString(void)			{ return (char *)theString_->addr(); }
	font *theFont(void)				{ return theFont_; }
	color_t fgcol(void)				{ return fgcol_; }
	color_t bgcol(void)				{ return bgcol_; }
	fontString(mem *_theString, font *_theFont,
								color_t _fgcol, color_t _bgcol)
	{
		theString_ = _theString;
		theFont_ = _theFont;
		fgcol_ = _fgcol;
		bgcol_ = _bgcol;
	}
	fontString(char *_theString, font *_theFont,
								color_t _fgcol, color_t _bgcol)
	{
		theString_ = new mem(strlen(_theString) + 1);
		strcpy(theString(), _theString);
		theFont_ = _theFont;
		fgcol_ = _fgcol;
		bgcol_ = _bgcol;
	}
	~fontString(void)
	{
		delete theString_;
	}
	int isA(pixmap_t type)
	{
		return type == e_binaryPixmap;
	}
	void write(pixmap &dest, int destXoff, int destYoff,
									int srcXoff, int srcYoff);
	size_t backingStoreSize(void);
	color_t setFgcol(color_t _fgcol)		{ return fgcol_ = _fgcol; }
	color_t setBgcol(color_t _bgcol)		{ return bgcol_ = _bgcol; }
	int xPels(void);
	int yPels(void);
	int pitch(void);
	unsigned char origin(void);
};

// An abstract class for representing linear or XY pixmaps
class pelPixmap : public pixmap
{
private:
	int xPels_;		// Displayed width
	int yPels_;		// Displayed height
	int pitch_;		// Number of bits between adjacent rows
					//	A multiple of 16 for linear pixmaps
					//	A power of 2 for XY pixmaps
					//	Really this is the number of bits between adjacent
					//		columns for rotated pixmaps
public:
	void pelPixmap(int _xPels, int _yPels, int _pitch)
	{
		xPels_ = _xPels;
		yPels_ = _yPels;
		pitch_ = _pitch;
	}
	virtual unsigned char *origin(void) = 0;
	int xPels(void)				{ return xPels_; }
	int yPels(void)				{ return yPels_; }
	int pitch(void)				{ return pitch_; }
};

// A linear pixmap concrete class
class linearPixmap : public pelPixmap
{
	FREELIST_DECLARE(linearPixmap);
private:
	mem *theMem_;			// Where its binary data is stored
public:
	linearPixmap(	mem *_theMem,
					int _xPels, int _yPels, int _pitch) :
				pelPixmap(_xPels, _yPels, _pitch)
	{
		theMem_ = _theMem;
	}
	int isA(pixmap_t type)
	{
		return type == e_linearPixmap;
	}
	void write(pixmap &dest, int destXoff, int destYoff,
									int srcXoff, int srcYoff);
	size_t backingStoreSize(void);
	mem *theMem(void)		{ return theMem_; }
	unsigned char *origin(void)
	{
		return (unsigned char *)theMem_->addr();
	}
};

// An XY pixmap concrete class
class XYPixmap : public pelPixmap
{
	FREELIST_DECLARE(XYPixmap);
private:
	unsigned char *origin_;		// The screen origin on which it is based
public:
	XYPixmap(		unsigned char *_origin,
					int _xPels, int _yPels, int _pitch) :
			pelPixmap(_xPels, _yPels, _pitch)
	{
		origin_ = _origin;
	}
	int isA(pixmap_t type)
	{
		return type == e_XYPixmap;
	}
	void write(pixmap &dest, int destXoff, int destYoff,
									int srcXoff, int srcYoff);
	size_t backingStoreSize(void);
	unsigned char *origin(void)		{ return origin_; }
};

// A concrete class for a list of pixmaps
// A list of pixmaps is itself a pixmap type
class displayList : public pixmap
{
	FREELIST_DECLARE(displayList);
private:
	pixmapList *theList_;
public:
	displayList(pixmapList *_theList)
	{
		theList_ = _theList;
	}
	int isA(pixmap_t type)
	{
		return type == e_displayListPixmap;
	}
	void write(pixmap &dest, int destXoff, int destYoff,
									int srcXoff, int srcYoff);
	size_t backingStoreSize(void);
	pixmapList *theList(void)		{ return theList_; }
	int xPels(void);
	int yPels(void);
	int pitch(void);
	unsigned char *origin(void);
};

