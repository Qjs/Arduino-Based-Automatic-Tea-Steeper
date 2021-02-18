//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <AccelStepper.h>



// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

    // Motor pin definitions:
#define motorPin1  42     // IN1 on the ULN2003 driver
#define motorPin2  44      // IN2 on the ULN2003 driver
#define motorPin3  46     // IN3 on the ULN2003 driver
#define motorPin4  48     // IN4 on the ULN2003 driver
// Define the AccelStepper interface type; 4 wire motor in half step mode:
#define MotorInterfaceType 4

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper library with 28BYJ-48 stepper motor:
AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);

int t = 60;
int i = 0;
bool steep_status = false;
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;
 if (adc_key_in < 250)  return btnUP;
 if (adc_key_in < 450)  return btnDOWN;
 if (adc_key_in < 650)  return btnLEFT;
 if (adc_key_in < 850)  return btnSELECT;

 return btnNONE;  // when all others fail, return this...
}

void steep_func()
{
   stepper.setCurrentPosition(0);
   // Run the motor forward at 500 steps/second until the motor reaches 4096 steps (1 revolution):
   while (stepper.currentPosition() != -4096) 
   {
     stepper.setSpeed(-500);
     stepper.runSpeed();
   }
   for(i = t; i>=0; i--)
   {
    lcd.setCursor(9,1);

    lcd.print(i);
    lcd.print(" ");
    delay(1000);
   }
   stepper.setCurrentPosition(0);
   // Run the motor forward at 500 steps/second until the motor reaches 4096 steps (1 revolution):
   while (stepper.currentPosition() != 4096) 
   {
     stepper.setSpeed(500);
     stepper.runSpeed();
   }
   lcd.setCursor(0,0);
   lcd.print("Complete  ");
   delay(2000);

}


void setup()
{
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Steep Time"); // print a simple message
 stepper.setMaxSpeed(1000);

}

void loop()
{
 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 if(t < 60)
 {
  t=60;
 }
 lcd.print(t);      // display seconds elapsed since power-up
 lcd.print(" "); 

 lcd_key = read_LCD_buttons();  // read the buttons
 
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     delay(100); 
     t += 30;
     break;
     }
   case btnLEFT:
     {
     delay(100);
     t -= 30;
 
     break;
     }
   case btnUP:
     {
     
     stepper.setCurrentPosition(0);
     // Run the motor backwards at 1000 steps/second until the motor reaches -4096 steps (1 revolution):
     while (stepper.currentPosition() != 100) {
      stepper.setSpeed(500);
      stepper.runSpeed();
      }
     break;
     }
   case btnDOWN:
     {
     stepper.setCurrentPosition(0);
      // Run the motor forward at 500 steps/second until the motor reaches 4096 steps (1 revolution):
      while (stepper.currentPosition() != -100) {
        stepper.setSpeed(-500);
        stepper.runSpeed();
      }
     break;
     }
   case btnSELECT:
     {
     steep_func();
     break;
     }

 }

}
