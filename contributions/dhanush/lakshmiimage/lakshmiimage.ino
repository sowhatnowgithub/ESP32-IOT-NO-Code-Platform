#include <Ucglib.h>
#include "image_data.h"  // Include the converted image data

#define CS    5
#define DC    21
#define RESET 22

Ucglib_ST7735_18x128x160_HWSPI ucg(DC, CS, RESET);

void drawImage() {
  int index = 0;
  for (int y = 0; y < 160; y++) {
    for (int x = 0; x < 128; x++) {
      uint16_t color = image_data[index++]; // Get pixel color from array
      ucg.setColor(color >> 8, (color >> 3) & 0xFC, (color << 3) & 0xF8);
      ucg.drawPixel(x, y);
    }
  }
}

void setup() {
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.clearScreen();
  ucg.setRotate90();  // Rotate to fit display
  drawImage();        // Display the image
}

void loop() {
  // Nothing needed in loop for static image
}
