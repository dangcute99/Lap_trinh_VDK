#include "mpu6050.h"


float xAsis, yAsis, xAsis1, yAsis1, zAsis, zAsis1;

void setup()
{
    // put your setup code here, to run once:
    mpu6050();
    Serial.begin(9600);

}

void loop()
{
    // put your main code here, to run repeatedly:

    

        xAsis1 = get_accx();
        yAsis1 = get_accy();
        zAsis1 = get_accz();

        xAsis = 90*xAsis1/16384;
        yAsis = 90*yAsis1/16384;
        zAsis = 90*zAsis1/16384;
       Serial.print("angleX : ");
    Serial.print(xAsis);
  Serial.print("\tangleY : ");
  Serial.print(yAsis);
  Serial.print("\tangleZ : ");
  Serial.println(zAsis);
  delay(2000);
        



 
  }
