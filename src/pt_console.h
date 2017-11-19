#ifndef PT_CONSOLE_H
#define PT_CONSOLE_H

#include <SDL2/SDL.h>

/* Include the STB image library with only the PNG support. */
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

/* Helper macros for working with pixel colors. */
#define RED(c) ((c & 0xff000000) >> 24)
#define GREEN(c) ((c & 0x00ff0000) >> 16)
#define BLUE(c) ((c & 0x0000ff00) >> 8)
#define ALPHA(c) (c & 0xff)
#define COLOR_FROM_RGBA(r, g, b, a) ((r << 24) | (g << 16) | (b << 8) | a)

/* Console helper types. */
typedef struct
{
  unsigned char glyph;
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
  unsigned char firstCharInAtlas;
} PT_Font;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t w;
  int32_t h;
} PT_Rect;

/* Screen. */
typedef struct
{
  uint32_t *pixels;
  uint32_t width;
  uint32_t height;
  uint32_t rowCount;
  uint32_t colCount;
  uint32_t cellWidth;
  uint32_t cellHeight;
  PT_Font *font;
  PT_Cell *cells;
} PT_Console;


/* Console. */
void PT_ConsoleClear (PT_Console * console);

PT_Console *PT_ConsoleInit (int32_t width, int32_t height,
                            int32_t rowCount, int32_t colCount);

void PT_ConsoleSetBitmapFont (PT_Console * console, char *filename,
                              unsigned char firstCharInAtlas,
                              int charWidth, int charHeight);

void PT_ConsolePutCharAt (PT_Console * console, unsigned char c,
                          int32_t cellX, int32_t cellY, uint32_t fgColor,
                          uint32_t bgColor);


/* Utilities. */
uint32_t PT_ColorizePixel (uint32_t dest, uint32_t src);

void PT_CopyBlend (uint32_t * destPixels, PT_Rect * destRect,
                   uint32_t destPixelsPerRow, uint32_t * srcPixels,
                   PT_Rect * srcRect, uint32_t srcPixelsPerRow,
                   uint32_t * newColor);

void PT_Fill (uint32_t * pixels, uint32_t pixelsPerRow,
              PT_Rect * destRect, uint32_t color);

void PT_FillBlend (uint32_t * pixels, uint32_t pixelsPerRow,
                   PT_Rect * destRect, uint32_t color);

PT_Rect PT_RectForGlyph (unsigned char c, PT_Font * font);

#endif
