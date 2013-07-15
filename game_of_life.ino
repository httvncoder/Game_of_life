/*================================================================
 *
 *  Conway's Gqme of Life on Sparkfun Color LCD shield (Philips)
 *  Revision: 1.0 2013-07-15
 *
 *  Adapted from the Life example
 *  on the Processing.org site
 *
 *  by: Ulf Hellström, oraminute@gmail.com 
 *  date: 2013-07-15 
 *
 *  This Arduino scetch is Conway's Game of life running 
 *  on Sparkfun Color LCD Shield.
 *  
 *  Hit S1 to increase to the next generation
 *  Hit S3 to change the color of the next generations cells
 *  Hit S2 to run in automode (No longer possible to change colors.
 *
 *  license: CC-BY SA 3.0 - Creative commons share-alike 3.0
 *  use this code however you'd like, just keep this license and
 *  attribute. Let me know if you make hugely, awesome, great changes.
 *
 *===============================================================*/

#include <ColorLCDShield.h>

// Variables etc.
//****************************************************************
LCDShield lcd;

byte cont = -64;        // contrast value
int color = 1;          // Color value intial
byte SIZE = 21;         // Size of gameboard +1
int world[20][20][2];   // Array holding current and next gen cells
int density = 50;       // 50% cells alive on game start
int myArray[] = {NAVY, BLUE, TEAL, EMERALD, GREEN
                 , CYAN, SLATE, INDIGO, TURQUOISE
                 , OLIVE, MAROON, PURPLE, GRAY
                 , SKYBLUE, BROWN, CRIMSON, ORCHID
                 , RED, MAGENTA, ORANGE, PINK, CORAL
                 , SALMON, ORANGE, GOLD, YELLOW, WHITE}; // avilible colors of cells
int buttons[3] = {3, 4, 5};  // S1 = 3, S2 = 4, S3 = 5 buttons on the LCD shield
int autorun = 1;        // Should game runt in automode ? (No when starting up)
//****************************************************************


/**************************************************************
 * Setup arrays for the game by random putting cells in
 * current generation and empty the next generation
 *************************************************************/
void initgame() {
  for (int i = 0; i < SIZE; i++ ) {
    for (int j = 0; j < SIZE; j++) {
      if (random(100) < density) {
        world[i][j][0] = 1;
      }
      else {
        world[i][j][0] = 0;
      }
      world[i][j][1] = 0;
    }
  }  
}

/**************************************************************
 * Calculate the number of neighbours a cell have.
 *************************************************************/
int neighbours(int x, int y) {
 return world[(x + 1) % SIZE][y][0] +
         world[x][(y + 1) % SIZE][0] +
         world[(x + SIZE - 1) % SIZE][y][0] +
         world[x][(y + SIZE - 1) % SIZE][0] +
         world[(x + 1) % SIZE][(y + 1) % SIZE][0] +
         world[(x + SIZE - 1) % SIZE][(y + 1) % SIZE][0] +
         world[(x + SIZE - 1) % SIZE][(y + SIZE - 1) % SIZE][0] +
         world[(x + 1) % SIZE][(y + SIZE - 1) % SIZE][0];
}

/**************************************************************
 * Draw 6x6 pixels filled rects as cells on the screen
 *************************************************************/
void drawcell(int x, int y, int color) {
  int x0 = 4 + (6*x);
  int y0 = 4 + (6*y); 
  int x1 = 4 + (6*x)+6;
  int y1 = 4 + (6*y)+6;
  lcd.setRect(x0,y0,x1,y1,1,myArray[color]);
}

/**************************************************************
 * Draw the current generation of cells on the LCD shield.
 *************************************************************/
void displaycurgen() {
  lcd.clear(BLACK);
  // Display current generation
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (world[i][j][0] == 1) {
        drawcell(i,j,color);
      }
    }
  }  
}

/**************************************************************
 * Calculate who lives or dies.
 *************************************************************/
void birthcycle() {
  // Birth and death cycle
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      // Default is for cell to stay the same
      world[x][y][1] = world[x][y][0];
      int count = neighbours(x, y);
      if (count == 3 && world[x][y][0] == 0) {
        // A new cell is born
        world[x][y][1] = 1;
      }
      if ((count < 2 || count > 3) && world[x][y][0] == 1) {
        // Cell dies
        world[x][y][1] = 0;
      }
    }
  }
 
  // Copy next generation into place
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      world[x][y][0] = world[x][y][1];
    }
  } 
}

/**************************************************************
 * Handle buttons S1,S2,S3 on the Sparkfun Color LCD Shield.
 *************************************************************/
void checkbuttons() {
  // Check if button is pressed
  if (autorun == 1) {
    while(digitalRead(buttons[0])&&digitalRead(buttons[1])&&digitalRead(buttons[2]))
      ;  // Wait, do nothing, until a button is pressed
    if (!digitalRead(buttons[2])) {
       color++;
       if ( color > 27) {
         color = 1;
       }
    } else if (!digitalRead(buttons[1])) {
      autorun = 0;
    }
  }  
}

/**************************************************************
 * The by Arduino required setup()
 *************************************************************/
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  for (int i=0; i<3; i++)
  {
    pinMode(buttons[i], INPUT);  // Set buttons as inputs
    digitalWrite(buttons[i], HIGH);  // Activate internal pull-up
  }  
  lcd.init(PHILIPS);
  lcd.contrast(cont);
  lcd.clear(BLACK); 
  initgame(); 
}

/**************************************************************
 * The by arduino required loop
 *************************************************************/
void loop() {  
  displaycurgen();
  birthcycle();
  checkbuttons();  
  delay(700);  
}
