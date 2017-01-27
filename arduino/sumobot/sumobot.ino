
#include <Servo.h>
#include "IManager.h"

IManager iManager;

Servo servoLeft; 
Servo servoRight;

void setup() 
{
  servoLeft.attach(D2);
  servoRight.attach(D8);
  iManager.setup();
  iManager.setCallback(movServo, "/move");
  servoRight.write(90);
  servoLeft.write(90);
}

void loop()

{
  iManager.loop();    
}


String movServo(String args)
{

  String mLeft     = args.substring(args.indexOf(":")+1, args.indexOf(";"));
  String mRight    = args.substring(args.lastIndexOf(":")+1, args.lastIndexOf(";"));
  
  servoRight.write(abs(mRight.toInt()-180));//se invierte direccion
  servoLeft.write(mLeft.toInt());
  delay(1);
  return "{\"success\":true,\"mLeft\":"+mLeft+", \"mRight\":"+mRight+"}";

}


