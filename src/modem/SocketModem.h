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
 * This class contains interface to work with a socket modem.
 *
 *  Created on: Jan 25, 2011
 *      Author: Basil Shikin
 */

#ifndef SOKET_MODEM_H_
#define SOKET_MODEM_H_

#include "EmailCamSettings.h"
#include "DebugInterface.h"

#include <avr/pgmspace.h>
#include "Stream.h"
#include "wiring.h"

// Modem configuration
#define MODEM_BAUD_RATE 38400
#define BUFFER_SIZE 64
#define WAIT_BUFFER_SIZE 32
#define BOUND_SIZE 16

#define TIMEOUT_LONG_WAIT 30
#define TIMEOUT_SHORT_WAIT  5

class SocketModem {
public:

	/*
	 * Create new socket modem
	 */
	SocketModem( Stream * modemPort, uint8_t ctsPin, EmailCamSettings * settings, DebugInterface * debugInterface );

	/**
	 * Initialize the modem. Must be called before using it.
	 */
	bool initialize();

	/**
	 * Start a connection. This method should be called before an
	 * operation is performed.
	 */
	bool startConnection();

	/**
	 * Stop a connection.
	 */
	bool stopConnection();


	/**
	 * Shutdown the modem
	 */
	void shutdown();

	/**
	 * Check if configured mailbox has any new mail. Once mail checked, mark all
	 * mail as read.
	 */
	bool hasNewMail();

	/**
	 * Initialize sending of a JPEG image
	 */
	bool initalizeSendPicture();

	/**
	 * Send current buffer to the modem
	 */
	bool sendData( char * buffer );

	/**
	 * Commit sending of a picture.
	 */
	bool commitSendPicture();
private:

	// Parent variables
	Stream           * m_modemPort;
	EmailCamSettings * m_settings;
	DebugInterface   * m_debugInterface;
	uint8_t            m_ctsPin;

	// Controlled objects
	char m_buffer[ BUFFER_SIZE ];
	char m_waitBuffer[ WAIT_BUFFER_SIZE ];
	char m_boundary[ BOUND_SIZE ];

	int16_t m_randomNumber;

	void sendSetCommandP( PGM_P command, char * setting );
	void sendCommandP( PGM_P command );
	void sendP( PGM_P string );

	void readDebug();
	int8_t readResponse( char * buffer, uint8_t timeoutSeconds );
	int8_t readOK( uint8_t timeoutSeconds );

	int8_t waitForOK(char * buffer, uint8_t timeoutSeconds );
	int8_t waitForResponseP( PGM_P response, char * buffer, uint8_t timeoutSeconds );
};

#endif /* SOKET_MODEM_H_ */
