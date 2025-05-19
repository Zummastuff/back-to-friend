#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Lirik per bagian
const char* lyrics[] = {
  "I still remember",
  "I was scared to take a breath, didn't want you to move your head",
  "How can we go back to being friends",
  "When we just shared a bed? (Yeah)",
  "How can you look at me and pretend",
  "I'm someone you've never met?"
};

const int numLyrics = sizeof(lyrics) / sizeof(lyrics[0]);
int currentIndex = 0;
unsigned long lastUpdate = 0;
const unsigned long interval = 5000; // 5 detik per bagian

// Fungsi untuk memecah teks menjadi baris tanpa memotong kata
int wrapTextPreserveWords(const char* input, String lines[], int maxLineLength, int maxLines) {
  String text(input);
  int lineCount = 0;

  while (text.length() > 0 && lineCount < maxLines) {
    int len = maxLineLength;
    if (text.length() <= len) {
      lines[lineCount++] = text;
      break;
    }

    int lastSpace = text.lastIndexOf(' ', len);
    if (lastSpace == -1) lastSpace = len;

    lines[lineCount++] = text.substring(0, lastSpace);
    text = text.substring(lastSpace);
    text.trim();
  }

  return lineCount;
}

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED tidak terdeteksi"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);  // textSize 1: 6px wide, 8px tall per char
  display.setTextColor(SSD1306_WHITE);
  display.display();
  lastUpdate = millis();
}

void loop() {
  if (millis() - lastUpdate >= interval) {
    display.clearDisplay();

    // Bungkus teks ke beberapa baris
    String wrappedLines[4]; // maksimal 4 baris
    int lineCount = wrapTextPreserveWords(lyrics[currentIndex], wrappedLines, 21, 4); // 21 karakter/line

    int lineHeight = 8;
    int totalHeight = lineCount * lineHeight;
    int startY = (SCREEN_HEIGHT - totalHeight) / 2;

    for (int i = 0; i < lineCount; i++) {
      int textWidth = wrappedLines[i].length() * 6; // textSize(1): 6 px per char
      int x = (SCREEN_WIDTH - textWidth) / 2;
      int y = startY + i * lineHeight;
      display.setCursor(x, y);
      display.println(wrappedLines[i]);
    }

    display.display();

    currentIndex = (currentIndex + 1) % numLyrics;
    lastUpdate = millis();
  }
}
