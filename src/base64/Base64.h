#ifndef _BASE64_H
#define _BASE64_H

#include <inttypes.h>

/**
 * Edited by Basil Shikin
 */

class Base64 {
public:

	Base64();

	/**
	 * Encode next chunk of raw data and write it into the output array.
	 *
	 * @param output   the output buffer for the encoding, stores the encoded string
	 * @param input    the input buffer for the encoding, stores the binary to be encoded
	 * @param length   the length of the input buffer, in bytes
	 *
	 * @return length of the encoded string
	 */
	uint16_t encodeChunk(char *output, char *input, uint16_t length);

	uint16_t finishEncoding(char *output);

private:
	/* b64_alphabet:
	 * 		Description: Base64 alphabet table, a mapping between integers
	 * 					 and base64 digits
	 * 		Notes: This is an extern here but is defined in Base64.c
	 */
	const char * b64_alphabet;

	/* 'Private' declarations */
	inline void a3_to_a4(unsigned char * a4, unsigned char * a3);

	int i;
	unsigned char a3[3];
	unsigned char a4[4];
};

#endif // _BASE64_H
