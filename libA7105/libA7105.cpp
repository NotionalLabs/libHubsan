#include "Arduino.h"
#include "libA7105.h"
#include <SPI.h>

libA7105::libA7105()
{
	_cspin = 0;
}

void libA7105::setupA7105(int cspin)
{
	// Configure SPI parameters for the Due and A7105.
	_cspin = cspin; // set user-configured SPI CS enable pin.
	SPI.begin(_cspin);
	SPI.setDataMode(_cspin,SPI_MODE0); // rising edge, clock idle low.
	SPI.setBitOrder(_cspin,MSBFIRST); // most-significant-bit first.

	writeRegister(A7105_00_MODE,0x00); // Issue full reset command to A7105.
	writeRegister(A7105_0B_GIO1_PIN_I,0x19); // configure 4-wire SPI (GIO1 to MISO)
}

void libA7105::readRegister(byte address, int len, byte pdata_r[])
{
	// Read data from the addressed control register - MULTIPLE BYTES
	SPI.transfer(_cspin,0x40 | address,SPI_CONTINUE); // READ flag + address.
   	for (int i=0;i<(len);i++){
   		if (i==(len-1)){
    		pdata_r[i] = SPI.transfer(_cspin,0x00); // release Chip-Select on last byte.
   		} else {
   			pdata_r[i] = SPI.transfer(_cspin,0x00,SPI_CONTINUE); // maintain Chip-Select till the penultimate byte.
   		}
   	}
}

void libA7105::readRegister(byte address, byte &pdata_r)
{
	// Read data from the addressed control register - SINGLE BYTE
	SPI.transfer(_cspin,0x40 | address,SPI_CONTINUE); // READ flag + address.
	pdata_r = SPI.transfer(_cspin,0x00);
}

void libA7105::writeRegister(byte address, int len, byte pdata_w[])
{
	// Write data to the addressed control register - MULTIPLE BYTES
	SPI.transfer(_cspin,address,SPI_CONTINUE); // Send address and Write flag.
	for (int i = 0; i<(len);i++){
		if (i==(len-1)){
			SPI.transfer(_cspin,pdata_w[i]); // release Chip-Select on last byte.
		} else {
			SPI.transfer(_cspin,pdata_w[i],SPI_CONTINUE); // maintain Chip-Select till the penultimate byte.
		}
	}
}

void libA7105::writeRegister(byte address, byte pdata_w)
{
	// Write data to the addressed control register - SINGLE BYTE
	SPI.transfer(_cspin,address,SPI_CONTINUE); // Send address and Write flag.
	SPI.transfer(_cspin,pdata_w); // release Chip-Select on last byte.
}

void libA7105::sendStrobe(enum A7105_State strobe)
{
	// Send A7105 state machine STROBE commands.
	SPI.transfer(_cspin,strobe);
}
