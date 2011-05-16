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
 * This class contains implementation of a debugging interface
 *
 *  Created on: Feb 12, 2011
 *      Author: Basil Shikin
 */

#include "DebugInterface.h"

DebugInterface::DebugInterface() {}

/**
 * Initialize debug interface
 */
void DebugInterface::initialize( SoftwareSerial * serial)
{
	m_serial = serial;

	// Flash debug pin
	if ( PIN_DEBUG )
	{
		pinMode(DEBUG_PIN, OUTPUT);

		blink( 4 );
	}

	if ( SERIAL_DEBUG )
	{
		m_serial->println("");
		m_serial->println("---");
		m_serial->println("");
	}
}

void DebugInterface::blink( uint8_t times )
{
	for (uint8_t i = 0; i < times; i++)
	{
		digitalWrite(DEBUG_PIN, HIGH);
		delay(FLASH_INTERVAL);
		digitalWrite(DEBUG_PIN, LOW);
		delay(FLASH_INTERVAL);
	}
}

void DebugInterface::debug( const char * message )
{
	if ( SERIAL_DEBUG )
	{
		m_serial->println( message );
	}
}

void DebugInterface::debug( const char * message, uint32_t number )
{
	if ( SERIAL_DEBUG )
	{
		m_serial->print( message );
		m_serial->println( number, DEC);
	}
}

void DebugInterface::ledOn()
{
	if ( PIN_DEBUG )
	{
		digitalWrite( DEBUG_PIN, HIGH );
	}
}

void DebugInterface::ledOff()
{
	if ( PIN_DEBUG )
	{
		digitalWrite( DEBUG_PIN, LOW );
	}
}


void DebugInterface::status( Component component, uint8_t statusCode )
{
	if ( SERIAL_DEBUG )
	{
		m_serial->print( "I [");
		printName( component );
		m_serial->print("] Code: 0x");
		m_serial->println( statusCode, HEX );
	}
}

void DebugInterface::progress( Component component, uint32_t number )
{
	if ( SERIAL_DEBUG )
	{
		m_serial->print( "P [");
		printName( component );
		m_serial->print("]: ");
		m_serial->println( number, DEC);
	}
}

void DebugInterface::error( Component component, uint8_t errorCode )
{
	if ( SERIAL_DEBUG )
	{
		m_serial->print( "E [");
		printName( component );
		m_serial->print("] Code: 0x");
		m_serial->println( errorCode, HEX );
	}
}

void DebugInterface::printName( Component component )
{
	if ( SERIAL_DEBUG )
	{
		const char * name;
		if ( component == DebugInterface::CMP_CAMERA )
		{
			name = "Camera";
		}
		else if ( component == DebugInterface::CMP_MODEM )
		{
			name = "Modem";
		}
		else if ( component == DebugInterface::CMP_SDCARD)
		{
			name = "SDCard";
		}
		else
		{
			name = "General";
		}

		m_serial->print(name);
	}
}
