#include "draw.h"
#include "keys.h"
#include <unistd.h>
#include <time.h>

char bricks[80][24];

void drawscreen() {
  for (int y=0; y<=23; ++y)
    for (int x=0; x<=79; ++x) {
      d_putpixel(x,y,bricks[x][y]);
    }
  return;
}

int main() {
  d_clear(0);
  int x=40;
  int y=10;
  int dx=20;
  int dy=20;
  int oldx;
  int oldy;
  int olddx;
  int olddy;

  int slider=45;
  int sliderwidth=10;

  for (int i=0; i<=79; ++i) {
    bricks[i][0]=2;
    bricks[i][23]=4;

    bricks[i][1]=1;
    bricks[i][2]=1;
    bricks[i][3]=1;
    bricks[i][4]=1;
  }
  for (int i=0; i<=23; ++i) {
    bricks[0][i]=2;
    bricks[79][i]=2;
    bricks[25+i][6]=1;
  }

  drawscreen();
  d_hline(slider,22,slider+sliderwidth,6);

  int simpletimer=10;

  setcbreak();
  int ch=-1;
  while (ch==-1 || ch!='q') {
    usleep(10000);
    ch=getch();
    if (ch!=-1) {
      if (ch=='d' && slider+sliderwidth<78) {
        d_putpixel(slider, 22, 0);
        slider+=1;
        d_putpixel(slider+sliderwidth, 22, 6);
      }
      if (ch=='a' && slider>1) {
        d_putpixel(slider+sliderwidth, 22, 0);
        slider-=1;
        d_putpixel(slider, 22, 6);
      }
    }

    --simpletimer;
    if (simpletimer<=0) {
      simpletimer=10;
      oldx=x;
      oldy=y;
      x+=dx; y+=dy; // Tis is the movement
      d_putpixel(oldx,oldy,0);
      d_putpixel(x,y,3);

      olddx=dx;
      olddy=dy;
      if (bricks[x+dx][y]!=0) dx*=-1; // Check if solid was hit on horizontal axis
      if (bricks[x][y+dy]!=0) dy*=-1; // Check if solid was hit on vertical axis
      if (y+dy>=22 && dy>0 && x>=slider && x<=slider+sliderwidth) dy*=-1;

      if (bricks[x+olddx][y+olddy]==1) { // This is a breakable brick
         bricks[x+olddx][y+olddy]=0;
         d_putpixel(x+olddx,y+olddy,0); // Essentially erase it from the screen too
      }
      if (bricks[x+olddx][y]==1) { // This is a breakable brick
         bricks[x+olddx][y]=0;
         d_putpixel(x+olddx,y,0); // Essentially erase it from the screen too
      }
      if (bricks[x][y+olddy]==1) {// This is a breakable brick
         bricks[x][y+olddy]=0;
         d_putpixel(x,y+olddy,0); // Essentially erase it from the screen too
      }
    }
    d_goto(0,0);
    d_flush();
  }
  unsetcbreak();
  return 0;
}
