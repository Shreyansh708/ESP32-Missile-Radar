#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------- OLED ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------------- Pins ----------------
#define TRIG_PIN     5
#define ECHO_PIN     18

#define RED_LED      25
#define GREEN_LED    26
#define YELLOW_LED   27

#define BUZZER       19

// ---------------- Setup ----------------
void setup() {

  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(8,20);
  display.println("RADAR");

  display.display();

  delay(2000);
}

// ---------------- Distance Function ----------------
float getDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if(duration == 0)
    return -1;

  float distance = duration * 0.0343 / 2;

  return distance;
}

// ---------------- Main Loop ----------------
void loop()
{
  float distance = getDistance();

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("MISSILE RADAR");

  display.setCursor(0,18);

  if(distance < 0)
  {
    display.println("No Target");

    digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(YELLOW_LED,LOW);

    noTone(BUZZER);
  }

  else
  {
    display.print("Distance: ");
    display.print(distance,1);
    display.println(" cm");

    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" cm");

    // ---------- SAFE ----------
    if(distance > 50)
    {
      digitalWrite(GREEN_LED,HIGH);
      digitalWrite(YELLOW_LED,LOW);
      digitalWrite(RED_LED,LOW);

      noTone(BUZZER);

      display.setCursor(0,40);
      display.println("STATUS : SAFE");
    }

    // ---------- TRACKING ----------
    else if(distance > 20)
    {
      digitalWrite(GREEN_LED,LOW);
      digitalWrite(YELLOW_LED,HIGH);
      digitalWrite(RED_LED,LOW);

      tone(BUZZER,1000,150);

      display.setCursor(0,40);
      display.println("STATUS : TRACKING");
    }

    // ---------- LOCKED ----------
    else
    {
      digitalWrite(GREEN_LED,LOW);
      digitalWrite(YELLOW_LED,LOW);
      digitalWrite(RED_LED,HIGH);

      tone(BUZZER,2000);

      display.setCursor(0,40);
      display.println("TARGET LOCKED");
    }
  }

  display.display();

  if(distance >20 && distance<=50)
    delay(400);
  else
    delay(100);
}

