/**
 * Copyright (C) 2011 by Basil Shikin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "WProgram.h"

#include "JPEGCamera.h"
#include "SocketModem.h"
#include "SleepUtils.h"
#include "SoftwareSerial.h"
#include "EmailCamSettings.h"
#include "Base64.h"
#include "Fat16.h"



/**
 * Setup pin configruations
 */
void setupPins();


/**
 * Initialize on-board camera. This turns on camera power and
 * runs camera initialization.
 */
bool initializeCamera();

/**
 * Initialize SD Card. This turns on SD card power and
 * runs card initialization.
 */
bool initializeSDCard();

/**
 * Take picture and write picture file name into a buffer
 */
bool takePicture();

/**
 * Turn SD Card off
 */
void shutdownSDCard();

/**
 * Turn camera off
 */
void shutdownCamera();

/**
 * Initialize on-board modem. This turns on modem power and
 * runs modem initialization.
 */
bool initializeModem();

/**
 * Checks email for new mail and return true if a new image
 * has been requested.
 */
bool checkImageRequested();

/**
 * Email a picture to configured recipient address
 */
bool emailPicture();

/**
 * Turn modem off
 */
void shutdownModem();

/**
 * Hybernate chip for a sleep time
 */
void sleep( uint16_t duration );


void setup(); // Called once, sets up application variables
void loop();  // Called continuously, does main work


int main(void); // App entry point
