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
 * This class contains 
 *
 *  Created on: Feb 12, 2011
 *      Author: Basil Shikin
 */

#include "EmailCamSettings.h"

EmailCamSettings::EmailCamSettings()
{
}


bool EmailCamSettings::loadSettings(Fat16 settingsFile )
{
	uint16_t read = settingsFile.read();
	uint8_t  offset = 0;

	char buffer[ 60 ];

	while ( read > 0 && offset < 60 )
	{
		if ( read == '\n' )
		{
			buffer[offset] = '\0';

			if ( offset > 3 )
			{
				processLine( (char *)buffer );
			}
			offset = 0;
		}
		else
		{
			buffer[offset] = (char)read;

			offset += 1;
		}

		read = settingsFile.read();
	}

	return true;
}

char * EmailCamSettings::getAPN()
{
	return m_apn;
}
char * EmailCamSettings::getApnUsername()
{
	return m_apnUsername;
}

char * EmailCamSettings::getApnPassword()
{
	return m_apnPassword;
}

char * EmailCamSettings::getEmailPopServer()
{
	return m_emailPopServer;
}

char * EmailCamSettings::getEmailDomain()
{
	return m_emailDomain;
}

char * EmailCamSettings::getEmailPopServerPort()
{
	return m_emailPopServerPort;
}

char * EmailCamSettings::getEmailSmtpServer()
{
	return m_emailSmtpServer;
}

char * EmailCamSettings::getEmailSmtpServerPort()
{
	return m_emailSmtpServerPort;
}

char * EmailCamSettings::getEmailUsername()
{
	return m_emailUsername;
}

char * EmailCamSettings::getEmailPassword()
{
	return m_emailPassword;
}

char * EmailCamSettings::getEmailRecipient()
{
	return m_emailRecipient;
}

uint16_t EmailCamSettings::getSleepDuration()
{
	return m_sleepDuration;
}

void EmailCamSettings::processLine( char * line )
{
	if ( strstr( line, "#") != line )
	{
		uint8_t nameEnd = 0;
		while ( line[nameEnd] != '=' && nameEnd < strlen( line ) ) nameEnd += 1;

		uint8_t valueEnd = strlen( line ) - 1;
		while ( line[valueEnd] == '\n' || line[valueEnd]== '\r' || line[valueEnd] == ' ' || valueEnd == 0 ) valueEnd -= 1;

		if ( nameEnd >= valueEnd || valueEnd == 0 || nameEnd == strlen( line ) ) return;
		line[ nameEnd ] = '\0';

		size_t valueLength = valueEnd - nameEnd;
		char * value = (char *)malloc( valueLength + 1);
		memset(value, 0, valueLength + 1);
		memcpy( value, &line[ nameEnd + 1 ], valueLength );


		if ( strcmpP(line, PSTR("apn.address")) == 0 )
		{
			m_apn = value;
		}
		else if ( strcmpP(line, PSTR("apn.username")) == 0 )
		{
			m_apnUsername = value;
		}
		else if ( strcmpP(line, PSTR("apn.password")) == 0 )
		{
			m_apnPassword = value;
		}
		else if ( strcmpP(line, PSTR("email.pop.server")) == 0 )
		{
			m_emailPopServer = value;
		}
		else if ( strcmpP(line, PSTR("email.pop.port")) == 0 )
		{
			m_emailPopServerPort = value;
		}
		else if ( strcmpP(line, PSTR("email.domain")) == 0 )
		{
			m_emailDomain = value;
		}
		else if ( strcmpP(line, PSTR("email.smtp.server")) == 0 )
		{
			m_emailSmtpServer = value;
		}
		else if ( strcmpP(line, PSTR("email.smtp.port")) == 0 )
		{
			m_emailSmtpServerPort = value;
		}
		else if ( strcmpP(line, PSTR("email.username")) == 0 )
		{
			m_emailUsername = value;
		}
		else if ( strcmpP(line, PSTR("email.password")) == 0 )
		{
			m_emailPassword = value;
		}
		else if ( strcmpP(line, PSTR("email.recipient")) == 0 )
		{
			m_emailRecipient= value;
		}
		else if ( strcmpP(line, PSTR("sleep")) == 0 )
		{
			m_sleepDuration = atoi( value );
		}
	}
}

uint8_t EmailCamSettings::strcmpP( char * source, PGM_P sample )
{
	char * t = source;
	char s = (char)pgm_read_byte_near( sample++ );

	while ( *t != '\0' && s != '\0' )
	{
		if ( *t != s ) return -1;

		s = pgm_read_byte_near( sample++ );
		t++;
	}

	return *t == s ? 0 : 1;
}
