/*
  SoftwareSerial.h - Software serial library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
 *
 * This class contains implementation of the modem module
 *
 *  Created on: Jan 25, 2011
 *      Author: Basil Shikin
 */

#include "SocketModem.h"


#define RES_OK "OK"


SocketModem::SocketModem( Stream * modemPort, uint8_t ctsPin, EmailCamSettings * settings, DebugInterface * debugInterface )
{
	m_modemPort = modemPort;
	m_ctsPin = ctsPin;
	m_settings = settings;
	m_debugInterface = debugInterface;

	m_randomNumber = analogRead(0);
}


bool SocketModem::initialize()
{
	m_debugInterface->status( DebugInterface::CMP_MODEM, 0x00 );

	// Mark CTS pin as input pin
	pinMode( m_ctsPin, INPUT );

	// Reset the modem
	sendCommandP( PSTR("AT+CFUN=1") );
	readOK( TIMEOUT_SHORT_WAIT );

	// Wait for "+WIND: 4"
	if ( waitForResponseP( PSTR("+WIND: 4"), m_buffer, TIMEOUT_LONG_WAIT ) < 0 ) return false;

	// Turn on full response
	sendCommandP( PSTR("ATV1") );
	readOK( TIMEOUT_SHORT_WAIT );

	sendCommandP( PSTR("ATE0") );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	delay(500); // Wait for the command to be processed

	// Open IP stack
	sendCommandP( PSTR("AT+WOPEN=1") );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	// Turn on GRPS mode
	sendCommandP( PSTR("AT#GPRSMODE=1") );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	// Setup APN server
	sendSetCommandP( PSTR( "AT#APNSERV="), m_settings->getAPN() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	if ( strlen( m_settings->getApnUsername() ) > 0 )
	{
		sendSetCommandP( PSTR( "AT#APNUN="), m_settings->getApnUsername() );
		if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;
	}

	if ( strlen( m_settings->getApnPassword() ) > 0 )
	{
		sendSetCommandP( PSTR( "AT#APNPW="), m_settings->getApnPassword() );
		if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;
	}

	// Attach to the network
	sendCommandP( PSTR("AT+CGATT=1") );
	if ( readOK( TIMEOUT_LONG_WAIT ) < 0 ) return false;

	// Wait for CREG notification
//	waitForResponseP( PSTR("+CGREG: 5"), m_buffer );
	if ( waitForResponseP( PSTR("+CGREG: 1"), m_buffer, TIMEOUT_LONG_WAIT ) < 0 ) return false;


	return true;
}


bool SocketModem::startConnection()
{
	// Start connection
	sendCommandP( PSTR("AT#CONNECTIONSTART") );

	// Wait for GRPS start
	if ( waitForResponseP( PSTR("Ok_Info_GprsActivation"), m_buffer, TIMEOUT_LONG_WAIT ) < 0 ) return false;

	m_debugInterface->status( DebugInterface::CMP_MODEM, 0x11 );

	return true;
}

bool SocketModem::stopConnection()
{
	m_debugInterface->status( DebugInterface::CMP_MODEM, 0xF0 );

	// Shut down current connection
	sendCommandP( PSTR("AT#CONNECTIONSTOP") );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	return true;
}

bool SocketModem::hasNewMail()
{
	m_debugInterface->status( DebugInterface::CMP_MODEM, 0x10 );

	// Setup email server
	sendSetCommandP( PSTR("AT#POP3SERV="), m_settings->getEmailPopServer() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendSetCommandP( PSTR("AT#POP3PORT="), m_settings->getEmailPopServerPort() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendSetCommandP( PSTR("AT#POP3UN="), m_settings->getEmailUsername() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendSetCommandP( PSTR("AT#POP3PW="), m_settings->getEmailPassword() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendCommandP( PSTR("AT#POP3HEADERMODE=0") );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	// Check for email
	sendCommandP( PSTR("AT#GETMAIL") );

	const char * hasMailStr = "Ok_Info_Mail";
	const char * noMailStr  = "Ok_Info_NoMail";
	const char * errorStr   = "ERROR";

	bool hasMail = false;
	while ( true )
	{
		if ( readResponse( m_buffer, TIMEOUT_LONG_WAIT ) < 0 ) return false;

		if ( strstr( m_buffer, noMailStr ) != NULL )
		{
			hasMail = false;

			if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;
			break;
		}

		if ( strstr( m_buffer, hasMailStr ) != NULL )
		{
			hasMail = true;
			if ( waitForOK( m_buffer, TIMEOUT_LONG_WAIT ) < 0 ) return false;

			break;
		}

		if ( strstr( m_buffer, errorStr ) != NULL )
		{
			hasMail = true;
			return false;
		}
	}

	m_debugInterface->status( DebugInterface::CMP_MODEM, 0x11 );

	return hasMail;
}

bool SocketModem::initalizeSendPicture()
{
	m_debugInterface->status( DebugInterface::CMP_MODEM, 0x14 );

	// Create random boundary
	randomSeed( m_randomNumber );
	for ( uint8_t i = 0; i < BOUND_SIZE - 1; i++ )
	{
		m_boundary[i] = (char)( random( 26 ) + 97 );
	}
	m_boundary[ BOUND_SIZE - 1 ] = '\0';



	sendSetCommandP( PSTR("AT#SMTPSERV="), m_settings->getEmailSmtpServer() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendSetCommandP( PSTR("AT#SMTPPORT="), m_settings->getEmailSmtpServerPort() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendSetCommandP( PSTR("AT#SENDERADDR="), m_settings->getEmailUsername() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendSetCommandP( PSTR("AT#DOMAIN="), m_settings->getEmailDomain() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendSetCommandP( PSTR("AT#SMTPUN="), m_settings->getEmailUsername() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendSetCommandP( PSTR("AT#SMTPPW="), m_settings->getEmailPassword() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendSetCommandP( PSTR("AT#REC1=" ), m_settings->getEmailRecipient() );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendCommandP( PSTR("AT#SENDERNAME=\"Email Camera\"" ) );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendCommandP( PSTR("AT#SUBJ1=\"Surveillance Update\"" ) );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendCommandP( PSTR("AT#SMTPAUTH=1") );
	if ( readOK( TIMEOUT_SHORT_WAIT ) < 0 ) return false;

	sendCommandP( PSTR("AT#PUTMAIL") );
	if ( waitForResponseP( PSTR("Ok_Info_WaitingForData"), m_buffer, TIMEOUT_LONG_WAIT ) < 0 ) return false;

	delay( 1000 );

	// Print JPEG image header
	sendP( PSTR("Content-type: multipart/mixed; boundary=Email-Cam-1-") );
	sendData( m_boundary );
	sendP( PSTR("\n") );
	sendP( PSTR("\n") );
	sendP( PSTR("\n") );
	sendP( PSTR("--Email-Cam-1-") );
	sendData( m_boundary );
	sendP( PSTR("\n") );
	sendP( PSTR("Content-Disposition: inline\n") );
	sendP( PSTR("\tfilename=SurveillanceUpdate.jpg\n") );
	sendP( PSTR("Content-Type: image/jpg;\n") );
	sendP( PSTR("\tx-unix-mode=0644;\n") );
	sendP( PSTR("\tname=\"SurveillanceUpdate.jpg\"\n") );
	sendP( PSTR("Content-Transfer-Encoding: base64\n") );
	sendP( PSTR("\n" ) );

	delay( 1000 );

	return true;
}

bool SocketModem::sendData( char * buffer )
{
	// Wait until it is clear to send
	while ( digitalRead( m_ctsPin ) == HIGH ) delay( 10 );

	// Send data
	m_modemPort->print( buffer);

	delay( 50 );

	return true;
}

bool SocketModem::commitSendPicture()
{
	sendP( PSTR("\n" ) );
	sendP( PSTR("\n" ) );

	sendP( PSTR("--Email-Cam-1-" ) );
	sendData( m_boundary );
	sendP( PSTR("--" ) );

	delay( 300 );
	
	sendP( PSTR("\r\n.\r\n" ) );

	if ( readOK( TIMEOUT_LONG_WAIT ) < 0 ) return false;

	return true;
}


void SocketModem::shutdown()
{
	m_debugInterface->status( DebugInterface::CMP_MODEM, 0xF1 );

	sendCommandP( PSTR("AT+CFUN=1,1") );
	readOK( TIMEOUT_SHORT_WAIT );

	sendCommandP( PSTR("AT+CPOF=1" ) );
	readOK( TIMEOUT_SHORT_WAIT );
}

void SocketModem::sendSetCommandP( PGM_P command, char * setting )
{
	uint8_t b;
	uint8_t o = 0;
	while( true )
	{
		b = pgm_read_byte_near( command++);
		if( b == '\0') break;

		m_buffer[o++] = (char)b;
	}

	m_buffer[o++] = '\"';
	while( true )
	{
		b = *setting++;
		if ( b == '\0') break;

		m_buffer[o++] = b;
	}
	m_buffer[o++] = '\"';
	m_buffer[o++] = '\r';
	m_buffer[o++] = '\0';

	sendData( m_buffer );

	m_debugInterface->debug( m_buffer );
}

void SocketModem::sendCommandP( PGM_P command )
{
	uint8_t b = 0;
	uint8_t o = 0;
	while( true )
	{
		b = pgm_read_byte_near( command++);
		if ( b == '\0') break;

		m_buffer[o++] = (char)b;
	}
	m_buffer[o++] = '\r';
	m_buffer[o++] = '\0';

	sendData( m_buffer );
	m_debugInterface->debug( m_buffer );
}

void SocketModem::sendP( PGM_P string )
{
	uint8_t b = 0;
	uint8_t o = 0;

	while( true )
	{
		// Reset counter
		o = 0;

		// Fill buffer
		while ( o < BUFFER_SIZE - 1 )
		{
			b = pgm_read_byte_near( string++);
			if ( b == '\0') break;

			m_buffer[o++] = (char)b;
		}

		// Write last character (zero)
		m_buffer[o++] = '\0';

		// Send to modem port and debug output
		sendData( m_buffer );

		// If we stumbled upon the last char, exit
		if ( b == '\0') return;
	}
}

int8_t SocketModem::readResponse(char * buffer, uint8_t timeoutSeconds )
{
	bool responseStarted = false;
	uint16_t offset = 0;
	uint16_t delayMillis = 0;

	while ( offset < BUFFER_SIZE )
	{
		delayMillis = 0;

		// Wait for the next byte
		while ( !m_modemPort->available() )
		{
			delay(100);

			delayMillis += 100;

			// Increase random number
			m_randomNumber += 1;

			if ( delayMillis/1000 > timeoutSeconds )
			{
				return -1;
			}
		}

		buffer[ offset ] = m_modemPort->read();

		if ( offset > 0 && buffer[offset - 1] == '\r' && buffer[offset] == '\n' )
		{
			if ( responseStarted )
			{
				buffer[ offset - 1 ] = '\0';
				break;
			}
			else
			{
				responseStarted = true;
				offset = 0;
			}
		}
		else
		{
			offset += 1;
		}
	}

	m_debugInterface->debug( buffer );
}

int8_t SocketModem::waitForOK(char * buffer, uint8_t timeoutSeconds )
{
	uint16_t offset = 0;
	uint16_t delayMillis = 0;

	while ( true )
	{
		delayMillis = 0;

		// Wait for the next byte
		while (!m_modemPort->available())
		{
			delay(100);

			delayMillis += 100;

			if ( delayMillis/1000 > timeoutSeconds )
			{
				return -1;
			}
		}

		buffer[ offset ] = m_modemPort->read();

		if ( offset > 4 )
		{
			if (
				 buffer[ 0 ] == '\r' &&
				 buffer[ 1 ] == '\n' &&
				 buffer[ 2 ] == 'O'  &&
				 buffer[ 3 ] == 'K'  &&
				 buffer[ 4 ] == '\r' &&
			     buffer[ 5 ] == '\n' )
			{
				break;
			}
			else
			{
				buffer[ 0 ] = buffer [ 1 ];
				buffer[ 1 ] = buffer [ 2 ];
				buffer[ 2 ] = buffer [ 3 ];
				buffer[ 3 ] = buffer [ 4 ];
				buffer[ 4 ] = buffer [ 5 ];

				offset = 5;
			}
		}
		else
		{
			offset += 1;
		}
	}

	buffer[offset] = '\0';

	return offset;
}

int8_t SocketModem::waitForResponseP( PGM_P response, char * buffer, uint8_t timeoutSeconds )
{
	uint8_t b = 0;
	uint8_t o = 0;
	while( o < WAIT_BUFFER_SIZE )
	{
		b = pgm_read_byte_near( response++ );
		m_waitBuffer[o++] = (char)b;

		if ( b == '\0') break;
	}

	bool keepWaiting = true;

	while ( keepWaiting )
	{
		delay( 1000 );

		int8_t read = readResponse( buffer, timeoutSeconds );
		if ( read < 0 ) return -1;

		if ( strstr( buffer, m_waitBuffer ) != NULL ) keepWaiting = false;
		if ( strstr( buffer, "ERROR" ) != NULL ) return -2;
	}

	return 1;
}

int8_t SocketModem::readOK(uint8_t timeoutSeconds )
{
	int8_t read = readResponse( m_buffer, timeoutSeconds );
	if ( read < 0 ) return -1;

	if ( strstr( m_buffer, RES_OK ) != NULL ) return 1;

	return -1;
}
