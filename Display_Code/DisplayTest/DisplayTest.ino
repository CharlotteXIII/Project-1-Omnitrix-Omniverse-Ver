#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <math.h> // for sin(), cos()

// Define color constants
#define BLACK       0x0000
#define WHITE       0xFFFF
#define RED         0xF800
#define GREEN       0x07E0
#define BLUE        0x001F
#define CYAN        0x07FF
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define DARKGREEN   0x03E0
#define ORANGE      0xFD20
#define GRAY        0x8410
#define BROWN       0xA145

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


/////////////////////////////////////////////////////////////////////
unsigned long touchStartTime = 0;
bool showSelector = false;
int rotationAngle = 0;

void drawSelectorWheel(int offsetAngle = 0) {
  int cx = 120;
  int cy = 160;
  int r1 = 30;  // inner radius
  int r2 = 80;  // outer radius
  int segments = 6;

  tft.fillCircle(cx, cy, r2 + 2, BLACK); // clear previous

  for (int i = 0; i < segments; i++) {
    float angle1 = radians(i * (360 / segments) + offsetAngle);
    float angle2 = radians((i + 1) * (360 / segments) + offsetAngle);

    int x0 = cx + cos(angle1) * r1;
    int y0 = cy + sin(angle1) * r1;

    int x1 = cx + cos(angle2) * r1;
    int y1 = cy + sin(angle2) * r1;

    int x2 = cx + cos(angle2) * r2;
    int y2 = cy + sin(angle2) * r2;

    int x3 = cx + cos(angle1) * r2;
    int y3 = cy + sin(angle1) * r2;

    // Draw the wedge (segment)
    uint16_t color = (i % 2 == 0) ? RED : ORANGE;
    tft.fillTriangle(x0, y0, x1, y1, x2, y2, color);
    tft.fillTriangle(x2, y2, x3, y3, x0, y0, color);
  }

  // Draw center
  tft.fillCircle(cx, cy, r1, BLACK);
}

// ⏳ รอ 0.5 วิ แล้วหมุน
void updateSelectorMotion() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 100) { // หมุนทุก 100 ms
    rotationAngle += 10; // หมุนทีละ 10 องศา
    drawSelectorWheel(rotationAngle);
    lastUpdate = millis();
  }
}
/////////////////////////////////////////////////////////////////////

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
    touchStartTime = millis();  // เริ่มจับเวลา
    isTouching = true;
  }
  else if (touchedNow && isTouching) {
  if (!showSelector && millis() - touchStartTime > 500) {
    showSelector = true;  // แสดงหน้าปัด
    }
  }
  else if (!touchedNow && isTouching) {
    clearCircleAreaProperly();  // Restore UI only if previously touched
    isTouching = false;
    showSelector = false;
    rotationAngle = 0;
    drawOmnitrixUI();
  }

  // ถ้าอยู่ในโหมดหมุนหน้าปัด
  if (showSelector) {
    updateSelectorMotion();
  }
  delay(10); // reduce CPU usage, smooth update
}
