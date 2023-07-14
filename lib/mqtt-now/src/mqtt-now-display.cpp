#include <mqtt-now-display.h>
#ifdef HAS_DISPLAY

#define MAX_DISPL_LINES 10
#define MAX_DISPL_LINE_LENGTH 30
#define LINE_HEIGHT 20

char lines[MAX_DISPL_LINES][MAX_DISPL_LINE_LENGTH];
uint8_t lastline = 0;

void setupDisplay() {
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
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
  strcpy(lines[lastline], txt);

  M5.Lcd.clear();

  for (uint8_t i = 0; i <= lastline; i++) {
    M5.Lcd.setCursor(10,(LINE_HEIGHT * (i+1)));
    M5.Lcd.println(lines[i]);
  }
}

#endif