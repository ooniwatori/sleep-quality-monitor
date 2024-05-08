#include <Wire.h>
#include<SoftwareSerial.h>
#define SENSOR 0x53 // Device add in which is also included the 8th bit for selectting the mode, read in this case
#define Power_Register 0x2D
#define X_Axis_Register_DATAX0 0x32 // Hexadecima address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x33 // Hexadecima address for the DATAX1 internal register.
#define Y_Axis_Register_DATAY0 0x34
#define Y_Axis_Register_DATAY1 0x35
#define Z_Axis_Register_DATAZ0 0x36
#define Z_Axis_Register_DATAZ1 0x37
#define ab(x) ((x) > 0? (x):-(x))

int  DataReturned_x0, DataReturned_x1, DataModified_x_out;
int  DataReturned_y0, DataReturned_y1, DataModified_y_out;
int  DataReturned_z0, DataReturned_z1, DataModified_z_out;
float x[2], y[2], z[2];
char incoming = 0;
SoftwareSerial myBlue(5, 4); //rx tx
int counter = 0;
char one = 'G';
char zero = 'B';
float def_x;
float def_y;
float def_z;
void setup() {
  Wire.begin(9600);
  Serial.begin(9600);
  myBlue.begin(9600);
  delay(100);
  Wire.beginTransmission(SENSOR);       // Begin transmission with the chosen chip
  // 2 - Which Register do you want to talk to ?
  Wire.write(Power_Register);           // Power-saving features control
  // Register 0x2D POWER_CTL is 8 bits: [unused:unused:Link:AUTO_SLEEP:Measure:Sleep:Wakeup1:Wakeup0]
  // 3 - What do you want to transmit? Enable measurement
  Wire.write(8);                        // bit D3 high for measuring enable: 8d - 00001000b

  Wire.endTransmission();               // end Transmission
  // initialize
  for (int i = 0; i < 2; ++i) {
    x[i] = 0;
    y[i] = 0;
  }
}

void loop()
{
  // sensor
  sensor();

  // Prints and send the data

  if ( def_x < 0.04 &&  def_y < 0.04 && def_z < 0.04) {
    Serial.println(one);
    myBlue.write(one);
  }
  else {
    Serial.println(zero);
    myBlue.write(zero);
  }
  /*for (int i = 0; i < 2; ++i) {
    Serial.print("x = ");
    Serial.println(x[i]);
    Serial.print("y = ");
    Serial.println(y[i]);
    }
    Serial.print("x = ");
    Serial.println(def_x);
    Serial.print("y = ");
    Serial.println(def_y);
    //Serial.print("z = ");
    //Serial.println(z);*/
  delay(500);
}

void sensor() {
  // x
  Wire.beginTransmission(SENSOR);       // begin transmission
  // 4 -What do you want to ask about the specific register ?
  Wire.write(X_Axis_Register_DATAX0);   // Requiring Register DATA_0
  Wire.write(X_Axis_Register_DATAX1);   // Requiring Register DATA_1

  Wire.endTransmission();               // end Transmission
  // 5 - Now wait for the data looping in a while ...
  Wire.requestFrom(SENSOR, 2);
  if (Wire.available() <= 2) {
    DataReturned_x0 = Wire.read();
    DataReturned_x1 = Wire.read();
    // 6 - Now how do you want the configuration of the three axes ?
    //  now read the data and do as the datasheet says.
    DataReturned_x1    = DataReturned_x1 << 8;
    DataModified_x_out = DataReturned_x0 + DataReturned_x1;
    x[counter] = (float)DataModified_x_out / 256.0;
    //-----------------------------------------------------------------------------------------------
  }

  // y
  Wire.beginTransmission(SENSOR);       // begin transmission
  // 4 -What do you want to ask about the specific register ?
  Wire.write(Y_Axis_Register_DATAY0);   // Requiring Register DATA_0
  Wire.write(Y_Axis_Register_DATAY1);   // Requiring Register DATA_1

  Wire.endTransmission();               // end Transmission
  // 5 - Now wait for the data looping in a while ...
  Wire.requestFrom(SENSOR, 2);
  if (Wire.available() <= 2) {
    DataReturned_y0 = Wire.read();
    DataReturned_y1 = Wire.read();
    // 6 - Now how do you want the configuration of the three axes ?
    //  now read the data and do as the datasheet says.
    DataReturned_y1    = DataReturned_y1 << 8;
    DataModified_y_out = DataReturned_y0 + DataReturned_y1;
    y[counter] = (float)DataModified_y_out / 256.0;
    //-----------------------------------------------------------------------------------------------
  }

  // z
  Wire.beginTransmission(SENSOR);       // begin transmission
  // 4 -What do you want to ask about the specific register ?
  Wire.write(Z_Axis_Register_DATAZ0);   // Requiring Register DATA_0
  Wire.write(Z_Axis_Register_DATAZ1);   // Requiring Register DATA_1

  Wire.endTransmission();               // end Transmission
  // 5 - Now wait for the data looping in a while ...
  Wire.requestFrom(SENSOR, 2);
  if (Wire.available() <= 2) {
    DataReturned_z0 = Wire.read();
    DataReturned_z1 = Wire.read();
    // 6 - Now how do you want the configuration of the three axes ?
    //  now read the data and do as the datasheet says.
    DataReturned_z1    = DataReturned_z1 << 8;
    DataModified_z_out = DataReturned_z0 + DataReturned_z1;
    z[counter] = (float)DataModified_z_out / 256.0;
    //-----------------------------------------------------------------------------------------------
  }
  def_x = ab(x[1] - x[0]);
  def_y = ab(y[1] - y[0]);
  def_z = ab(z[1] - z[0]);
  Serial.print("x = ");
    Serial.println(def_x);
    Serial.print("y = ");
    Serial.println(def_y);
    Serial.print("z = ");
    Serial.println(def_z);
    
  if (++counter >= 2) {
    counter = 0;
  }
}
