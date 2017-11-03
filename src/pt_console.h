#ifndef PT_CONSOLE_H
#define PT_CONSOLE_H

#include <SDL2/SDL.h>

typedef unsigned char ascii_char;

/* Include the STB image library - only the PNG support */
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

/* Helper macros for working with pixel colors */
#define RED(c) ((c & 0xff000000) >> 24)
#define GREEN(c) ((c & 0x00ff0000) >> 16)
#define BLUE(c) ((c & 0x0000ff00) >> 8)
#define ALPHA(c) (c & 0xff)
#define COLOR_FROM_RGBA(r, g, b, a) ((r << 24) | (g << 16) | (b << 8) | a)

/* Console Helper Types */
typedef struct
{
  ascii_char glyph;
  uint32_t fgColor;
  uint32_t bgColor;
} PT_Cell;


typedef struct
{
  uint32_t *atlas;
  uint32_t atlasWidth;
  uint32_t atlasHeight;
  uint32_t charWidth;
  uint32_t charHeight;
  ascii_char firstCharInAtlas;
} PT_Font;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t w;
  int32_t h;
} PT_Rect;

typedef struct
{
  uint32_t *pixels;             /* screen */
  uint32_t width;
  uint32_t height;
  uint32_t rowCount;
  uint32_t colCount;
  uint32_t cellWidth;
  uint32_t cellHeight;
  PT_Font *font;
  PT_Cell *cells;
} PT_Console;


/* Console Functions */
void PT_ConsoleClear (PT_Console * con);

PT_Console *PT_ConsoleInit (int32_t width, int32_t height,
                            int32_t rowCount, int32_t colCount);

void PT_ConsoleSetBitmapFont (PT_Console * con, char *filename,
                              ascii_char firstCharInAtlas,
                              int charWidth, int charHeight);

void PT_ConsolePutCharAt (PT_Console * con, ascii_char c,
                          int32_t cellX, int32_t cellY, uint32_t fgColor,
                          uint32_t bgColor);


/* Utility Functions */
uint32_t PT_ColorizePixel (uint32_t dest, uint32_t src);

void PT_CopyBlend (uint32_t * destPixels, PT_Rect * destRect,
                   uint32_t destPixelsPerRow, uint32_t * srcPixels,
                   PT_Rect * srcRect, uint32_t srcPixelsPerRow,
                   uint32_t * newColor);

void PT_Fill (uint32_t * pixels, uint32_t pixelsPerRow,
              PT_Rect * destRect, uint32_t color);

void PT_FillBlend (uint32_t * pixels, uint32_t pixelsPerRow,
                   PT_Rect * destRect, uint32_t color);

PT_Rect PT_RectForGlyph (ascii_char c, PT_Font * font);

#endif
