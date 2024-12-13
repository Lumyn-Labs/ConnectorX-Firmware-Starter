// Include the library
#include <MD5.h>

// Define Arduino Library
#ifndef Arduino_h
	#include <Arduino.h>
#endif

// Setup
void setup() {

	// Start Serial
	Serial.begin(115200);

	// Create the MD5 Hashing Class
	MD5 MD5_Hash;

	// Declare Constants
	char Data_Variable[] = "Hello World!";

	// MD5 Checksum
	MD5_Hash.Update(Data_Variable, sizeof(Data_Variable) - 1);

	// Calculate MD5
	uint8_t _Hash[16];
	MD5_Hash.Finalize(_Hash);

	// MD5 to Char Array
	char _Etag[33];
	MD5_Hash.Digest(_Hash, _Etag);

	// Print Data
	Serial.print("MD5 Data: "); Serial.println(Data_Variable);
	Serial.print("MD5 Hash: "); Serial.println(_Etag);
	
}

// Loop
void loop() {

}
