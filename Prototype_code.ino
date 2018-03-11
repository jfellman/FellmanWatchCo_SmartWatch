#include<Wire.h>
const int BUTTON = 2; // input pin where the button is connected
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int s=50;
int btnVal = 0;

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

    for (int x = 4; x < 13; x++) {
      pinMode(x, OUTPUT);
    }

    pinMode(BUTTON, INPUT);
}
void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
//  printX_Axis(AcX);
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
//  printY_Axis(AcY);
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
//  Serial.print("AcX = "); Serial.println(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.println(AcZ);
//  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
//  Serial.print(" | GyX = "); Serial.print(GyX);
//  Serial.print(" | GyY = "); Serial.print(GyY);
//  Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(100);

  btnVal = digitalRead(BUTTON);
  
  if (btnVal == HIGH) {
    digitalWrite(1, HIGH);
    for ( int x = 1; x < 13; x++) {
      digitalWrite(x, HIGH);
      delay(s);
    }
    digitalWrite(1, LOW);
    for ( int x = 1; x < 13; x++) {
      digitalWrite(x, LOW);
      delay(s);
    } 
  } else {
    for ( int x = 1; x < 13; x++) {
      digitalWrite(x, LOW);
      delay(s);
    }
  }
}

void printX_Axis(int x)
{
  /*
   * Calibration is a little off. 
   * Here is the plot of the x-axis
   * 
   *    o------------|--------------o
   * -16000        1300          17000
   * 
   */

   // Range variables for the x-axis
   int maxLeft = -16000;
   int middle = 1300;
   int maxRight = 17000;
   int offset = 3500;

  Serial.print("AcX = "); Serial.println(x);
  
  if(x > maxLeft && x < (maxLeft + offset)) {
    Serial.println("Face Left");
  }
  else if(x > (maxLeft + offset) && x < (middle - offset)) {
    Serial.println("Face Left Angle");
  }
  else if (x > (middle - offset) && x < (middle + offset)){
    Serial.println("Face Level to ground (Up or Down)");
  }
  else if (x > (middle + offset) && x < (maxRight - offset)){
    Serial.println("Face Right Angle");
  }
  else if (x > (maxRight - offset) && x < maxRight){
    Serial.println("Face Right");
  }
}


void printY_Axis(int y)
{
  /*
	17000  o
		     |
		     |
		     |
	-150	 -
  		   |
  		   |
  		   |
	-17000 o
  */

   // Range variables for the y-axis
   int maxUp = 17000;
   int middle = -150;
   int maxDown = -17000;
   int offset = 3500;
   
  Serial.print("AcY = "); Serial.println(y);
  
  if(y > maxDown && y < (maxDown + offset)) {
    Serial.println("Face Towards");
  }
  else if(y > (maxDown + offset) && y < (middle - offset)) {
    Serial.println("Face Slightly Towards");
  }
  else if (y > (middle - offset) && y < (middle + offset)){
    Serial.println("Face Level to ground (Up or Down)");
  }
  else if (y > (middle + offset) && y < (maxUp - offset)){
    Serial.println("Face Slightly Outwards");
  }
    else if (y > (maxUp - offset) && y < maxUp){
    Serial.println("Face Outwards");
  }
}

// Z-Axis seems like it's the same as Y-Axis..
//  TODO...
//  void printZ_Axis(int z)



