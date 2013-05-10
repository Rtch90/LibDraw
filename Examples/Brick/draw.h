#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>
#include <math.h>
#include <string.h>

// Call this to make sure everything is on display...
inline void d_flush() {
  fflush(stdout);
  return;
}

// Clears the screen to the specified background color
void d_clear(int color=0) {
  printf("\033[0,0H\033[0m\033[4%im\033[2J", color);
  return;
}

// Positions the cursor on the specified position - for later text output...
void d_goto(int x, int y) {
  if ( x<0 || y<0 ) return;
  ++x; ++y;
  printf("\033[%i;%iH", y, x);
  return;
}

void d_textcolor(int color, int background=0) {
  printf("\033[3%i;4%im", color, background);
}

// Draws a "pixel" on the specified position with a color from 0 to 7
// additionally you can specify a character to use on the drawn surface, and a text color
void d_putpixel(int x, int y, int color, char ch=' ', int textcolor=7) {
  if ( x<0 || y<0 ) return;
  ++x; ++y;
  char c2[2];
  c2[1]=0;
  c2[0]=ch;
  printf("\033[%i;%iH\033[4%im\033[3%im%s\033[0m", y, x, color, textcolor, c2);
  return;
}

// Draws a horizontal line
void d_hline(int x1, int y, int x2, int color=1, char ch=' ', int textcolor=7) {
  if ( x1<0 || x2<0 || x2<x1 || y<0 ) return;
  char hline[1024];
  memset(hline, ch, x2-x1+1);
  hline[x2-x1+1]=0;
  ++x1; ++y;
  printf("\033[%i;%iH\033[4%im\033[3%im%s\033[0m", y, x1, color, textcolor, hline);
  return;
}

// Draws a vertical line
void d_vline(int x, int y1, int y2, int color=1, char ch=' ', int textcolor=7) {
  if ( x<0 || y1<0 || y2<0 || y2<y1 ) return;
  char c2[2];
  c2[1]=0;
  c2[0]=ch;
  ++x; ++y1; ++y2;
  for (int y=y1; y<=y2; ++y) {
    printf("\033[%i;%iH\033[4%im\033[3%im%s\033[0m", y, x, color, textcolor, c2);
  }
  return;
}

// Draws a filled circle
// Remember, your terminal usually has 8x16 pixel fonts, so your vertical resolution is half of the horizontal, so this will look like an ellipse!!!
void d_fillcircle(int xc, int yc, int r, int color=1, char ch=' ', int textcolor=7) {
  for (int y=-r; y<=r; ++y) {
    int p=int( truncf( sqrtf( (r*r) - (y*y) ) + 0.00f ) );
    d_hline(xc-p, yc+y, xc+p, color, ch, textcolor);
  }
  return;
}

// Draws a circle
void d_circle(int xc, int yc, int r, int color=1, char ch=' ', int textcolor=7) {
  int prevp=int( truncf( sqrtf( (r*r) - (r*r) ) + 0.00f ) ); // Nasty trick to force the first line to be drawn normally
  for (int y=-r; y<0; ++y) {
    int p=int( truncf( sqrtf( (r*r) - (y*y) ) + 0.00f ) );
    for (int x=prevp; x<=p; ++x) {
      d_putpixel(xc-x, yc+y, color, ch, textcolor); // Top left
      d_putpixel(xc+x, yc+y, color, ch, textcolor); // Top right
      d_putpixel(xc-x, yc-y, color, ch, textcolor); // Bottom left
      d_putpixel(xc+x, yc-y, color, ch, textcolor); // Bottom right
    }
    prevp=p;
  }
  // Draw the circle's center horizontal line
  int p=int( truncf( sqrtf( (r*r) ) + 0.00f ) );
  d_putpixel(xc-p, yc, color, ch, textcolor);
  d_putpixel(xc+p, yc, color, ch, textcolor);
  return;
}

// Draws a filled rectangle
void d_fillrect(int x1, int y1, int x2, int y2, int color=1, char ch=' ', int textcolor=7) {
  if ( x1<0 || x2<0 || x2<x1 || y1<0 || y2<0 || y2<y1 ) return;
  for (int y=y1; y<=y2; ++y) {
    d_hline(x1, y, x2, color, ch, textcolor);
  }
  return;
}

// Draws a rectangle
void d_rect(int x1, int y1, int x2, int y2, int color=1, char ch=' ', int textcolor=7) {
  if ( x1<0 || x2<0 || x2<x1 || y1<0 || y2<0 || y2<y1 ) return;
  d_hline(x1, y1, x2, color, ch, textcolor);
  d_hline(x1, y2, x2, color, ch, textcolor);
  d_vline(x1, y1+1, y2-1, color, ch, textcolor);
  d_vline(x2, y1+1, y2-1, color, ch, textcolor);
  return;
}

void d_line(int x0, int y0, int x1, int y1, int color=1, char ch=' ', int textcolor=7 ) {
  int dy = y1 - y0;
  int dx = x1 - x0;
  int stepx, stepy;

  if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
  if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
  dy <<= 1;                                                  // dy is now 2*dy
  dx <<= 1;                                                  // dx is now 2*dx

  d_putpixel(x0, y0, color, ch, textcolor);
  if (dx > dy) {
    int fraction = dy - (dx >> 1);                         // Same as 2*dy - dx
    while (x0 != x1) {
      if (fraction >= 0) {
        y0 += stepy;
        fraction -= dx;                                // Same as fraction -= 2*dx
      }
      x0 += stepx;
      fraction += dy;                                   // Same as fraction -= 2*dy
      d_putpixel(x0, y0, color, ch, textcolor);
    }
  } else {
    int fraction = dx - (dy >> 1);
    while (y0 != y1) {
      if (fraction >= 0) {
        x0 += stepx;
        fraction -= dy;
      }
      y0 += stepy;
      fraction += dx;
      d_putpixel(x0, y0, color, ch, textcolor);
    }
  }
}

#endif
