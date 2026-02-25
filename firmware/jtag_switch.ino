/*
  JTAG Switch Controller - Minimal Version
  Board: Arduino Nano (ATmega328P)

  D2 -> SELECT_TMUX
  D4 -> EN_TMUX

  Default target: APP1

  Serial Commands:
    '1' -> Switch to APP1
    '2' -> Switch to APP2
*/

#include <Arduino.h>

// Pin definitions
#define PIN_SELECT 2   // D2 -> SELECT_TMUX
#define PIN_EN     4   // D4 -> EN_TMUX

// If EN_TMUX is connected to an active-low /EN pin,
// change this to false.
#define EN_ACTIVE_HIGH true

enum Target
{
  APP1 = 0,
  APP2 = 1
};

Target currentTarget = APP1;

// ------------------------------------------------------------
// Low-level control
// ------------------------------------------------------------
void setEnable(bool enable)
{
  if (EN_ACTIVE_HIGH)
  {
    digitalWrite(PIN_EN, enable ? HIGH : LOW);
  }
  else
  {
    digitalWrite(PIN_EN, enable ? LOW : HIGH);
  }
}

void setSelect(Target target)
{
  currentTarget = target;

  // Convention:
  // LOW  -> APP1
  // HIGH -> APP2
  digitalWrite(PIN_SELECT, (target == APP2) ? HIGH : LOW);
}

// Safe switching:
// 1. Disable mux
// 2. Change select
// 3. Small delay
// 4. Enable mux
void switchTarget(Target target)
{
  setEnable(false);
  delay(2);

  setSelect(target);
  delay(2);

  setEnable(true);
}

// ------------------------------------------------------------

void setup()
{
  pinMode(PIN_SELECT, OUTPUT);
  pinMode(PIN_EN, OUTPUT);

  // Default state: APP1
  setEnable(false);
  delay(5);

  setSelect(APP1);
  delay(5);

  setEnable(true);

  Serial.begin(115200);
}

void loop()
{
  if (Serial.available())
  {
    char cmd = Serial.read();

    if (cmd == '1')
    {
      switchTarget(APP1);
    }
    else if (cmd == '2')
    {
      switchTarget(APP2);
    }
  }
}
