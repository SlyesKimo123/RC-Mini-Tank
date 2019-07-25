// Connecting PS2 Controller to the Motor Shield
// Luke Chase
// July 2019

#include <PS2X_lib.h>
PS2X ps2x;  // PS2 Controller Class
//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;

//Arduino PWM Speed Control for DFRobot Motor Shield (default pins)
int E1 = 4; // IN2
int M1 = 5; // IN1
int S1 = A2; // ENA
int E2 = 3; // IN3
int M2 = 2; //IN4
int S2 = A1; //ENB
int lmotor = 0;
int rmotor = 0;

void setup()
{
  Serial.begin(57600);

  // set pin modes for DFRobot Motor Shield
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  error = ps2x.config_gamepad(8, 12, 9, 13, false, false); //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if (error == 0)
  {
    Serial.println("Found Controller, configured successfully!");
  }
  else if (error == 1)
  {
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
  }
  else if (error == 2)
  {
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
  }
  else if (error == 3)
  {
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  }

  type = ps2x.readType();
  if (type != 1)
  {
    Serial.println("warning: DualShock Controller Not Found!");
  }

}

void loop()
{
  if (error == 1) //skip loop if no controller found
    return;
  if (type == 1)
  {
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

    lmotor = 0;
    if (ps2x.Button(PSB_L1))
    {
      lmotor = 155;
    }
    if (ps2x.Button(PSB_L2))
    {
      lmotor = -155;
    }

    rmotor = 0;
    if (ps2x.Button(PSB_R1))
    {
      rmotor = 155;
    }
    if (ps2x.Button(PSB_R2))
    {
      rmotor = -155;
    }

    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
      if (ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if (ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if (ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if (ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if (ps2x.Button(PSB_TRIANGLE))
        Serial.println("Triangle pressed");
    }

    // update motors
    Serial.print("lmotor: ");
    Serial.println(lmotor);
    Serial.print("rmotor: ");
    Serial.println(rmotor);

    if (lmotor > 0)
    {
      digitalWrite(M1, LOW);
      digitalWrite(E1, HIGH);
      analogWrite(S1, abs(lmotor));
      //analogWrite(E1, -lmotor);   //PWM Speed Control
    }

    else if (lmotor < 0) {
      digitalWrite(M1, HIGH);
      digitalWrite(E1, LOW);
      analogWrite(S1, abs(lmotor));
      //analogWrite(E1, lmotor);   //PWM Speed Control

    }
    else
    {
      digitalWrite(M1, LOW);
      digitalWrite(E1, LOW);
      analogWrite(S1, 0);
    }

    if (rmotor > 0)
    {
      digitalWrite(M2, LOW);
      digitalWrite(E2, HIGH);
      analogWrite(S2, abs(rmotor));
      //analogWrite(E2, -rmotor);   //PWM Speed Control
    }
    else if (rmotor < 0) {
      digitalWrite(M2, HIGH);
      digitalWrite(E2, LOW);
      analogWrite(S2, abs(rmotor));
      // analogWrite(E2, rmotor);   //PWM Speed Control
    }
    else
    {
      digitalWrite(M2, LOW);
      digitalWrite(E2, LOW);
      analogWrite(S2, 0);
    }

    delay(30);
  }

  else
  {
    lmotor = 0;
    rmotor = 0;
  }

  


}
