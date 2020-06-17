
// TinyGPS++ library is used to connect with the GPS module
#include <TinyGPS++.h> // Include TinyGPS++ library
#include <SoftwareSerial.h> // Include Software Serial library
#include <LiquidCrystal.h> // Include Liquid Crystal library for display

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

TinyGPSPlus gps;

#define RXPin 4 // Define software serial RX pin
#define TXPin 3 // Define software serial TX pin
 
SoftwareSerial gpsSerial(RXPin, TXPin); // Configure SoftSerial library

int count = 0; // Count variable used for timing
int vehicle_stopped = 0; // Variable used for indicating when vehicle is stopped
float vehicle_speed = 0; // Variable used to hold current speed in miles per hour

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  lcd.begin(16, 2);
}
 
void loop() {
  
  while (gpsSerial.available() > 0) { // Provides information when new sentence is available
    lcd.print("0-60 in: "); // Display text on LCD
 
    if (gps.encode(gpsSerial.read())) {
      if (gps.speed.isValid()) { // If speed is available from GPS
        vehicle_speed = gps.speed.mph(); // Store value of speed in miles per hour in variable vehicle_speed
        Serial.print("Speed: ");
        Serial.print(vehicle_speed); // Display speed
        Serial.println("mph");

        if (vehicle_speed == 0){ // If vehicle is stopped it has potential to go 0-60 mph
          vehicle_stopped = 1;
        }


        if (vehicle_stopped == 1 && vehicle_speed > 0){ // If vehicle was stopped then increased speed start timer

          count++; // Start counting 
          Serial.println(count);


          if (vehicle_speed == 60){ // Stop timer and display its value to LCD 
            lcd.setCursor(0, 1);
            lcd.print(count);
            lcd.print(" Seconds");
            delay(3000); // Allow time for user to view the time taken
            count = 0; // Reset timer to 0
          }
        }else {
          count = 0; // If car doesnt reach 60 mph and stops, reset timer
        }
        }
      }else {
        lcd.print("Speed Not Available"); // If speed cant be read from GPS, display this message
      }
      
  delay(1000); // Delay 1 second to count in seconds
  lcd.clear(); // Clear 0-60 time from screen
  }
  }
