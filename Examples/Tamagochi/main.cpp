#include "draw.h"
#include "keys.h"

#include <unistd.h>
#include <time.h>

void tamagochi();

int main() {
  d_clear(6);

  for (int i=0; i<10; ++i) {
    d_putpixel(10+i, 10, 3, 'x', 1);
    d_putpixel(10+i, 20, 3);
  }
  for (int i=1; i<=9; ++i) {
    d_putpixel(10, 10+i, 3);
    d_putpixel(19, 10+i, 3);
  }

  d_fillrect(11, 11, 18, 19, 5, '.', 1);

  d_hline(0,2,9, 4, 'A', 1);
  d_vline(0,4,20, 4, 'B');

  d_rect(4,4,6,6, 4, 'R');

  d_putpixel(1 ,1 , 4, '1', 1);
  d_putpixel(0 ,0 , 4, '0', 2);

  d_circle(10, 10, 3, 2);

// End of technical demo, here comes the tamagochi fun:

//-- RUN Tamagochi!!! ------------------------------------------------------------
  tamagochi();
  
//-- Fireworks!!! ----------------------------------------------------------------
  d_fillcircle(30, 7, 1, 1);
  d_goto(0,0); d_flush(); usleep(500000);

  d_fillcircle(30, 7, 2, 1);
  d_fillcircle(35, 2, 1, 2);
  d_goto(0,0); d_flush(); usleep(500000);

  d_fillcircle(30, 7, 3, 1);
  d_fillcircle(35, 2, 2, 2);
  d_goto(0,0); d_flush(); usleep(50000);

  d_fillcircle(35, 2, 3, 2);
  d_goto(0,0); d_flush(); usleep(50000);
  
  d_putpixel(30, 7, 6);
  d_goto(0,0); d_flush(); usleep(50000);

  d_putpixel(35, 2, 6);
  d_goto(0,0); d_flush(); usleep(50000);

  d_putpixel(37, 3, 6);
  d_putpixel(29, 5, 6);
  d_goto(0,0); d_flush(); usleep(50000);

  d_putpixel(34, 8, 6);
  d_putpixel(32, 4, 6);
  d_goto(0,0); d_flush(); usleep(50000);

  d_fillcircle(30, 7, 3, 6);
  d_fillcircle(35, 2, 3, 6);
  d_goto(0,0); d_flush(); usleep(50000);

  d_goto(0, 23);
}

void drawtamagochi(int mood=1) {
  d_fillcircle(50, 10, 10, 3); // Head
  if (mood!=3) d_fillcircle(45, 7, 2, 4); // Left eye
  if (mood!=3) d_fillcircle(55, 7, 2, 4); // Right eye
  d_fillcircle(50, 12, 1, 0); // Nose
  if (mood==1) {
    d_hline(45, 16, 55, 1, ' ', 7); // Mouth
    // Make it smile a bit...
    d_putpixel(44, 15, 1);
    d_putpixel(56, 15, 1);
  }
  else if (mood==2 || mood==3) {
    d_hline(45, 15, 55, 1, ' ', 7); // Mouth
    d_putpixel(44, 16, 1);
    d_putpixel(56, 16, 1);
  }
  // Draw some ears, as tamagochies are often animals :)
  d_vline(42,0,3, 3);
  d_vline(43,1,3, 3);
  d_vline(58,0,3, 3);
  d_vline(57,1,3, 3);
  // Some hairs around the nose
  d_hline(52,12,54, 3, '-', 0);
  d_hline(55,12,58, 3, '=', 0);
  d_hline(42,12,45, 3, '=', 0);
  d_hline(46,12,48, 3, '-', 0);
  // Dead?
  if (mood==3) {
    for (int i=0; i<=4; ++i) {
      d_putpixel(43+i,5+i,0);
      d_putpixel(53+i,5+i,0);
      d_putpixel(47-i,5+i,0);
      d_putpixel(57-i,5+i,0);
    }
  }
  // Position the cursor and flush the display
  d_goto(0, 0);
  d_flush();
}

void blinktamagochi() {
  d_fillcircle(45, 7, 2, 3); // Erase left eye so we can draw a line instead there
  d_fillcircle(55, 7, 2, 3); // As above - Right eye. no :P as 45,7 is the coordinate for the left eye  - head color==3 | eye color==4 | 45,7 are coordinates, 2 is radius
  d_hline(43, 7, 47, 4); // Slits of the eyes
  d_hline(53, 7, 57, 4);
  d_goto(0, 0);
  d_flush();
  sleep(1);

  d_fillcircle(45, 7, 2, 4); // Redraw left eye
  d_fillcircle(55, 7, 2, 4); // Redraw right eye
  d_goto(0, 0);
  d_flush();
}

void tamagochi() {
  int hunger=98;
  int mood=1;
  int prevmood=mood;
  drawtamagochi();
  
  time_t lasttime;
  time(&lasttime);
  time_t timenow;

  bool updatestatus=true;

  d_fillrect(64,4,75,6,1);
  d_goto(64, 5); d_textcolor(0, 1); printf(" <-- LOLCAT ");

  setcbreak();
  int ch=-1;
  while (ch==-1 || ch!='q') {
    // Do things with the tamagochi
    time(&timenow);
    if (timenow-lasttime>2) {
      lasttime=timenow;
      if (hunger<200) ++hunger;
      if (hunger>100 && mood==1) {
          mood=2;
          drawtamagochi(mood);
      }
      else if (hunger<=100 && mood==2) {
        mood=1;
        drawtamagochi(mood);
      }
      else if (hunger==200) {
        mood=3;
        drawtamagochi(mood);
      }
      // Print status
      prevmood=mood;
      updatestatus=true;
    }

    if (updatestatus) {
      d_goto(30, 21); d_textcolor(3,5); printf("Hunger: %i ", hunger);
      d_goto(0, 0);
      d_flush();
      updatestatus=false;
    }

    usleep(1);
    ch=getch();
    if (ch!=-1) {
      if (ch=='f') {
        if (hunger>0) hunger-=3;
        if (hunger<0) hunger=0;
        d_hline(45, 15, 55, 1);
        d_hline(45, 16, 55, 1);
        d_hline(47, 16, 53, 0);
        d_hline(45, 17, 55, 1);
        d_goto(0, 0);
        d_flush();
        sleep(1);
        drawtamagochi(mood);
        updatestatus=true;
      }
      else if (ch=='k') {
	      hunger=200;
	      mood=3;
	      drawtamagochi(mood);
	      updatestatus=true;
      }
      else if (ch=='r') {
	      hunger=0;
        mood=1;
	      drawtamagochi(mood);
	      updatestatus=true;
      }
    }
  }
  unsetcbreak();

  blinktamagochi();
}
