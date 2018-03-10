#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int s=50;

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
}
void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  printX_Axis(AcX);
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  printY_Axis(AcY);
//  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
//  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
//  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
//  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
//  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
//  Serial.println();
//  Serial.print("AcX = "); Serial.print(AcX);
//  Serial.print(" | AcY = "); Serial.print(AcY);
//  Serial.print(" | AcZ = "); Serial.print(AcZ);
//  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
//  Serial.print(" | GyX = "); Serial.print(GyX);
//  Serial.print(" | GyY = "); Serial.print(GyY);
//  Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(100);

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
}

void printX_Axis(int x)
{
  /*
   * Calibration is a little off. 
   * Here is the plot of the x-axis
   * 
   *    |------------|--------------|
   * -16000       -1500          19000
   * 
   */

  Serial.print("AcX = "); Serial.println(x);
  
  if(x > -16000 && x < -12500) {
    Serial.println("Face Left");
  }
  if(x > -12500 && x < -7000) {
    Serial.println("Face Left Angle");
  }
  else if (x > -7000 && x < 3000){
    Serial.println("Face Level to ground (Up or Down)");
  }
  else if (x > 3000 && x < 9000){
    Serial.println("Face Right Angle");
  }
    else if (x > 9000 && x < 19000){
    Serial.println("Face Right");
  }
}


void printY_Axis(int y)
{
  /*
	17000  -
		     |
		     |
		     |
		     -
  		   |
  		   |
  		   |
	-17000 -
  */
  Serial.print("AcY = "); Serial.println(y);
  
  if(y > -17000 && y < -10000) {
    Serial.println("Face Towards");
  }
  if(y > -10000 && y < -3000) {
    Serial.println("Face Slightly Towards");
  }
  else if (y > -3000 && y < 3000){
    Serial.println("Face Level to ground (Up or Down)");
  }
  else if (y > 3000 && y < 10000){
    Serial.println("Face Slightly Outwards");
  }
    else if (y > 10000 && y < 17000){
    Serial.println("Face Outwards");
  }
}

// Z-Axis is just the slope
// Change in y over change in x
// or just y / x
// or just use AcZ (probably best idea)
//  TODO...
//  void printZ_Axis(int y, int x)
//    or
//  void printZ_Axis(int z)



