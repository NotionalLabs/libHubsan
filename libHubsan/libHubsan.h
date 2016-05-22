/*
	Hubsan_H107L.h - Arduino Library for interfacing with the Hubsan X4 Quadcopter.
	Note: This lib is based on reverse-engineering work performed on the H107L "Improved"/V2 model.
	Created by James E. Hung, April 14, 2014. www.jimhung.com
*/

#ifndef libHubsan_H
#define libHubsan_H

#include "Arduino.h"
#include "libA7105.h"

class libHubsan
{
public:
	libHubsan();
	void init(int cspin);
	void bind();
	void txPacket(byte *ppacket);
	void rxPacket(byte *ppacket);
	void getChecksum(byte *ppacket);
private:
	libA7105 a7105;
	byte _channel;
	byte _sessionid[4];
	byte _txpacket[16];
	byte _rxpacket[16];
};

#endif