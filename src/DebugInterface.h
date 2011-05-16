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

/*
 *
 * This class represents debug interface used in EmailCam project
 *
 *  Created on: Feb 12, 2011
 *      Author: Basil Shikin
 */

#ifndef DEBUGINTERFACE_H_
#define DEBUGINTERFACE_H_

#include "WProgram.h"
#include "SoftwareSerial.h"

#define DEBUG_BAUD_RATE 9600
#define SERIAL_DEBUG true

#define PIN_DEBUG true
#define DEBUG_PIN 6
#define FLASH_INTERVAL 500

class DebugInterface {
public:

	enum Component
	{
		CMP_GENERAL = 0x0,
		CMP_MODEM   = 0xB,
		CMP_CAMERA  = 0xC,
		CMP_SDCARD  = 0xD,
	};

	DebugInterface();

	/**
	 * Initialize debug interface
	 */
	void initialize( SoftwareSerial * stream );

	/**
	 * Blink debug LED given number of times
	 */
	void blink( uint8_t times );

	/**
	 * Write a debug message into program logs
	 *
	 * @param message Message to log
	 */
	void debug( const char * message );

	/**
	 * Write a debug message into program logs
	 *
	 * @param message Message to log
	 * @param number  Number that is relevant to the message
	 */
	void debug( const char * message, uint32_t number );

	/**
	 * Turn debug LED on
	 */
	void ledOn();

	/**
	 * Turn debug LED off
	 */
	void ledOff();

	/**
	 * Report component status
	 */
	void status( Component component, uint8_t statusCode );

	/**
	 * Report component progress
	 */
	void progress( Component component, uint32_t number );

	/**
	 * Report component error
	 */
	void error( Component component, uint8_t errorCode );

private:
	void printName( Component component );

	SoftwareSerial * m_serial;
};

#endif /* DEBUGINTERFACE_H_ */
