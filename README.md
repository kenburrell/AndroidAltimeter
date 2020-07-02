# AndroidAltimeter
Practice altimeter using an Android phone for the display via an OTG cable.

 *  Emulation of an Aircraft Altimeter on an Android phone for personal practice and fun use.
 *   
 *  This utilizes a Serial connection from the Arduino to the Android via an OTG cable
 *  connection, requiring the VectorDisplay app (Omega Centauri Software) from the PlayStore, 
 *  free of charge under the MIT open software license.
 *
 *  An optional Bluetooth connection can also be made.  See separate software, parts list, and
 *  Gerber files for the Bluetooth version.  The Bluetooth version utilizes a 3.3V/8MHz Arduino
 *  ProMini, a 9V battery holder (requires 9V battery), and 5V voltage regulator to power a
 *  HC-05 Bluetooth module at 57600 Baud (instructions on how to do this are in the Arduino
 *  INO file for the Bluetooth version), and supply raw voltage to the 3.3V ProMini.
 *   
 *  Not all Android phones have a pressure sensor, and those that do may not have a Bosch BME280
 *  pressure sensor that is accurate to +/- 1 meter.  Using the Bosch BME280 pressure sensor
 *  with an Arduino makes it possible for any Android phone to have an accurate altimeter.
 *   
 *  The USB version of this program is designed specifically for the Arduino Leonardo 5V/16MHz board.
 *
 *  Full circuit diagram, assembly and instructions are available in the accompanying PDF file.
 *  
 *  To use, touch inside the Kollsman circle, which will bring up a button menu. Then change the
 *  value displayed in the Kollsman circle to the most recent QNH in inches of Hg obtained from a 
 *  nearby airport METAR.
 *  
 *  QNH is increased or decreased by 0.01, 0.10, or 1.00 inch of Hg each time the respective  button
 *  is pushed on the Android.  QNH will be limited between 27.50 and 31.50 inches of Hg, inclusively.
 *  
 *  Only the value of QNH is updated when in the button menu.
 *
 *  When the SET button is pushed on the Android, the button menu will close and the display returned
 *  to full screen mode, and then all other data and altimeter hands will update.
 *  
 *  Made publicly available under the terms of the Creative Commons License.
 *  
 *  Copyright(c) March 10, 2020.  Ken Burrell.
 *
