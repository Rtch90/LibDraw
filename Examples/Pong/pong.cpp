#include "draw.h"
#include "keys.h"
#include <unistd.h>
#include <time.h>

int main() {
  printf("\033[?25l"); // Hide the cursor

  d_clear(0);

  int x=40<<3; // We store the upscaled coordinates here, divide this by 4 to get real screen coordinates
  int y=10<<3;
  int dx=4;    // Direction on x axis, upscaled coordinate compatible value
  int dy=1;    // Direction on y axis, upscaled coordinate compatible value
  int oldx;    // Screen coordinate for erasing the ball
  int oldy;
  int newx;    // Used for converting back to the "screen" coordinates before checking wall hits and paddle hits
  int newy;

  int p1=10;   // Paddle 1's y coordinate - this is a screen coordinate
  int p2=10;
#define pw 5

  int p1score=0;
  int p2score=0;

  d_vline( 1, p1, p1+pw, 6);
  d_vline(78, p2, p2+pw, 6);

  int simpletimer=6;

  setcbreak();
  int ch=-1;
  while (ch==-1 || ch!='q') {
    usleep(3000);
    ch=getch();
    if (ch!=-1) {
      // Handles for the first paddle
      if (ch=='w' && p1>0) {
        d_putpixel(1, p1+pw, 0);
        --p1;
        d_putpixel(1, p1, 6);
      }
      if (ch=='s' && p1+pw<23) {
        d_putpixel(1, p1, 0);
        ++p1;
        d_putpixel(1, p1+pw, 6);
      }
      // Handles for the second paddle
      if (ch=='o' && p2>0) {
        d_putpixel(78, p2+pw, 0);
        --p2;
        d_putpixel(78, p2, 6);
      }
      if (ch=='l' && p2+pw<23) {
        d_putpixel(78, p2, 0);
        ++p2;
        d_putpixel(78, p2+pw, 6);
      }
    }

    --simpletimer;
    if (simpletimer<=0) {
      simpletimer=6;

      oldx=x>>3;
      oldy=y>>3;
      newx=(x+dx)>>3; // Now we can check screen objects against these for collision
      newy=(y+dy)>>3;

      if (newx==1 && newy>=p1 && newy<=p1+pw) dx*=-1; // Check if first paddle would be hit, if it is, reverse the x axis movement direction
      if (newx==78 && newy>=p2 && newy<=p2+pw) dx*=-1; // Check if second paddle...
      if (newy>23 || newy<0) dy*=-1; // If the upper or lower border would be crossed by this move, reverse the y axis movement

      if (newx<0 || newx>79) {// The ball went out on one side, place it on the center of the loser's paddle and let it fly
        d_goto(30,12); // Prepare the drawing cursor for text output at this position
        if (newx<40) {// The ball was on the left side when it exited the playable area, increment player 2's score, and put the ball on player 1's paddle
	        ++p2score;
	        newx=2;
	        newy=p1+(pw/2);

	        printf("Player 2 scores!");
	      } else { // Otherwise it was on the right side, increment player 1 score, put ball on player 2's paddle
	        ++p1score;
	        newx=77;
	        newy=p2+(pw/2);

	        printf("Player 1 scores!");
	      }
        dx*=-1; // Remember that the ball was headed outwards the last time we saw it, so we have to switch the horizontal direction
	      x=newx<<3; // And scale up the screen coordinates for later use
	      y=newy<<3;

        // Wait for a bit then erase the text
        d_goto(0,0);
	      d_flush();
        usleep(1000000);
        d_goto(30,12); printf("                ");
        d_goto(0,0);
	      d_flush();
      }

      x+=dx; y+=dy; // This is the movement

      d_putpixel(oldx, oldy, 0);
      d_putpixel(x>>3, y>>3, 3); // Remember, x and y stores upscaled coordinates, so we have to divide them by 4 for drawing  [also remember that a bit shift to the right by 3 equals to that divide]
    }
    d_goto(10,0); printf("%i", p1score);
    d_goto(50,0); printf("%i", p2score);
    d_goto(0,0);
    d_flush();
  }
  unsetcbreak();

  printf("\033[?25h"); // Unhide the cursor
  d_clear();
  d_goto(0,0);
  d_flush();

  return 0;
}
