#include <mqtt-now-display.h>
#ifdef HAS_DISPLAY

#ifndef MAX_DISPL_LINES
#define MAX_DISPL_LINES 10
#endif

#ifndef MAX_DISPL_LINE_LENGTH
#define MAX_DISPL_LINE_LENGTH 25
#endif

#ifndef LINE_HEIGHT
#define LINE_HEIGHT 20
#endif

char lines[MAX_DISPL_LINES][MAX_DISPL_LINE_LENGTH+1];
uint8_t lastline = 0;

void setupDisplay() {
  LCD.setTextColor(WHITE);
  LCD.setTextSize(2);
};

void updateDisplay() {
  
};

void log2Display(const char* txt) {
  if (lastline == (MAX_DISPL_LINES - 1)) {
    for (uint8_t i = 1; i < MAX_DISPL_LINES; i++) {
      strcpy(lines[i-1], lines[i]);
    }
  } else {
    lastline ++;
  }
  if (strlen(txt) > (MAX_DISPL_LINE_LENGTH - 1)) {
    memcpy(lines[lastline], txt, MAX_DISPL_LINE_LENGTH);
  } else {
    strcpy(lines[lastline], txt);
  }
  lines[lastline][MAX_DISPL_LINE_LENGTH] = '\0';  

  LCD.clear();

  for (uint8_t i = 0; i <= lastline; i++) {
    int16_t y = LINE_HEIGHT * (i+1);
    LCD.setCursor(10,(LINE_HEIGHT * (i+1)));
    LCD.print(lines[i]);
  }
}

#endif