/*
    Console Library

    Written by Peter de Tagyos
    Started: 12/24/2015
*/

#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "pt_console.h"

/* Console Function Implementation */

void
PT_ConsoleClear (PT_Console * con)
{
  PT_Rect r = { 0, 0, con->width, con->height };
  PT_Fill (con->pixels, con->width, &r, 0x000000ff);
}

PT_Console
*PT_ConsoleInit (int32_t width, int32_t height, int32_t rowCount,
                 int32_t colCount)
{

  PT_Console *console = malloc (sizeof (PT_Console));

  console->pixels = calloc (width * height, sizeof (uint32_t));
  console->width = width;
  console->height = height;
  console->rowCount = rowCount;
  console->colCount = colCount;
  console->cellWidth = width / colCount;
  console->cellHeight = height / rowCount;
  console->font = NULL;
  console->cells = calloc (rowCount * colCount, sizeof (PT_Cell));

  return console;
}

void
PT_ConsolePutCharAt (PT_Console * console, unsigned char c,
                     int32_t cellX, int32_t cellY, uint32_t fgColor,
                     uint32_t bgColor)
{

  int32_t x = cellX * console->cellWidth;
  int32_t y = cellY * console->cellHeight;
  PT_Rect destRect = {x, y, console->cellWidth, console->cellHeight};

  // Fill the background with alpha blending
  PT_FillBlend (console->pixels, console->width, &destRect, bgColor);

  // Copy the glyph with alpha blending and desired coloring
  PT_Rect srcRect = PT_RectForGlyph (c, console->font);
  PT_CopyBlend (console->pixels, &destRect, console->width,
                console->font->atlas, &srcRect, console->font->atlasWidth,
                &fgColor);
}

void
PT_ConsoleSetBitmapFont (PT_Console * con, char *filename,
                         unsigned char firstCharInAtlas,
                         int charWidth, int charHeight)
{

  // Load the image data
  int imgWidth, imgHeight, numComponents;
  unsigned char *imgData = stbi_load (filename,
                                      &imgWidth, &imgHeight,
                                      &numComponents, STBI_rgb_alpha);

  // Copy the image data so we can hold onto it
  uint32_t imgDataSize = imgWidth * imgHeight * sizeof (uint32_t);
  uint32_t *atlasData = malloc (imgDataSize);
  memcpy (atlasData, imgData, imgDataSize);

  // Create and configure the font
  PT_Font *font = malloc (sizeof (PT_Font));
  font->atlas = atlasData;
  font->charWidth = charWidth;
  font->charHeight = charHeight;
  font->atlasWidth = imgWidth;
  font->atlasHeight = imgHeight;
  font->firstCharInAtlas = firstCharInAtlas;

  stbi_image_free (imgData);

  if (con->font != NULL)
    {
      free (con->font->atlas);
      free (con->font);
    }
  con->font = font;
}


/* Utility Function Implementation */

inline uint32_t
PT_ColorizePixel (uint32_t dest, uint32_t src)
{
  // Colorize the destination pixel using the source color
  if (ALPHA (dest) == 255)
    {
      return src;
    }
  else if (ALPHA (dest) > 0)
    {
      // Scale the final alpha based on both dest & src alphas
      return COLOR_FROM_RGBA (RED (src),
                              GREEN (src),
                              BLUE (src),
                              (uint8_t) (ALPHA (src) *
                                         (ALPHA (dest) / 255.0)));
    }
  else
    {
      return dest;
    }
}

void
PT_CopyBlend (uint32_t * destPixels, PT_Rect * destRect,
              uint32_t destPixelsPerRow, uint32_t * srcPixels,
              PT_Rect * srcRect, uint32_t srcPixelsPerRow,
              uint32_t * newColor)
{
  // If src and dest rects are not the same size ==> bad things
  assert (destRect->w == srcRect->w && destRect->h == srcRect->h);

  // For each pixel in the destination rect, alpha blend to it the
  // corresponding pixel in the source rect.
  // ref: https://en.wikipedia.org/wiki/Alpha_compositing

  uint32_t stopX = destRect->x + destRect->w;
  uint32_t stopY = destRect->y + destRect->h;

  for (uint32_t dstY = destRect->y, srcY = srcRect->y;
       dstY < stopY; dstY++, srcY++)
    {
      for (uint32_t dstX = destRect->x, srcX = srcRect->x;
           dstX < stopX; dstX++, srcX++)
        {

          uint32_t srcColor = srcPixels[(srcY * srcPixelsPerRow) + srcX];
          uint32_t *destPixel = &destPixels[(dstY * destPixelsPerRow) + dstX];
          uint32_t destColor = *destPixel;

          // Colorize our source pixel before we blend it
          srcColor = PT_ColorizePixel (srcColor, *newColor);

          if (ALPHA (srcColor) == 0)
            {
              // Source is transparent - so do nothing
              continue;
            }
          else if (ALPHA (srcColor) == 255)
            {
              // Just copy the color, no blending necessary
              *destPixel = srcColor;
            }
          else
            {
              // Do alpha blending
              float srcA = ALPHA (srcColor) / 255.0;
              float invSrcA = (1.0 - srcA);
              float destA = ALPHA (destColor) / 255.0;

              float outAlpha = srcA + (destA * invSrcA);
              uint8_t fRed =
                ((RED (srcColor) * srcA) +
                 (RED (destColor) * destA * invSrcA)) / outAlpha;
              uint8_t fGreen =
                ((GREEN (srcColor) * srcA) +
                 (GREEN (destColor) * destA * invSrcA)) / outAlpha;
              uint8_t fBlue =
                ((BLUE (srcColor) * srcA) +
                 (BLUE (destColor) * destA * invSrcA)) / outAlpha;
              uint8_t fAlpha = outAlpha * 255;

              *destPixel = COLOR_FROM_RGBA (fRed, fGreen, fBlue, fAlpha);
            }
        }
    }
}

void
PT_Fill (uint32_t * pixels, uint32_t pixelsPerRow, PT_Rect * destRect,
         uint32_t color)
{
  uint32_t stopX = destRect->x + destRect->w;
  uint32_t stopY = destRect->y + destRect->h;

  for (uint32_t dstY = destRect->y; dstY < stopY; dstY++)
    {
      for (uint32_t dstX = destRect->x; dstX < stopX; dstX++)
        {
          pixels[(dstY * pixelsPerRow) + dstX] = color;
        }
    }
}

void
PT_FillBlend (uint32_t * pixels, uint32_t pixelsPerRow, PT_Rect * destRect,
              uint32_t color)
{
  // For each pixel in the destination rect, alpha blend the
  // bgColor to the existing color.
  // ref: https://en.wikipedia.org/wiki/Alpha_compositing

  uint32_t stopX = destRect->x + destRect->w;
  uint32_t stopY = destRect->y + destRect->h;

  // If the color we're trying to blend is transparent, then bail
  if (ALPHA (color) == 0)
    return;

  float srcA = ALPHA (color) / 255.0;
  float invSrcA = 1.0 - srcA;

  // Otherwise, blend each pixel in the dest rect
  for (uint32_t dstY = destRect->y; dstY < stopY; dstY++)
    {
      for (uint32_t dstX = destRect->x; dstX < stopX; dstX++)
        {
          uint32_t *pixel = &pixels[(dstY * pixelsPerRow) + dstX];

          if (ALPHA (color) == 255)
            {
              // Just copy the color, no blending necessary
              *pixel = color;
            }
          else
            {
              // Do alpha blending
              uint32_t pixelColor = *pixel;

              float destA = ALPHA (pixelColor) / 255.0;

              float outAlpha = srcA + (destA * invSrcA);
              uint8_t fRed =
                ((RED (color) * srcA) +
                 (RED (pixelColor) * destA * invSrcA)) / outAlpha;
              uint8_t fGreen =
                ((GREEN (color) * srcA) +
                 (GREEN (pixelColor) * destA * invSrcA)) / outAlpha;
              uint8_t fBlue =
                ((BLUE (color) * srcA) +
                 (BLUE (pixelColor) * destA * invSrcA)) / outAlpha;
              uint8_t fAlpha = outAlpha * 255;

              *pixel = COLOR_FROM_RGBA (fRed, fGreen, fBlue, fAlpha);
            }
        }
    }
}

PT_Rect
PT_RectForGlyph (unsigned char c, PT_Font * font)
{
  int32_t idx = c - font->firstCharInAtlas;
  int32_t charsPerRow = (font->atlasWidth / font->charWidth);
  int32_t xOffset = (idx % charsPerRow) * font->charWidth;
  int32_t yOffset = (idx / charsPerRow) * font->charHeight;

  PT_Rect glyphRect = { xOffset, yOffset, font->charWidth, font->charHeight };
  return glyphRect;
}
