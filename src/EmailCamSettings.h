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
 * This class contains settings for email cam
 *
 *  Created on: Feb 12, 2011
 *      Author: Basil Shikin
 */

#ifndef EMAILCAMSETTINGS_H_
#define EMAILCAMSETTINGS_H_

#include <inttypes.h>
#include "Fat16.h"

#include "SoftwareSerial.h"

class EmailCamSettings {
public:
	EmailCamSettings();

	/**
	 * Load settings form a given settings file
	 *
	 * @param settingsFile File to load settings from
	 */
	bool loadSettings(Fat16 settingsFile );

	char * getAPN();
	char * getApnUsername();
	char * getApnPassword();


	char * getEmailPopServer();
	char * getEmailDomain();
	char * getEmailPopServerPort();
	char * getEmailSmtpServer();
	char * getEmailSmtpServerPort();
	char * getEmailUsername();
	char * getEmailPassword();

	char * getEmailRecipient();

	uint16_t getSleepDuration();

private:
	char * m_apn;
	char * m_apnUsername;
	char * m_apnPassword;
	char * m_emailPopServer;
	char * m_emailDomain;
	char * m_emailPopServerPort;
	char * m_emailSmtpServer;
	char * m_emailSmtpServerPort;
	char * m_emailUsername;
	char * m_emailPassword;

	char * m_emailRecipient;

	int16_t m_sleepDuration;

	void processLine( char * line );
	char * readValue( char * line );
	uint8_t strcmpP( char * source, PGM_P sample );
};

#endif /* EMAILCAMSETTINGS_H_ */
