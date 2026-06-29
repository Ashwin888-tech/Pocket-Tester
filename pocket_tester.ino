#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

/* ================= OLED (SPI) ================= */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_DC     9
#define OLED_CS     10
#define OLED_RESET  8

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &SPI,
  OLED_DC,
  OLED_RESET,
  OLED_CS
);

/* ================= BUTTONS ================= */
#define BTN_UP     2
#define BTN_DOWN   3
#define BTN_ENTER  4

/* ================= SENSOR PINS ================= */
#define DHT_PIN     5
#define TEST_PIN    6
#define TRIG_PIN    7
#define ECHO_PIN    1   // kept as-is

#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

/* ================= STATES ================= */
enum State {
  MAIN_MENU,
  PROTOCOL_MENU,
  PROTOCOL_RUN,
  COMPONENT_MENU,
  PIN_INFO,
  COMPONENT_RUN,
  LOGIC_SCREEN
};

State state = MAIN_MENU;
bool screenDrawn = false;
int menuIndex = 0;
int protocolIndex = 0;

/* ================= BUTTON HANDLER ================= */
bool pressed(uint8_t pin) {
  static uint32_t lastTime[10];
  static bool lastState[10];

  bool now = digitalRead(pin);
  if (lastState[pin] == HIGH && now == LOW) {
    if (millis() - lastTime[pin] > 250) {
      lastTime[pin] = millis();
      lastState[pin] = LOW;
      return true;
    }
  }
  if (now == HIGH) lastState[pin] = HIGH;
  return false;
}

/* ================= OLED HELPER ================= */
void oledClear() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);   // 🔴 THIS WAS MISSING
  display.setCursor(0, 0);
}

/* ================= SETUP ================= */
void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_ENTER, INPUT_PULLUP);

  pinMode(TEST_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();
  SPI.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC)) while (1);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);   // 🔴 CRITICAL
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println("Pocket");
  display.println("Tester");
  display.display();
  delay(1200);
}

/* ================= LOOP ================= */
void loop() {

  /* ===== MAIN MENU ===== */
  if (state == MAIN_MENU) {
    if (!screenDrawn) {
      oledClear();
      display.println("== MAIN MENU ==");
      display.print(menuIndex==0?"> ":"  "); display.println("Protocol");
      display.print(menuIndex==1?"> ":"  "); display.println("Component");
      display.print(menuIndex==2?"> ":"  "); display.println("Logic Probe");
      display.display();
      screenDrawn=true;
    }

    if (pressed(BTN_UP))   { menuIndex=(menuIndex+2)%3; screenDrawn=false; }
    if (pressed(BTN_DOWN)) { menuIndex=(menuIndex+1)%3; screenDrawn=false; }

    if (pressed(BTN_ENTER)) {
      screenDrawn=false;
      state = (menuIndex==0)?PROTOCOL_MENU :
              (menuIndex==1)?COMPONENT_MENU :
                              LOGIC_SCREEN;
    }
  }

  /* ===== PROTOCOL MENU ===== */
  else if (state == PROTOCOL_MENU) {
    if (!screenDrawn) {
      oledClear();
      display.println("Protocol Mode");
      display.print(protocolIndex==0?"> ":"  "); display.println("GPIO Test");
      display.print(protocolIndex==1?"> ":"  "); display.println("GPIO Activity");
      display.print(protocolIndex==2?"> ":"  "); display.println("I2C Scan");
      display.print(protocolIndex==3?"> ":"  "); display.println("< Back");
      display.display();
      screenDrawn=true;
    }

    if (pressed(BTN_UP))   { protocolIndex=(protocolIndex+3)%4; screenDrawn=false; }
    if (pressed(BTN_DOWN)) { protocolIndex=(protocolIndex+1)%4; screenDrawn=false; }

    if (pressed(BTN_ENTER)) {
      screenDrawn=false;
      state = (protocolIndex==3)?MAIN_MENU:PROTOCOL_RUN;
    }
  }

  /* ===== PROTOCOL RUN ===== */
  else if (state == PROTOCOL_RUN) {
    if (!screenDrawn) {
      oledClear();
      if (protocolIndex==0) {
        display.println("GPIO Test");
        display.print("D6: ");
        display.println(digitalRead(TEST_PIN)?"HIGH":"LOW");
      } else if (protocolIndex==1) {
        display.println("GPIO Activity");
        display.println("Watch signal");
      } else if (protocolIndex==2) {
        display.println("I2C Scan");
        display.println("Use SDA/SCL");
      }
      display.println("\nENTER = Back");
      display.display();
      screenDrawn=true;
    }
    if (pressed(BTN_ENTER)) { state=PROTOCOL_MENU; screenDrawn=false; }
  }

  /* ===== COMPONENT MENU ===== */
  else if (state == COMPONENT_MENU) {
    if (!screenDrawn) {
      oledClear();
      display.println("Component Mode");
      display.print(menuIndex==0?"> ":"  "); display.println("DHT11");
      display.print(menuIndex==1?"> ":"  "); display.println("IR Sensor");
      display.print(menuIndex==2?"> ":"  "); display.println("Ultrasonic");
      display.print(menuIndex==3?"> ":"  "); display.println("< Back");
      display.display();
      screenDrawn=true;
    }

    if (pressed(BTN_UP))   { menuIndex=(menuIndex+3)%4; screenDrawn=false; }
    if (pressed(BTN_DOWN)) { menuIndex=(menuIndex+1)%4; screenDrawn=false; }

    if (pressed(BTN_ENTER)) {
      screenDrawn=false;
      state = (menuIndex==3)?MAIN_MENU:PIN_INFO;
    }
  }

  /* ===== PIN INFO ===== */
  else if (state == PIN_INFO) {
    if (!screenDrawn) {
      oledClear();
      if(menuIndex==0) display.println("DHT11 -> D5");
      if(menuIndex==1) display.println("IR -> D6");
      if(menuIndex==2) display.println("TRIG D7 ECHO D1");
      display.println("\nENTER = Run");
      display.display();
      screenDrawn=true;
    }
    if (pressed(BTN_ENTER)) { state=COMPONENT_RUN; screenDrawn=false; }
  }

  /* ===== COMPONENT RUN ===== */
  else if (state == COMPONENT_RUN) {
    if (!screenDrawn) {
      oledClear();
      if(menuIndex==0){
        display.println("Reading DHT11");
        display.display();
        delay(2000);
        float t=dht.readTemperature();
        float h=dht.readHumidity();
        oledClear();
        if(isnan(t)||isnan(h)) display.println("DHT ERROR");
        else{
          display.print("Temp: "); display.print(t); display.println(" C");
          display.print("Hum : "); display.print(h); display.println(" %");
        }
      } else if(menuIndex==1){
        display.println("IR Sensor");
        display.print("State: ");
        display.println(digitalRead(TEST_PIN)?"HIGH":"LOW");
      } else if(menuIndex==2){
        digitalWrite(TRIG_PIN,LOW); delayMicroseconds(2);
        digitalWrite(TRIG_PIN,HIGH); delayMicroseconds(10);
        digitalWrite(TRIG_PIN,LOW);
        long d=pulseIn(ECHO_PIN,HIGH,30000);
        display.println("Ultrasonic");
        if(!d) display.println("No Echo");
        else{
          display.print("Dist: ");
          display.print(d*0.034/2);
          display.println(" cm");
        }
      }
      display.println("\nENTER = Back");
      display.display();
      screenDrawn=true;
    }
    if (pressed(BTN_ENTER)) { state=COMPONENT_MENU; screenDrawn=false; }
  }

  /* ===== LOGIC PROBE ===== */
  else if (state == LOGIC_SCREEN) {
    if (!screenDrawn) {
      oledClear();
      display.println("Logic Probe");
      display.print("D6: ");
      display.println(digitalRead(TEST_PIN)?"HIGH":"LOW");
      display.println("\nENTER = Back");
      display.display();
      screenDrawn=true;
    }
    if (pressed(BTN_ENTER)) { state=MAIN_MENU; screenDrawn=false; }
  }
}
