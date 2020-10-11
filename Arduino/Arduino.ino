#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "PCF8574.h"
#include "Arduino.h"

PCF8574 expander(0x26);

bool pressed[6] = {true, true, true, true, true, true}; //true = unpressed false = pressed

int lastTime;

String line[14] = {
    "Siemaneczko :D",
    "Uruchamianie...",
    "Wcisnij przycisk",
    "1 - sterowanie",
    "2 - pomin",
    "Przycisk-funkcja",
    "6 - OK",
    "5 - n/a",
    "4 - n/a",
    "3 - n/a",
    "2 - DOL",
    "1 - GORA",
    "OK - POWROT",
    "Wybierz tryb:", //there put arrow down and up
};

#define BACKLIGHT_PIN 3

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);

byte cDown[] = { //character down arrow
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B10101,
    B01110,
    B00100};

byte cUp[] = { //character up arrow
    B00100,
    B01110,
    B10101,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100};

void setup()
{
  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.createChar(1, cUp);
  lcd.createChar(2, cDown);

  for (int i = 0; i < 6; i++)
  {
    expander.pinMode(i, INPUT_PULLUP);
  }

  expander.begin();

  lcd.home();
  displayLine(0, 0);
  displayLine(1, 1);

  int counting = 0;
  int lastDisplayed = 0;
  lastTime = millis() / 1000;
  int actualTime;
  while (pressed[0] && pressed[1])
  {
    actualTime = millis() / 1000;

    if (actualTime != lastTime)
    {
      if (counting == 3)
      {
        counting = 0;
        if (lastDisplayed == 1)
        {
          lcd.clear();
          displayLine(2, 0);
          displayLine(4, 1);
          lastDisplayed = 0;
        }
        else if (lastDisplayed == 0)
        {
          lcd.clear();
          displayLine(2, 0);
          displayLine(3, 1);
          lastDisplayed = 1;
        }
      }
      counting++;
      lastTime = millis() / 1000;
      for (int i = 0; i < 6; i++)
      {
        pressed[i] = expander.digitalRead(i);
      }
    }

    delay(500);
  };
  if (!pressed[0])
  {
    displaySterowanie();
  }
}

void displaySterowanie()
{
  lcd.clear();
  displayLine(5, 0);
  displayLine(6, 1);
  while (expander.digitalRead(5))
  {
    delay(250);
  }
  lcd.clear();
  displayLine(7, 0);
  displayLine(8, 1);
  delay(1000);
  while (expander.digitalRead(5))
  {
    delay(250);
  }
  lcd.clear();
  displayLine(9, 0);
  displayLine(10, 1);
  delay(1000);
  while (expander.digitalRead(5))
  {
    delay(250);
  }
  lcd.clear();
  displayLine(11, 0);
  displayLine(12, 1);
  delay(1000);
  while (expander.digitalRead(5))
  {
    delay(250);
  }
}

void displayLine(int no, int lineno)
{
  lcd.setCursor(0, lineno);
  lcd.print(line[no]);
}

void loop()
{
  lcd.clear();
  while (true)
  {

    displayLine(13, 0);
    lcd.setCursor(14, 0);
    lcd.write(1);
    lcd.write(2);
    delay(1000);
    displayLine(13, 0);
    lcd.setCursor(14, 0);
    lcd.write(2);
    lcd.write(1);
    delay(1000);
  }
}