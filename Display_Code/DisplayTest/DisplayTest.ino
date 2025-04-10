#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

// Define color constants
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

MCUFRIEND_kbv tft;
uint16_t ID;

// Touchscreen pin definitions for 2.4" TFT shield
#define YP A3  // must be analog
#define XM A2  // must be analog
#define YM 9   // digital
#define XP 8   // digital

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Touch pressure threshold
#define MINPRESSURE 5
#define MAXPRESSURE 1000

// Touch calibration (adjust if touch position is off)
#define TS_MINX 150
#define TS_MAXX 920
#define TS_MINY 120
#define TS_MAXY 900

// Screen dimensions
#define LCD_WIDTH  240
#define LCD_HEIGHT 320

// Flag for touch state
bool isTouching = false;

// Function to draw Omnitrix-style UI
void drawOmnitrixUI() {
  tft.fillScreen(BLACK);

  // Top green triangle
  tft.fillTriangle(0, 40, 120, 140, 240, 40, RED);
  tft.fillRect(0, 0, 240, 40, RED);
  tft.fillTriangle(20, 0, 120, 120, 220, 0, BLACK); // carve black notch in top

  // Bottom green triangle
  tft.fillTriangle(0, 280, 120, 180, 240, 280, RED);
  tft.fillRect(0, 280, 240, 40, RED);
  tft.fillTriangle(20, 320, 120, 200, 220, 320, BLACK); // carve black notch in bottom
}

// Function to draw green circle when touching
void drawGreenCircle() {
  tft.fillCircle(120, 160, 50, RED);
}

// Function to restore center area of UI to its original state
void clearCircleAreaProperly() {
  // Repaint only the part under the circle to match original look (no flicker)
  tft.fillCircle(120, 160, 51, BLACK);
  
  // Bottom part of top triangle (fills over green circle)
  tft.fillTriangle(59, 90, 120, 140, 180, 90, RED);

  // Black notch of top triangle
  tft.fillTriangle(20, 0, 120, 120, 220, 0, BLACK); 
  // (80, 60, 120, 120, 160, 60, BLACK);
  // Top part of bottom triangle
  tft.fillTriangle(59, 230, 120, 180, 180, 230, RED);

  // Black notch of bottom triangle
  tft.fillTriangle(20, 320, 120, 200, 220, 320, BLACK);
}

void setup() {
  Serial.begin(9600);
  ID = tft.readID();      // Detect LCD driver
  tft.begin(ID);          // Initialize TFT screen
  drawOmnitrixUI();       // Draw initial UI
}

void loop() {
  TSPoint p = ts.getPoint();         // Read touch input
  pinMode(XM, OUTPUT);               // Restore pin modes
  pinMode(YP, OUTPUT);

  // Convert raw touch to screen coordinates
  bool touchedNow = (p.z > MINPRESSURE && p.z < MAXPRESSURE);

  if (touchedNow && !isTouching) {
    drawGreenCircle();  // Draw green circle only once when touched
    isTouching = true;
  }
  else if (!touchedNow && isTouching) {
    clearCircleAreaProperly();  // Restore UI only if previously touched
    isTouching = false;
  }

  delay(10); // reduce CPU usage, smooth update
}
