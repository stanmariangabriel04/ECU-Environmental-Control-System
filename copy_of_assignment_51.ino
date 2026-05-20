#include <LiquidCrystal_I2C.h>

// butoane
#define BTN_UP     4
#define BTN_DOWN   3
#define BTN_SELECT 2

// iesiri
#define PIN_LIGHTS 6
#define PIN_WATER  7
#define PIN_BUZZER 11

// senzori
#define PIN_LDR    A0
#define PIN_SOIL   A1
#define PIN_TRIG   9
#define PIN_ECHO   10

// LCD
LiquidCrystal_I2C lcd(0x20, 16, 2);

// meniuri
enum MenuItem {
  MENU_TEMP = 0,
  MENU_WATER,
  MENU_LIGHTS,
  MENU_ALARM,
  MENU_COUNT
};

enum Mode {
  MODE_BROWSE = 0,
  MODE_EDIT
};

int  setTemp  = 22;
bool lightsOn = false;
bool waterOn  = false;
bool alarmOn  = false;

int  currentItem = MENU_TEMP;
Mode mode = MODE_BROWSE;

// debounce
unsigned long lastButtonTime = 0;
const unsigned long debounceMs = 150;
bool prevUp = HIGH, prevDown = HIGH, prevSelect = HIGH;

// BUTOANE 
int readButtons() {
  if (millis() - lastButtonTime < debounceMs) return 0;

  bool up = digitalRead(BTN_UP);
  bool dn = digitalRead(BTN_DOWN);
  bool se = digitalRead(BTN_SELECT);

  int evt = 0;
  if (prevUp && !up) evt = 1;
  else if (prevDown && !dn) evt = 2;
  else if (prevSelect && !se) evt = 3;

  prevUp = up;
  prevDown = dn;
  prevSelect = se;

  if (evt) lastButtonTime = millis();
  return evt;
}

//IESIRI
void updateOutputs() {
  int lumina = analogRead(PIN_LDR);
  int umiditate = analogRead(PIN_SOIL);

  // LDR: daca e luminos, stinge becul
  digitalWrite(PIN_LIGHTS, (lightsOn && lumina < 500) ? HIGH : LOW);

  // sol umed → opreste aspersor
  digitalWrite(PIN_WATER, (waterOn && umiditate < 600) ? HIGH : LOW);
}

// DISTANTA
long readDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  return pulseIn(PIN_ECHO, HIGH) * 0.034 / 2;
}

//LCD
void drawBrowseScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(currentItem == MENU_TEMP ? ">" : " ");
  lcd.print("Temp:");
  lcd.print(setTemp);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print(currentItem == MENU_WATER ? ">" : " ");
  lcd.print("W:");
  lcd.print(waterOn ? "On " : "Off");

  lcd.setCursor(8, 1);
  lcd.print(currentItem == MENU_LIGHTS ? ">" : " ");
  lcd.print("L:");
  lcd.print(lightsOn ? "On " : "Off");
}

void drawEditScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (currentItem == MENU_TEMP) {
    lcd.print("Set Temp");
    lcd.setCursor(0, 1);
    lcd.print(setTemp);
    lcd.print(" C");
  }
  else if (currentItem == MENU_WATER) {
    lcd.print("Sprinkler");
    lcd.setCursor(0, 1);
    lcd.print(waterOn ? "ON" : "OFF");
  }
  else if (currentItem == MENU_LIGHTS) {
    lcd.print("Lights");
    lcd.setCursor(0, 1);
    lcd.print(lightsOn ? "ON" : "OFF");
  }
  else if (currentItem == MENU_ALARM) {
    lcd.print("Alarm");
    lcd.setCursor(0, 1);
    lcd.print(alarmOn ? "ON" : "OFF");
  }
}

//SETUP
void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);

  pinMode(PIN_LIGHTS, OUTPUT);
  pinMode(PIN_WATER, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  drawBrowseScreen();
}

//LOOP
void loop() {
  updateOutputs();

  if (alarmOn) {
    long d = readDistance();
    digitalWrite(PIN_BUZZER, d < 20 ? HIGH : LOW);
  } else {
    digitalWrite(PIN_BUZZER, LOW);
  }

  int evt = readButtons();
  if (!evt) return;

  if (mode == MODE_BROWSE) {
    if (evt == 1) currentItem = (currentItem + MENU_COUNT - 1) % MENU_COUNT;
    if (evt == 2) currentItem = (currentItem + 1) % MENU_COUNT;
    if (evt == 3) mode = MODE_EDIT;
    drawBrowseScreen();
  } 
  else {
    if (evt == 3) {
      mode = MODE_BROWSE;
      drawBrowseScreen();
      return;
    }

    if (currentItem == MENU_TEMP && evt == 1) setTemp++;
    if (currentItem == MENU_TEMP && evt == 2) setTemp--;

    if (currentItem == MENU_WATER) waterOn = !waterOn;
    if (currentItem == MENU_LIGHTS) lightsOn = !lightsOn;
    if (currentItem == MENU_ALARM) alarmOn = !alarmOn;

    drawEditScreen();
  }
}
