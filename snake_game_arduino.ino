#include "LedControl.h"

const int xp = 0;
const int yp = 1;
int dir = 1;

struct Apple {
  byte x;
  byte y;
};

struct Player {
  byte x;
  byte y;
  byte oldx;
  byte oldy;
  byte inuse = 0;
};

Apple apple;
Player player;
Player worm[64];

LedControl lc = LedControl(12, 10, 11, 1);

void setup() {
  /* matrix wake up */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  startGame();
}

void startGame() {
  for (int i = 0; i < 63; i++) {
    worm[i].inuse = 0;
  }
  apple.x = 4;
  apple.y = 4;
  player.inuse = 1;
}

byte checkCollision(byte xcoord, byte ycoord) {
  for (int i = 0; i < 63; i++) {
    if (worm[i].inuse && worm[i].x == xcoord && worm[i].y == ycoord) {
      return 0;
    }
  }
  return 1;
}

void newApple() {
  byte rndx;
  byte rndy;
  while (1) {
    rndx = random(8);
    rndy = random(8);
    if (checkCollision(rndx, rndy)) break;
  }
  apple.x = rndx;
  apple.y = rndy;
}

void draw() {
  lc.clearDisplay(0);
  lc.setLed(0, player.x, player.y, true);
  lc.setLed(0, apple.x, apple.y, true);
  for (int i = 0; i < 63; i++) {
    if (worm[i].inuse == 1) {
      lc.setLed(0, worm[i].x, worm[i].y, true);
    }
  }
}

void moveTail() {
  if (player.x == apple.x && player.y == apple.y) {
    int c = 0;
    while(worm[c].inuse == 1) {
        c++;
    }
    newApple();
    worm[c].inuse = 1;
  }
  for (int i = 63; i > 0; i--) {
    if (worm[i].inuse == 1) {
      worm[i].x = worm[i-1].x;
      worm[i].y = worm[i-1].y;
    }
  }
  if (worm[0].inuse == 1) {
    worm[0].x = player.x;
    worm[0].y = player.y;
  }
}

void move() {
  delay(100);
  switch(dir) {
    case 1:
      if(player.x < 7) {
        moveTail();
        player.x++;
      }
      break;
    case 2:
      if(player.x > 0) {
        moveTail();
        player.x--;
      }
      break;
    case 3:
      if(player.y > 0) {
        moveTail();
        player.y--;
      }
      break;
    case 4:
      if(player.y < 7) {
        moveTail();
        player.y++;
      }
      break;
  }
  if (!checkCollision(player.x, player.y)) {
    startGame();
  }
}

void loop() {
  int ix = analogRead(xp);
  int iy = analogRead(yp);
  int greater;
  if (abs(ix - 512) > abs(iy - 512)) {
    greater = 0;
  } else {
    greater = 1;
  }
  if (greater == 0) {
    if(ix < 300 && !(dir == 2 && worm[0].inuse)) {
      dir = 1;
    } else if(ix > 700 && !(dir == 1 && worm[0].inuse)) {
      dir = 2;
    }
  }
  if (greater == 1) {
    if(iy < 300 && !(dir == 4 && worm[0].inuse)) {
      dir = 3;
    } else if(iy > 700 && !(dir == 3 && worm[0].inuse)) {
      dir = 4;
    } 
  }
  move();
  draw();
}
