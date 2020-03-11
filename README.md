# AndroidAltimeter
Practice altimeter using an Android phone for the display via an OTG cable.

 *   Emulation of an Aircraft Altimeter on an Android phone for personal practice and fun use.
 *   
 *   This utilizes either a Serial connection from the Arduino to the Android via an OTG cable
 *   or a Bluetooth connection, requiring the VectorDisplay app (Omega Centauri Software) from 
 *   the PlayStore, free of charge under the MIT open software license.
 *   
 *   Not all Android phones have a pressure sensor, and those that do may not have a Bosch BMP280
 *   pressure sensor that is accurate to +/- 1 meter.  Using the Bosch BMP280 pressure sensor
 *   with an Arduino makes it possible for any Android phone to have an accurate altimeter.
 *   
 *  Hardware ICSP with Pin #19 for the sensor CSB.
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
 *
