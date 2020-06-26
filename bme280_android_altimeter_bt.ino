/*  
 *   Emulation of an Aircraft Altimeter on an Android phone for personal practice and fun use.
 *   
 *   This utilizes a Bluetooth connection, requiring the VectorDisplay app (Omega Centauri Software) 
 *   from the PlayStore, free of charge under the MIT open software license.
 *   
 *   Not all Android phones have a pressure sensor, and those that do may not have a Bosch BMP280
 *   or BME280 pressure sensor that is accurate to +/- 1 meter.  Using the Bosch pressure sensor
 *   with an Arduino makes it possible for any Android phone to have an accurate altimeter.
 *   
 *  SPI communication with BMP280 on an Arduino Pro Mini 3V/8MHz.
 *  I2C communication with BME280 on an Arduino Pro Mini 3V/8MHz.
 *  
 *  To use, touch inside the Kollsman circle, which will bring up a button menu. Then change the
 *  value displayed in the Kollsman circle to the most recent QNH in inches of Hg obtained from a 
 *  nearby airport METAR.
 *  
 *  QNH is increased or decreased by 0.01, 0.10, or 1.00 inch of Hg each time the respective  button
 *  is pushed on the Android.  QNH will be limited between 27.50 and 31.50 inches of Hg, inclusively.
 *  
 *  When the SET button is pushed on the Android, the button menu will close and the display returned
 *  to full screen mode.
 *  
 *  Made publicly available under the terms of the Creative Commons License.
 *  
 *  Copyright(c) March 10, 2020.  Ken Burrell.
 */
 
#define USE_BME280 1

#ifdef USE_BME280
  #include <Wire.h>
#else
  #include <Wire.h>
  //#include <SPI.h> 
#endif

#include <Adafruit_Sensor.h>

#ifdef USE_BME280
  #include <Adafruit_BME280.h>
#else
  #include <Adafruit_BMP280.h>
#endif

#include <VectorDisplay.h>
SerialDisplayClass tft(Serial);
VectorDisplayMessage msg;

#ifdef USE_BME280
 Adafruit_BME280 bmp ;; // I2C 
#else
  //#define BMP_SCK 13
  //#define BMP_MISO 12
  //#define BMP_MOSI 11 
  //#define BMP_CS 10
  //Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
  Adafruit_BMP280 bmp ;; // I2C 
#endif

/* The HC-05 Bluetooth module has a default Bluetooth baud rate of 9600, 
 * and a default direct connection speed of 38400.  The BT baud rate can
 * be changed to 57600 by using a direct serial connection to the HC-05 
 * at 38400, and then issuing AT commands to change the BT baud rate.
 * Full instructions are found here:
 * https://www.instructables.com/id/AT-command-mode-of-HC-05-Bluetooth-module
 * 
 */
#define BAUD_RATE 57600
//#define BAUD_RATE 9600

float B = 29.92 ;
int delta = 15;
int clockCenterX=120;
int clockCenterY=110+delta;

void setup() {
  Serial.begin(BAUD_RATE);
  tft.begin(BAUD_RATE,240,320); //115200, 240,320);
  tft.clear();
  if (!bmp.begin(0x76)) {  
    tft.text(50, 20,"Could not find a valid BMP/BME/280 sensor, check wiring!");
    while (1);
  }
}

void drawDisplay(float B)
{
    tft.fillScreen(TFT_BLACK);
// Set up fixed text
    tft.setTextColor(TFT_GREEN,TFT_BLACK);  tft.setTextSize(2);
    tft.text(48,5,"Practice Altimeter");
    tft.setTextSize(1);  
    tft.text(0, 270," Feet");
    tft.text(0, 280," dAlt");
    tft.text(0, 290," mBar");
    tft.text(0, 300," degC");
#ifdef USE_BME280
    tft.text(0, 310," Hum%");
#endif
    
// Draw Clockface 
  // first clear clock
  tft.fillCircle(clockCenterX, clockCenterY, 100, TFT_BLACK);
  
  for (int i=0; i<2; i++)
  {
    tft.drawCircle(clockCenterX, clockCenterY, 100-i, TFT_GREEN);
  }
  for (int i=0; i<3; i++)
  {
    tft.drawCircle(clockCenterX, clockCenterY, i, TFT_GREEN);
  }
  
  // Draw a small mark for 100 feet
  drawMarks();

  // Label each mark
  drawNumbs();

  // insert Kollsman knob
  tft.fillCircle(190, 225+delta, 35, TFT_RED);
  tft.fillCircle(190, 225+delta, 30, TFT_BLACK);
  tft.setTextColor(TFT_MAGENTA); tft.setTextSize(2);
  tft.text(168,215+delta,String(B));
  tft.setTextColor(TFT_GREEN); tft.setTextSize(1);
  tft.text(100, 220+delta,"x100 Feet");
}
void drawMarks()
{
  float x1, y1, x2, y2, h, phi;

  tft.setTextColor(TFT_GREEN); tft.setTextSize(1);
   
  for (int i=0; i<10; i++)
  {
    h   = i*36.0;
    phi = radians(h);

    x1= 99.0*sin(phi);
    y1=-99.0*cos(phi);
    x2= 89.0*sin(phi);
    y2=-89.0*cos(phi);
  
    tft.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY, TFT_GREEN);
   
  }
  for (int i=0; i<50; i++)
  {
    h   = i*7.20;
    phi = radians(h);

    x1= 99.0*sin(phi);
    y1=-99.0*cos(phi);
    x2= 94.0*sin(phi);
    y2=-94.0*cos(phi);
  
    tft.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY, TFT_GREEN);
  }
}

void drawNumbs()
{
  float x3, y3, h, phi;
  int j,k;
    
  tft.setTextColor(TFT_GREEN); tft.setTextSize(2);
  
  for (int i=0; i<10; i++)
  {
    h = (float) i*36 ;
    phi = radians(h);   
    j = 85.0*sin(phi)+clockCenterX - 5; // adjust to center of character cell at 5,6
    k =-85.0*cos(phi)+clockCenterY - 6;
    
    tft.text(j,k,String(i));
  }
}
void draw100(float elev)
{
  float x1, y1, x2, y2, x3, y3, x4, y4, h, phi, chi, omg;

  h = ((int) elev % 1000 ) * 0.36;

  phi = radians(h);
  chi = radians(h+8.0);
  omg = radians(h-8.0);
  
  x1= 85.0*sin(phi);
  y1=-85.0*cos(phi);
  x2= 10.0*sin(phi);
  y2=-10.0*cos(phi);
  x3= 35.0*sin(chi);
  y3=-35.0*cos(chi);
  x4= 35.0*sin(omg);
  y4=-35.0*cos(omg);
  
  tft.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY, TFT_WHITE);
  tft.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY, TFT_WHITE);
  tft.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY, TFT_WHITE);
  tft.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY, TFT_WHITE);

}

void draw1K(float elev)
{
  float x1, y1, x2, y2, x3, y3, x4, y4, h, phi, chi, omg;

  h = elev * 0.036;

  phi = radians(h);
  chi = radians(h+12.0);
  omg = radians(h-12.0);
  
  x1=  70.0*sin(phi);
  y1= -70.0*cos(phi);
  x2=  10.0*sin(phi);
  y2= -10.0*cos(phi);
  x3=  25.0*sin(chi);
  y3= -25.0*cos(chi);
  x4=  25.0*sin(omg);
  y4= -25.0*cos(omg);
  
  tft.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY, TFT_WHITE);
  tft.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY, TFT_WHITE);
  tft.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY, TFT_WHITE);
  tft.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY, TFT_WHITE);

}
void draw10K(float elev)
{
  float x1, y1, x2, y2, h, phi;
  float x3, y3, x4, y4, x5, y5, chi, omg;

  h = elev * 0.0036;

  phi = radians(h);
  chi = radians(h-6.0);
  omg = radians(h+6.0);

  x1= 94.0*sin(phi)+clockCenterX;
  y1=-94.0*cos(phi)+clockCenterY;
  x2=  5.0*sin(phi)+clockCenterX;
  y2= -5.0*cos(phi)+clockCenterY;
  
  x3= 96.0*sin(chi)+clockCenterX;
  y3=-96.0*cos(chi)+clockCenterY;
  x4= 96.0*sin(omg)+clockCenterX;
  y4=-96.0*cos(omg)+clockCenterY;
  x5= 84.0*sin(phi)+clockCenterX;
  y5=-84.0*cos(phi)+clockCenterY;
   
  
  tft.drawLine(x1, y1, x2, y2, TFT_WHITE);
  tft.fillTriangle(x3, y3, x5, y5, x4, y4, TFT_WHITE);
}

int pixel_x, pixel_y;
float T,P,H,W,QNH,PA,DA;
float H0 = 0.0, OldB;

void loop() {
    QNH = 33.8639 * B ;
    T = bmp.readTemperature();
    P = 0.01 * bmp.readPressure();
    H = round(3.28*bmp.readAltitude(QNH));
#ifdef USE_BME280
    W = bmp.readHumidity();
#endif
    PA = H + 27.0 * (1013.00 - QNH);
    DA = round((1.2376 * PA) + ( 118.8 * T ) - 1782.0) ;

    if ( B != OldB || H < (H0 - 1.0) || H > (H0 + 1.0) ) {
      if ( B != OldB ) {
        drawDisplay(B);
      } else {
     // first clear hands of clock and re-draw clock numbers, then hands
        tft.fillCircle(clockCenterX, clockCenterY, 88, TFT_BLACK);
        for (int i=0; i<3; i++)
        {
           tft.drawCircle(clockCenterX, clockCenterY, i, TFT_GREEN);
        }
        drawNumbs();
      }
      draw10K(H);
      draw1K(H);
      draw100(H);
      H0 = H;
    }
     
     tft.setTextColor(TFT_YELLOW,TFT_BLACK); tft.setTextSize(1);
     tft.text(40, 270,String(H));
     tft.text(40, 280,String(DA));
     tft.text(40, 290,String(P));
     tft.text(40, 300,String(T));
#ifdef USE_BME280
     tft.text(40, 310,String(W));
#endif
     OldB = B;
     
     bool down = tft.readMessage(&msg);
     pixel_x = tft.getTouchX();
     pixel_y = tft.getTouchY();
     bool box = ( ( pixel_x >= 160 && pixel_x <= 220) &&
                  ( pixel_y >= 210 && pixel_y <= 240) ) ;
     delay (100);
     // if ( down  && box ) {
     if ( down ) {
       tft.addButton('a', "+ 0.01");
       tft.addButton('b', "+ 0.10");
       tft.addButton('c', "+ 1.00");
       tft.addButton('x', "- 0.01");
       tft.addButton('y', "- 0.10");
       tft.addButton('z', "- 1.00");
       tft.addButton('E', "SET");
       delay(250);
       do {
          bool quit = false;
          if (tft.readMessage(&msg) && msg.what == MESSAGE_BUTTON) {
            if (msg.data.button == 'a') {
              B = B + 0.01;
            }
            else if (msg.data.button == 'b') {
              B = B + 0.10;
            }
            else if (msg.data.button == 'c') {
              B = B + 1.00;
            }
            else if (msg.data.button == 'x') {
              B = B - 0.01;
            }
            else if (msg.data.button == 'y') {
              B = B - 0.10;
            }
            else if (msg.data.button == 'z') {
              B = B - 1.00;
            }
            else if (msg.data.button == 'E') {
              quit = true;
            }
            if ( B > 31.50 ) B = 31.50 ;
            if ( B < 27.50 ) B = 27.50 ;
            delay(125);
            tft.fillCircle(190, 225+delta, 30, TFT_BLACK);
            tft.setTextColor(TFT_WHITE); tft.setTextSize(2);
            tft.text(168,215+delta,String(B));
            delay(125);
            if ( quit ) {
              tft.deleteButton('a');
              tft.deleteButton('b');
              tft.deleteButton('c');
              tft.deleteButton('x');
              tft.deleteButton('y');
              tft.deleteButton('z');
              tft.deleteButton('E');
              OldB = 0.0;
              break;
            }
          }
        // keep processing entries until SET hit
        } while ( 1 );
     }
     delay(500);
}
