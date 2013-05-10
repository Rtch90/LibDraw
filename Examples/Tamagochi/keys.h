#ifndef KEYS_H
#define KEYS_H

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#define TERMINAL "/dev/tty"
int input;
struct termios savetty;

void unsetcbreak(void) {
  tcsetattr(input, TCSADRAIN, &savetty);
  close(input);
}

void setcbreak(void) {
  input=open(TERMINAL,O_RDONLY);
  struct termios tty;
    tcgetattr(input, &savetty);
    tcgetattr(input, &tty);
    tty.c_lflag &= ~(ECHO|ECHONL|ICANON|IEXTEN);
    tty.c_cc[VTIME]=0;
    tty.c_cc[VMIN]=0;
    tcsetattr(input,TCSANOW,&tty);
}

int getch(void) {
  static char line[2];
  if (read(input,line,1)) {
    return line[0];
  }
  return -1;
}

#endif
