#include <Servo.h>

#define Sensor 8
#define RELAY_PIN 7
#define SERVO_PIN 9 // Ganti dengan pin yang sesuai dengan koneksi servo

Servo myServo; // Deklarasi objek Servo

int clap = 0;
long detection_range_start = 0;
long detection_range = 0;
boolean status_lights = false;
boolean servo_moved = false;
unsigned long servo_move_time = 0;

void setup() {
  pinMode(Sensor, INPUT);
  pinMode(13, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  myServo.attach(SERVO_PIN); // Melampirkan pin servo
}

void loop() {
  int status_sensor = digitalRead(Sensor);
  if (status_sensor == 0) {
    if (clap == 0) {
      detection_range_start = detection_range = millis();
      clap++;
    } else if (clap > 0 && millis() - detection_range >= 50) {
      detection_range = millis();
      clap++;
    }
  }
  if (millis() - detection_range_start >= 400) {
    if (clap == 2) {
      if (!status_lights) {
        status_lights = true;
        digitalWrite(13, HIGH);
        digitalWrite(RELAY_PIN, LOW);
        // Gerakkan servo ke posisi 90 derajat
        myServo.write(90);
        servo_moved = true;
        servo_move_time = millis();
      } else if (status_lights) {
        status_lights = false;
        digitalWrite(13, LOW);
        digitalWrite(RELAY_PIN, HIGH);
        // Gerakkan servo ke posisi awal (misalnya 0 derajat)
        myServo.write(0);
        servo_moved = false;
      }
    }
    clap = 0;
  }

  // Kembalikan servo ke posisi awal setelah 5 detik
  if (servo_moved && (millis() - servo_move_time >= 5000)) {
    myServo.write(0);
    servo_moved = false;
  }
}