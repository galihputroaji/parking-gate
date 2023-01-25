#include <Servo.h>
#include <LiquidCrystal.h>


#define btn 7

#define TRIG_PIN 3
#define ECHO_PIN 4

#define ECHO_PIN_OUT 9
#define TRIG_PIN_OUT 10

#define svin_pin 2
#define svout_pin 6

#define PROX 8


// LCD PIN
const int rs = A5,
  en = A4,
  d4 = A0,
  d5 = A1,
  d6 = A2,
  d7 = A3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// LCD PROPS
char * LargeText = " = UNBAJA PARKING STATION ";
int iLineNumber = 1;
int iCursor = 0;

Servo svin;
Servo svout;

long duration, distance;
long duration_out, distance_out;

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  svout.attach(svout_pin);
  svout.write(90);
  lcd.begin(16, 2);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN_OUT, OUTPUT);
  pinMode(ECHO_PIN_OUT, INPUT);
  pinMode(PROX, INPUT);

  // Setup Tombol
  pinMode(btn, INPUT);
  // Attach Servo
  svin.attach(svin_pin);
  // Posisi Default Servo
  svin.write(90);
}

// Nilai Awal Slots = 8
int slots = 8;

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;
  // Serial.println("Jarak: " + String(distance));
  delay(100);

  lcd.setCursor(0, 0);

  // Jika slots lebih dari 0, maka Display Jumlah slots. Kika tidak, Slot Penuh
  if (slots > 0) {
    lcd.print("SLOT TERSEDIA: " + String(slots));
  } else {
    lcd.print("  SLOT PENUH      ");
  }

  // LCD Scroll (Baris Kedua)
  LCDScroll(300);

  // Status Tombol
  bool sbtn = digitalRead(btn);

  Serial.print("BTN: ");
  Serial.println(sbtn);

  // Status sudut servo saat ini
  int svin_status = svin.read();

  while (sbtn == 1 && slots > 0) {
    if (sbtn == 1 && svin_status != 0) {
      svin.write(0);
      delay(20);
      lcd.setCursor(0, 0);
      lcd.print("Selamat Datang,  ");
      lcd.setCursor(0, 1);
      lcd.print("Silahkan Masuk  ");
    }

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration / 2) / 29.1;
    // Serial.println("Jarak: " + String(distance));
    delay(100);

    if (distance > 0 && distance < 6) {
      break;
    }
  } // end while loop

  if (svin_status == 0 && distance > 0 && distance > 6) {
    delay(1000);
    svin.write(90);
    slots--;
  }
  //Gate Out


  // Servo svout;
  

  

  int status = digitalRead(PROX);
  Serial.println("Proximity: " + String(status));

  int svout_status = svout.read();

  //HERE WEGO

  digitalWrite(TRIG_PIN_OUT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_OUT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_OUT, LOW);
  duration_out = pulseIn(ECHO_PIN_OUT, HIGH);
  distance_out = (duration_out / 2) / 29.1;
  Serial.println("Jarak Keluar: " + String(distance_out));

  while (status == 0) {
    if (status == 0 && svout_status != 0) {
      svout.write(0);
      delay(20);
    }
  digitalWrite(TRIG_PIN_OUT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_OUT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_OUT, LOW);
  duration_out = pulseIn(ECHO_PIN_OUT, HIGH);
  distance_out = (duration_out / 2) / 29.1;
  Serial.println("Jarak Keluar: " + String(distance_out));

    if (distance_out > 0 && distance_out < 6) {
      break;
    }
  } // end while loop

   if (svout_status == 0 && distance_out > 0 && distance_out > 6) {
    delay(1000);
    svout.write(90);
    slots++;
  }

}


void updateLCDDisplay() {
  int iLenofLargeText = strlen(LargeText);
  if (iCursor == (iLenofLargeText - 1)) {
    iCursor = 0;
  }
  lcd.setCursor(0, iLineNumber);
  if (iCursor < iLenofLargeText - 16) {
    for (int iChar = iCursor; iChar < iCursor + 16; iChar++) {
      lcd.print(LargeText[iChar]);
    }
  } else {
    for (int iChar = iCursor; iChar < (iLenofLargeText - 1); iChar++) {
      lcd.print(LargeText[iChar]);
    }
    for (int iChar = 0; iChar <= 16 - (iLenofLargeText - iCursor); iChar++) {
      lcd.print(LargeText[iChar]);
    }
  }
  iCursor++;
}

void LCDScroll(long milidetik) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= milidetik) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    updateLCDDisplay();
  }
}
