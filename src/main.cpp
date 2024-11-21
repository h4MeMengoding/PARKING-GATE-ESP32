#include <Arduino.h>

#include <ESP32Servo.h>
#include <Wire.h>

// Servo object
Servo myservo;
#define servoPin 21

// Pin definitions
#define trig 18
#define echo 19
#define buzzer 22
#define button 5

float jarak = 0.0;
bool palangTerbuka = false; // Status palang
bool bendaTerdeteksi = false;

void bukaPalang();
void tutupPalang();
float bacaJarak();
void buzzerBip();

void setup() {
    myservo.attach(servoPin);        // Attach servo to pin
    pinMode(trig, OUTPUT);           // Ultrasonic trigger pin
    pinMode(echo, INPUT);            // Ultrasonic echo pin
    pinMode(buzzer, OUTPUT);         // Buzzer pin
    pinMode(button, INPUT_PULLUP);   // Button pin with pull-up resistor

    digitalWrite(buzzer, LOW);       // Buzzer off
    Serial.begin(115200);
    myservo.write(0);                // Servo in closed position
    Serial.println("Sistem Palang Pintu Otomatis Siap");
}

void loop() {
    if (digitalRead(button) == LOW && !palangTerbuka) {
        bukaPalang();               // Open the gate when button is pressed
        palangTerbuka = true;
    }

    if (palangTerbuka) {
        jarak = bacaJarak();
        Serial.print("Jarak: ");
        Serial.print(jarak);
        Serial.println(" cm");

        // Jika benda terdeteksi dalam jarak 1-4 cm
        if (jarak >=3 && jarak <= 5) {
            Serial.println("Benda terdeteksi di depan sensor");
            bendaTerdeteksi = true;
        }

        // Jika benda sebelumnya terdeteksi, lalu jarak menjadi lebih dari 10 cm
        if (bendaTerdeteksi && jarak > 10.0) {
            Serial.println("Benda telah melewati sensor, menunggu 3 detik...");
            delay(1000);            // Tunggu 3 detik sebelum menutup palang
            tutupPalang();
        }
    }
}

void bukaPalang() {
    Serial.println("Palang Dibuka");
    myservo.write(90);               // Open the gate
    buzzerBip();                     // Beep when gate opens
}

void tutupPalang() {
    Serial.println("Palang Ditutup");
    myservo.write(0);                // Close the gate
    buzzerBip();                     // Beep when gate closes
    palangTerbuka = false;
    bendaTerdeteksi = false;         // Reset detection status
}

float bacaJarak() {
    // Mengirimkan sinyal trigger
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    // Membaca sinyal echo
    long durasi = pulseIn(echo, HIGH);

    // Konversi durasi ke jarak dalam cm
    float jarak = durasi * 0.034 / 2.0; // Kecepatan suara = 0.034 cm/us

    return jarak;
}

void buzzerBip() {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
}
