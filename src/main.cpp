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

#include "main.h"

#include <stdlib.h>

extern "C" void __cxa_pure_virtual()
{
  cli();
  for (;;);
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));

void * operator new[](size_t size)
{
    return malloc(size);
}

void operator delete[](void * ptr)
{
	if (ptr)
		free(ptr);
}

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);}
void __cxa_guard_release (__guard *g) {*(char *)g = 1;}
void __cxa_guard_abort (__guard *) {}



void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
}

// Common defines
#define SEND_PICTURE_RETRIES 3
#define TEMP_BUFFER_SIZE 100
#define ENCODED_BUFFER_SIZE 100

// Common codes
#define READY 0xAA
#define INITIALIZING 0xA0
#define CANT_INITIALIZE 0xE1
#define SHUTTING_DOWN 0xAF


// Modem codes
#define CHECKING_EMAIL 0xB1
#define SENDING_PICTURE 0xB2
#define SEND_PROGRESS   0xB3
#define CANT_INITIALIZE_SEND_PICTURE 0xE1
#define CANT_SEND_PICTURE_CHUNK 0xE2
#define CANT_COMMIT_SEND_PICTURE 0xEA
#define CANT_START_CONNECTION 0xEB

// Camera codes
#define TAKING_PICTURE 0xC1
#define TAKING_PICTURE 0xC1
#define PICTURE_TAKEN 0xC2

#define CANT_TAKE_PICTURE 0xE2
#define NO_PICTURE_SIZE 0xE3
#define NO_FILE_SIZE 0xE4


// SD Card codes
#define LOADING_SETTINGS 0xC1
#define SAVING_FILE 0xD1
#define SAVE_PROGRESS   0xD3
#define FILE_SAVED 0xD2
#define FILE_SAVED 0xD2
#define CANT_OPEN_FILE 0xE3
#define CANT_OPEN_BASE_65_FILE 0xE4
#define CANT_INIT_FAT16 0xEA
#define CANT_INIT_SD 0xEB
#define CANT_OPEN_FILE_TO_READ 0xEC
#define CANT_OPEN_SETTINGS 0xED

// Pin mappings
#define PIN_MODEM_POWER 5
#define PIN_SDCARD_POWER 10
#define PIN_MODEM_CTS 3
#define PIN_CAMERA_POWER 4

#define PIN_DEBUG_TX 7
#define PIN_DEBUG_RX 9

#define PIN_PORT_SWITCH 2
#define STATE_MODEM_PORT  LOW
#define STATE_CAMERA_PORT HIGH
#define POWER_ON  LOW
#define POWER_OFF HIGH

// Common variables
static EmailCamSettings s_settings;

// Debug variables
static SoftwareSerial s_debugSerial = SoftwareSerial( PIN_DEBUG_RX, PIN_DEBUG_TX );
static DebugInterface s_debugInterface = DebugInterface();

// Socket modem interface
static SocketModem s_modem = SocketModem( &Serial, PIN_MODEM_CTS, &s_settings, &s_debugInterface );

// Camera interface
static JPEGCamera s_camera = JPEGCamera( &Serial );

// SD Card values
static SdCard s_card;

// State variables
char s_picutreFileName[]   = "IMAGE000.JPG";
char s_base64FileName[]    = "BASE64.TMP";
char s_settingsFileName[]  = "CAMERA.CFG";

uint8_t s_buffer[TEMP_BUFFER_SIZE];
uint8_t s_encodedBuffer[ENCODED_BUFFER_SIZE];

bool m_cameraReady = false;
bool m_modemReady  = false;
bool m_sdCardReady = false;

void setup()
{
	pinMode(PIN_DEBUG_RX, INPUT);
	pinMode(PIN_DEBUG_TX, OUTPUT);

	s_debugSerial.begin( 9600 );

	s_debugInterface.initialize( &s_debugSerial );

	su_initialize();

	setupPins();

	bool settingsLoaded = false;

	// Read settings from the SD card
	while ( !settingsLoaded )
	{
		if ( initializeSDCard() )
		{
			Fat16 settingsFile;
			if ( settingsFile.open( s_settingsFileName, O_READ ) )
			{
				s_debugInterface.status( DebugInterface::CMP_SDCARD, LOADING_SETTINGS );

				if ( s_settings.loadSettings( settingsFile ) )
				{
					settingsLoaded = true;
				}

				settingsFile.close();
			}
			else
			{
				s_debugInterface.error( DebugInterface::CMP_SDCARD, CANT_OPEN_SETTINGS );
			}
		}

		shutdownSDCard();

		if ( !settingsLoaded )
		{
			delay( 5000 );

			s_debugInterface.blink( 2 );
		}
	}
}

void setupPins()
{
	pinMode( PIN_MODEM_POWER, OUTPUT);
	digitalWrite( PIN_MODEM_POWER, POWER_OFF );

	pinMode( PIN_SDCARD_POWER, OUTPUT);
	digitalWrite( PIN_SDCARD_POWER, POWER_OFF );

	pinMode( PIN_CAMERA_POWER, OUTPUT);
	digitalWrite( PIN_CAMERA_POWER, POWER_OFF );

	pinMode( PIN_PORT_SWITCH, OUTPUT );
	digitalWrite( PIN_PORT_SWITCH, STATE_MODEM_PORT );
}



bool initializeModem()
{
	if ( m_modemReady ) return true;

	s_debugInterface.status( DebugInterface::CMP_MODEM, INITIALIZING );

	// Connect modem power and pin
	digitalWrite( PIN_MODEM_POWER, POWER_ON );
	digitalWrite( PIN_PORT_SWITCH, STATE_MODEM_PORT );

	Serial.begin( MODEM_BAUD_RATE );

	delay( 2000 ); // Some time for modem to turn on

	if ( !s_modem.initialize() )
	{
		s_debugInterface.error( DebugInterface::CMP_MODEM, CANT_INITIALIZE );

		return false;
	}

	m_modemReady = true;

	return true;
}

void shutdownModem()
{
	if ( !m_modemReady ) return;

	s_debugInterface.status( DebugInterface::CMP_MODEM, SHUTTING_DOWN );

	s_modem.shutdown();

	delay( 200 ); // Some time for modem to shut down

	digitalWrite( PIN_MODEM_POWER, POWER_OFF );

	m_modemReady = false;
}

bool initializeSDCard()
{
	if ( m_sdCardReady ) return true;

	s_debugInterface.status( DebugInterface::CMP_SDCARD, INITIALIZING );

	// Connect SD Card power
	digitalWrite( PIN_SDCARD_POWER, POWER_ON );

	delay( 1000 ); // Some time for SD card to turn on

	// Initialize the SD card
	if ( !s_card.init() )
	{
		s_debugInterface.error( DebugInterface::CMP_SDCARD, CANT_INIT_SD );

		return false;
	}

	delay( 200 );

	// Initialize a FAT16 volume
	if (!Fat16::init(&s_card))
	{
		s_debugInterface.error( DebugInterface::CMP_SDCARD, CANT_INIT_FAT16 );

		return false;
	}

	m_sdCardReady  = true;

	return true;
}

void shutdownSDCard()
{
	if ( !m_sdCardReady ) return;

	s_debugInterface.status( DebugInterface::CMP_SDCARD, SHUTTING_DOWN );

	digitalWrite( PIN_SDCARD_POWER, POWER_OFF );

	 m_sdCardReady = false;
}


bool initializeCamera()
{
	if ( m_cameraReady ) return true;

	s_debugInterface.status( DebugInterface::CMP_CAMERA, INITIALIZING );

	// Connect camera power and pin
	digitalWrite( PIN_CAMERA_POWER, POWER_ON );
	digitalWrite( PIN_PORT_SWITCH, STATE_CAMERA_PORT );

	Serial.begin( CAMERA_BAUD_RATE );

	delay( 2000 ); // Some time for camera to turn on

	if ( !s_camera.reset() )
	{
		digitalWrite( 13, HIGH);
		s_debugInterface.error(  DebugInterface::CMP_CAMERA, CANT_INITIALIZE );

		return false;
	}

	delay(2000);

	m_cameraReady = false;

	return true;
}


void shutdownCamera()
{
	if ( !m_cameraReady ) return;

	s_debugInterface.status( DebugInterface::CMP_CAMERA, SHUTTING_DOWN );

	delay( 200 ); // Some time for camera to shut down

	digitalWrite( PIN_CAMERA_POWER, POWER_OFF );

	m_cameraReady = false;
}


bool checkImageRequested()
{
	s_debugInterface.status( DebugInterface::CMP_MODEM, CHECKING_EMAIL );

	if ( !s_modem.startConnection() )
	{
		s_debugInterface.error( DebugInterface::CMP_MODEM, CANT_START_CONNECTION );

		return false;
	}

	bool hasNewMail = s_modem.hasNewMail();

	s_modem.stopConnection();

	return hasNewMail;
}


bool emailPicture()
{
	bool haveError = false;

	if ( !s_modem.startConnection() )
	{
		s_debugInterface.error( DebugInterface::CMP_MODEM, CANT_START_CONNECTION );

		return false;
	}

	delay( 1000 );

	s_debugInterface.status( DebugInterface::CMP_MODEM, SENDING_PICTURE );

	s_debugInterface.ledOn();

	// Now email base 64 file
	if (!s_modem.initalizeSendPicture() )
	{
		s_debugInterface.error(  DebugInterface::CMP_MODEM, CANT_INITIALIZE_SEND_PICTURE );

		haveError = true;
	}

	if ( !haveError )
	{
		// Read last picture from the SD card and save to to a temporary file in BASE64
		Fat16 base64File;
		if (!base64File.open( s_base64FileName, O_READ ) )
		{
			s_debugInterface.error( DebugInterface::CMP_SDCARD, CANT_OPEN_BASE_65_FILE );

			return false;
		}

		size_t readSize  = 77;
		memset( s_buffer, 0, sizeof( s_buffer ) );
		size_t read      = base64File.read( s_buffer, readSize );
		uint32_t totalRead = read;
		size_t readCounter = read;

		while ( read > 0 )
		{
			s_buffer[ read ] = '\0';

			if ( !s_modem.sendData( (char *)s_buffer ) )
			{
				s_debugInterface.error(  DebugInterface::CMP_MODEM, CANT_SEND_PICTURE_CHUNK );

				haveError = true;
				break;
			}

			memset( s_buffer, 0, sizeof( s_buffer ) );
			read = base64File.read( s_buffer, readSize );

			readCounter += read;
			totalRead += read;

			if ( readCounter > 1024 )
			{
				s_debugInterface.progress( DebugInterface::CMP_MODEM, totalRead/1024 );
				readCounter = 0;
			}
		}

		// Close base 64 file
		base64File.close();
	}

	if ( !haveError )
	{
		if ( !s_modem.commitSendPicture() )
		{
			s_debugInterface.error(  DebugInterface::CMP_MODEM, CANT_COMMIT_SEND_PICTURE );

			haveError = true;
		}
	}

	s_modem.stopConnection();

	s_debugInterface.ledOff();

	return !haveError;
}


bool takePicture()
{
	s_debugInterface.status( DebugInterface::CMP_CAMERA, TAKING_PICTURE );

	size_t      count   = 0; // Number of characters in the response string
	size_t      i       = 0; // For loops

	size_t size    = 0; // Size of the jpeg image
	uint32_t address = 0; // Data address being read from the camera

	size_t encoded = 0; // BASE64 encoded count
	uint32_t totalEncoded = 0; // Total BASE64 encoded
	uint32_t totalWritten = 0;

	char     eof     = 0; // 1 when JPEG end of file marker read

	if ( !s_camera.takePicture() )
	{
		s_debugInterface.error( DebugInterface::CMP_CAMERA, CANT_TAKE_PICTURE );

		return false;
	}
	s_debugInterface.status( DebugInterface::CMP_CAMERA, PICTURE_TAKEN );

	// Wait for a picture to be taken
	delay( 2000 );

	// Get picture size
	if ( !s_camera.getSize( &size ) )
	{
		s_debugInterface.error( DebugInterface::CMP_CAMERA, NO_PICTURE_SIZE );

		return false;
	}

	delay( 3000 ); // I don't know why we need a delay here,
				   // but it doesn't work without it

	if ( size > 0 )
	{
		Fat16 file;

		Base64 encoder;
		Fat16 base64File;

		// Find, create and open a new file
		for (int j = 0; j < 1000; j++)
		{
			s_picutreFileName[5] = j / 10 + '0';
			s_picutreFileName[6] = ( ( j % 100 ) / 10) + '0';
			s_picutreFileName[7] = j % 10 + '0';

			// O_CREAT - create the file if it does not exist
			// O_EXCL - fail if the file exists
			// O_WRITE - open for write
			if ( file.open(s_picutreFileName, O_CREAT | O_EXCL | O_WRITE) ) break;
		}

		if (!file.isOpen())
		{
			s_debugInterface.error( DebugInterface::CMP_SDCARD, CANT_OPEN_FILE );

			return false;
		}


		// Open BASE 64 file
		if (!base64File.open( s_base64FileName, O_WRITE | O_CREAT ) )
		{
			s_debugInterface.error( DebugInterface::CMP_SDCARD, CANT_OPEN_BASE_65_FILE );

			return false;
		}

		s_debugInterface.status( DebugInterface::CMP_SDCARD, SAVING_FILE );
		s_debugInterface.ledOn();

		while ( address < size &&
		        eof == 0 )
		{
			// Read the data starting at the current address.
			count = s_camera.readData( s_buffer, 32, address );

			//Store all of the data that we read to the SD card
			for(i=0; i < count; i++)
			{
				file.write( s_buffer[i] );

				//Check the response for the eof indicator (0xFF, 0xD9). If we find it, set the eof flag
				if ( i > 0 &&  (s_buffer[i] == (char)0xD9) && (s_buffer[i-1]==(char)0xFF) ) eof = 1;

				//If we found the eof character, get out of this loop and stop reading data
				if( eof == 1 ) break;
			}

			//Increment the current address by the number of bytes we read
			address += count;

			// Encode read data
			encoded = encoder.encodeChunk( (char * )s_encodedBuffer, (char * )s_buffer, count );

			// Write encoded chunk
			for ( i=0; i < encoded; i++ )
			{
				// Put a new line every 76 encoded characters
				if ( totalEncoded > 0 && totalEncoded % 75 == 0 ) base64File.write( "\n" );

				base64File.write( s_encodedBuffer[i] );

				totalEncoded += 1;
			}

			totalWritten += encoded;
			if ( totalWritten > 1024 )
			{
				s_debugInterface.progress( DebugInterface::CMP_SDCARD, address/1024 );
				totalWritten = 0;
			}

			delay( 1 ); // Don't work without this delay
		}

		// Finish encoding
		encoded = encoder.finishEncoding( (char * )s_encodedBuffer );
		for ( i=0; i < encoded; i++ )
		{
			// Put a new line every 76 encoded characters
			if ( totalEncoded > 0 && totalEncoded % 75  == 0 ) base64File.write( "\n" );

			base64File.write( s_encodedBuffer[i] );

			totalEncoded += 1;
		}

		// Close the file
		file.close();
		base64File.close();

		s_debugInterface.ledOff();

		s_camera.stopPictures();

		delay( 2000 );

		s_debugInterface.status( DebugInterface::CMP_SDCARD, FILE_SAVED );
	}
	else
	{
		s_debugInterface.error( DebugInterface::CMP_CAMERA, NO_FILE_SIZE );

		return false;
	}

	return true;
}


void  sleep( uint16_t duration )
{
	Serial.end();

	su_sleep( duration );

	s_debugSerial.begin( DEBUG_BAUD_RATE );
}

void loop()
{
	s_debugInterface.blink( 2 );

	s_debugInterface.status( DebugInterface::CMP_GENERAL, READY );

	do
	{
		if ( !initializeModem() ) break;

		bool isNewImageRequested = checkImageRequested();

		if ( isNewImageRequested )
		{
			shutdownModem();

			if ( !initializeSDCard() ) break;

			if ( !initializeCamera() ) break;

			s_debugInterface.blink( 1 );

			if ( !takePicture() ) break;

			shutdownCamera();

			uint8_t retryCount = 0;
			while ( retryCount < SEND_PICTURE_RETRIES )
			{
				if ( initializeModem()  )
				{
					if ( initializeSDCard() )
					{
						if ( emailPicture() ) break; // Sent successfully
					}
				}

				shutdownModem();
				shutdownSDCard();

				sleep( 60 );

				retryCount += 1;
			}
		}
	}
	while ( false );

	shutdownSDCard();
	shutdownCamera();
	shutdownModem();
}

int main(void)
{
	init();

	setup();

	while ( true )
	{
		loop();

		sleep( s_settings.getSleepDuration() );
	}

	return 0;
}
