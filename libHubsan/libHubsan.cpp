#include "Arduino.h"
#include "libA7105.h"
#include "libHubsan.h"

libA7105 a7105(); // Create A7105 interface object.
byte _channels[12] = {0x14,0x1E,0x28,0x32,0x3C,0x46,0x50,0x5A,0x64,0x6E,0x78,0x82};

libHubsan::libHubsan()
{
}

void libHubsan::init(int cspin)
{
	// Initialize A7015 settings for the Hubsan quadcopter.
	a7105.setupA7105(cspin); // Configure A7105 for SPI comms.

	// Set ID Code Register (x06) to "55 20 10 41".
	byte idcode[4] = {0x55,0x20,0x10,0x41};
	a7105.writeRegister(A7105_06_ID_DATA,4,idcode);
	// Set Mode Control Register (x01) Auto RSSI measurement, Auto IF Offset, FIFO mode enabled.
	a7105.writeRegister(A7105_01_MODE_CONTROL,0x63);
	// Set Calibration Control Reg (x02) - Reset.
	a7105.writeRegister(A7105_02_CALIB_CONT,0x00);
	// Set FIFO Register 1 (x03) - Set FIFO length to 16 bytes
	a7105.writeRegister(A7105_03_FIFO_I,0x0F);
	// Set FIFO Register 2 (x04) - 16 Byte TX/RX.
	a7105.writeRegister(A7105_04_FIFO_II,0xC0);
	// Set RC OSC Reg 1 (x07) - Reset.
	a7105.writeRegister(A7105_07_RC_OSC_I,0x00);
	// Set RC OSC Reg 2 (x08) - Reset.
	a7105.writeRegister(A7105_08_RC_OSC_II,0x00);
	// Set RC OSC Reg 3 (x09) - RC-oscillator Enable.
	a7105.writeRegister(A7105_09_RC_OSC_III,0x04);
	// Set CKO Pin Control Register (x0A) - Disable CLK out, TX clock, RX Recovery CLK, Non-Inverted CLK, Hi-Z CLK Out, Non-Inverted SPI Pin CLK.
	a7105.writeRegister(A7105_0A_CKO_PIN,0x00);
	// OMITTED: Set GIO1 Pin Control Register (x0B) - Reset.
	// Set GIO2 Pin Control Register (x0C) - GIO2 Pin Enable.
	a7105.writeRegister(A7105_0C_GIO2_PIN_II,0x01);
	// Set Clock Register (x0D) - Use Crystal Oscillator, CLK divider = /2.
	a7105.writeRegister(A7105_0D_CLOCK,0x05);
	// Set Data Rate Register (x0E) - Set data rate to 25kbps.
	a7105.writeRegister(A7105_0E_DATA_RATE,0x04);
	// Set PLL Register 1 (x0F) - Set Channel Offset to 80.
	a7105.writeRegister(A7105_0F_PLL_I,0x50);
	// Set PLL Register 2 (x10) - Reset.
	a7105.writeRegister(A7105_10_PLL_II,0x9E);
	// Set PLL Register 3 (x11) - Reset.
	a7105.writeRegister(A7105_11_PLL_III,0x4B);
	// Set PLL Register 4 (x12) - Reset.
	a7105.writeRegister(A7105_12_PLL_IV,0x00);
	// Set PLL Register 5 (x13) - Autofrequency Compensation.
	a7105.writeRegister(A7105_13_PLL_V,0x00);
	// Set TX Register 1 (x14) - Reset.
	a7105.writeRegister(A7105_14_TX_I,0x16);
	// Set TX Register 2 (x15) - Frequency Deviation: 186KHz
	a7105.writeRegister(A7105_15_TX_II,0x2B);
	// Set Delay Register 1 (x16) - Reset.
	a7105.writeRegister(A7105_16_DELAY_I,0x12);
	// Set Delay Register 2 (x17) - 200us settling delay, 10us AGC delay settling, 10us RSSI measurement delay.
	a7105.writeRegister(A7105_17_DELAY_II,0x00);
	// Set RX Register (x18) - BPF bandwidth = 500 KHz.
	a7105.writeRegister(A7105_18_RX,0x62);
	// Set RX Gain Register 1 (x19) - Manual VGA, Mixer Gain: 24dB, LNA gain: 24dB.
	a7105.writeRegister(A7105_19_RX_GAIN_I,0x80);
	// Set RX Gain Register 2 (x1A) - Internal Use?
	a7105.writeRegister(A7105_1A_RX_GAIN_II,0x80);
	// Set RX Gain Register 3 (x1B) - Internal Use?
	a7105.writeRegister(A7105_1B_RX_GAIN_III,0x00);
	// Set RX Gain Register 4 (x1C) - Reserved.
	a7105.writeRegister(A7105_1C_RX_GAIN_IV,0x0A);
	// Set RSSI Threshold Register (x1D) to x32.
	a7105.writeRegister(A7105_1D_RSSI_THRESH,0x32);
	// Set ADC Control Register (x1E) - RSSI Margin: 20, RSSI Measurement continue, FSARS: 4 MHZ, XADS = Convert RSS, RSSI measurement selected, RSSI continuous mode.
	a7105.writeRegister(A7105_1E_ADC,0xC3);
	// Set Code Register 1 (x1F) - Reset.
	a7105.writeRegister(A7105_1F_CODE_I,0x07);
	// Set Code Register 2 (x20) - Demodulator avg mode, 2 bit ID error code tolerance, 16bit  preamble detect.
	a7105.writeRegister(A7105_20_CODE_II,0x17);
	// Set Code Register 3 (x21) - Encryption Key (XOR…) - All zeroes (Data Whitening not enabled in register x1F.
	a7105.writeRegister(A7105_21_CODE_III,0x00);
	// Set IF Calibration Register (x22) - Autocalibrate.
	a7105.writeRegister(A7105_22_IF_CALIB_I,0x00);
	// Set VCO Current Calibration Reg (x24) - Autocalibrate.
	a7105.writeRegister(A7105_24_VCO_CUR_CALIB,0x00);
	// Set VCO Single band Calibration Reg (x25) - Autocalibrate.
	a7105.writeRegister(A7105_25_VCO_SB_CAL_I,0x00);
	// Set VCO Single band Calibration Reg 2 (x26) - Upper threshold: -1.3V, Lower threshold: 0.4V
	a7105.writeRegister(A7105_26_VCO_SB_CAL_II,0x3B);
	// Set Battery Detect Register (x27) - 2V battery detect threshold.
	a7105.writeRegister(A7105_27_BATT_DETECT,0x00);
	// Set TX Test Register (x28) - Reset.
	a7105.writeRegister(A7105_28_TX_TEST,0x17);
	// Set RX DEM test Reg (x29) - Internal Use?
	a7105.writeRegister(A7105_29_RX_DEM_TEST_I,0x47);
	// Set RX DEM test Reg 2 (x2A) - Reset.
	a7105.writeRegister(A7105_2A_RX_DEM_TEST_II,0x80);
	// Set Charge Pump Current Reg (x2B) - 2.0mA.
	a7105.writeRegister(A7105_2B_CHRG_PUMP_CUR,0x03);
	// Set Crystal Test Reg (x2C) - Internal Use?
	a7105.writeRegister(A7105_2C_XTAL_TEST,0x01);
	// Set PLL test Register (x2D) - Internal Use?
	a7105.writeRegister(A7105_2D_PLL_TEST,0x45);
	// Set VCO test Reg (x2E) - Internal Use?
	a7105.writeRegister(A7105_2E_VCO_TEST_I,0x18);
	// Set VCO test reg 2 (x2F) - Internal Use?
	a7105.writeRegister(A7105_2F_VCO_TEST_II,0x00);
	// Set IFAT reg (x30) - Internal Use?
	a7105.writeRegister(A7105_30_IFAT,0x01);
	// Set RSCALE reg (x31) - Reset.
	a7105.writeRegister(A7105_31_RSCALE,0x0F);

	// CALIBRATION TESTS
	byte test_result; // var to hold the test results for each calibration test in turn.
	int timeout; // var to hold timeout watchdog counter.

	// IF Filter Bank Calibration START.
	Serial.println("Performing IF Filter Bank Calibration Test.");
	timeout = 0;
	a7105.sendStrobe(A7105_STANDBY); // Strobe - STANDBY.
	a7105.writeRegister(A7105_02_CALIB_CONT,0x01); // Set Calibration Control Reg (x02) - IF Filter Bank calibration enable.
	while (true) {
		a7105.readRegister(A7105_02_CALIB_CONT,test_result);
		if (test_result == 0x00){
			break;
		}
		if (timeout > 10){ // Allow 10 iterations before signalling a failure.
			Serial.println("ERROR: IF Filter Bank Calibration Test FAILED - (timeout).");
		}
		timeout++;
		delayMicroseconds(20);
	}
	a7105.readRegister(A7105_22_IF_CALIB_I,test_result);
	if (CHECK_BIT(test_result,4)){
		Serial.println("ERROR: IF Filter Bank Calibration Test FAILED - FBCF Flag: " + String(test_result));
	} else {
		Serial.println(" - Passed.");
	}
	a7105.writeRegister(A7105_22_IF_CALIB_I,0x13); //Set IF Calibration Register - Configure relative control calibration.
	a7105.writeRegister(A7105_23_IF_CALIB_II,0x3B); // Set IF Calibration Register 2 - as above.

	// VCO Bank Calibration - TEST 1: START
	Serial.println("Performing VCO Bank Calibration - Test 1");
	timeout = 0;
	a7105.writeRegister(A7105_0F_PLL_I,0x00); // Set PLL Register 1 - Reset.
	a7105.sendStrobe(A7105_PLL); // Strobe - PLL Mode.
	a7105.writeRegister(A7105_02_CALIB_CONT,0x02); // Set Calibration Control Reg - VCO Bank Calibration enable.
	while (true) {
		a7105.readRegister(A7105_02_CALIB_CONT,test_result);
		if (test_result == 0x00){
			break;
		}
		if (timeout > 10){ // Allow 10 iterations before signalling a failure.
			Serial.println("ERROR: VCO Bank Calibration Test FAILED - (timeout).");
		}
		timeout++;
		delayMicroseconds(20);
	}
	a7105.readRegister(A7105_25_VCO_SB_CAL_I,test_result);
	if (CHECK_BIT(test_result,3)){
		Serial.println("ERROR: VCO Bank Calibration Test FAILED - VBCF Flag: " + String(test_result));
	} else {
		Serial.println(" - Passed.");
	}
	a7105.writeRegister(A7105_0F_PLL_I,0x78); // Set PLL Register 1 - Select Channel Offset.

	// VCO Bank Calibration - TEST 2: START
	Serial.println("Performing VCO Bank Calibration - Test 2");
	timeout = 0;
	a7105.sendStrobe(A7105_PLL); // Strobe - PLL Mode.
	a7105.writeRegister(A7105_02_CALIB_CONT,0x02); // Set Calibration Control Reg - VCO Bank Calibration enable.
	while (true) {
		a7105.readRegister(A7105_02_CALIB_CONT,test_result);
		if (test_result == 0x00){
			break;
		}
		if (timeout > 10){ // Allow 10 iterations before signalling a failure.
			Serial.println("ERROR: VCO Bank Calibration Test FAILED - (timeout).");
		}
		timeout++;
		delayMicroseconds(20);
	}
	a7105.readRegister(A7105_25_VCO_SB_CAL_I,test_result);
	if (CHECK_BIT(test_result,3)){
		Serial.println("ERROR: VCO Bank Calibration Test FAILED - VBCF Flag: " + String(test_result));
	} else {
		Serial.println(" - Passed.");
	}
	a7105.writeRegister(A7105_25_VCO_SB_CAL_I,0x0B); // Set VCO Single band Calibration Register 1 - Manual Calibration settings.

	a7105.sendStrobe(A7105_STANDBY);

	a7105.readRegister(A7105_06_ID_DATA,4,idcode);
	Serial.println("ID CODE: 0x" + String(idcode[0],HEX) + String(idcode[1],HEX) + String(idcode[2],HEX) + String(idcode[3],HEX));	

	a7105.writeRegister(A7105_0F_PLL_I,0xA0); // Set PLL Register 1 - Select Channel Offset.
	a7105.sendStrobe(A7105_PLL);
	a7105.sendStrobe(A7105_RX);

	// Setup RSSI measurement.
	a7105.writeRegister(A7105_1E_ADC,0xC3); // Set ADC Control Register (x1E) - RSSI Margin: 20, RSSI Measurement continue, FSARS: 4 MHZ, XADS = Convert RSS, RSSI measurement selected, RSSI continuous mode.

	// Cycle through the 12 channels and identify the best one to use.
	Serial.println("Scanning Channel RSSI values:");
	long chan_rssi[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	for (int i=0;i<12;i++){
		a7105.writeRegister(A7105_0F_PLL_I,_channels[i]); // Set PLL Register 1 - Select Channel Offset.
		a7105.sendStrobe(A7105_PLL);
		a7105.sendStrobe(A7105_RX);
		for (int j=0;j<15;j++){
			a7105.readRegister(A7105_1D_RSSI_THRESH,test_result);
			chan_rssi[i] = (chan_rssi[i] + test_result);
		}
	}
	int temp_rssi = 0;
	for (int i=0;i<12;i++){
		if (chan_rssi[i] > temp_rssi){
			temp_rssi = chan_rssi[i];
			_channel = _channels[i];
		}
	}
	Serial.println(" - Selected Channel: 0x" + String(_channel,HEX));
	a7105.writeRegister(A7105_28_TX_TEST,0x1F); // Set TX test Register - TX output: -4.8dBm, current: 14.9mA.
	a7105.writeRegister(A7105_19_RX_GAIN_I,0x9B); // Set RX Gain register - Manual, Mixer gain: 6dB, LNA gain: 6dB
	a7105.writeRegister(A7105_0F_PLL_I,_channel); // Set PLL Register 1 - Select Channel Offset to channel with the HIGHEST average RSSI from the scanning
	a7105.sendStrobe(A7105_PLL);
	a7105.sendStrobe(A7105_STANDBY);
}

void libHubsan::bind()
{
	Serial.println("Sending beacon packets...");
	byte status_byte = 0x00; // variable to hold W/R register data.

	// Generate 4 byte random session id.
	randomSeed(analogRead(0));
	for (int i=0;i<4;i++){
		_sessionid[i] = random(255);
	}

	for (int i=0;i<16;i++){ // Initialize packet array.
		_txpacket[i] = 0x00;
	}
	_txpacket[0] = 0x01; // Bind level = 01 (Unbound - BEACON lvl 1 Packet)
	_txpacket[1] = _channel; // Selected Channel
	for (int i=0;i<4;i++){
		_txpacket[i+2] = _sessionid[i];
	}
	getChecksum(_txpacket);

	// Transmit ANNOUNCE Packet until a response is heard.
	while (true){
		txPacket(_txpacket);
		a7105.sendStrobe(A7105_RX); // Switch to RX mode.
		bool response = false;
		for (int i=0;i<15;i++){ // Listen to see if there was a response.
			a7105.readRegister(A7105_00_MODE,status_byte);
			if (CHECK_BIT(status_byte,0)==false){
				response = true;
				break;
			}
			delay(1);
		}
		if (response){
			break;
		}
		a7105.sendStrobe(A7105_STANDBY);
	}
	rxPacket(_rxpacket);

	// Escalate handshake.
	_txpacket[0] = 0x03; // Bind Level = 01 (Unbound - BEACON lvl 3 Packet)
	getChecksum(_txpacket);
	while (true){
		txPacket(_txpacket);
		a7105.sendStrobe(A7105_RX); // Switch to RX mode.
		bool response = false;
		for (int i=0;i<15;i++){ // Listen to see if there was a response.
			a7105.readRegister(A7105_00_MODE,status_byte);
			if (CHECK_BIT(status_byte,0)==false){
				response = true;
				break;
			}
			delay(1);
		}
		if (response){
			break;
		}
		a7105.sendStrobe(A7105_STANDBY);
	}
	rxPacket(_rxpacket);

	// Set IDCode to the session value.
	a7105.writeRegister(A7105_06_ID_DATA,4,_sessionid);

	// Commence confirmation handshake.
	_txpacket[0] = 0x01; // Bind Level = 01 (Mid-Bind - Confirmation of IDCODE change packet)
	getChecksum(_txpacket);
	while (true){
		txPacket(_txpacket);
		a7105.sendStrobe(A7105_RX); // Switch to RX mode.
		bool response = false;
		for (int i=0;i<15;i++){ // Listen to see if there was a response.
			a7105.readRegister(A7105_00_MODE,status_byte);
			if (CHECK_BIT(status_byte,0)==false){
				response = true;
				break;
			}
			delay(1);
		}
		if (response){
			break;
		}
		a7105.sendStrobe(A7105_STANDBY);
	}
	rxPacket(_rxpacket);

	// Commence full handshake escalation.
	_txpacket[0] = 0x09;
	for (int i=0;i<10;i++){
		_txpacket[2] = byte(i);
		getChecksum(_txpacket);
		while (true){
			txPacket(_txpacket);
			a7105.sendStrobe(A7105_RX); // Switch to RX mode.
			bool response = false;
			for (int i=0;i<15;i++){ // Listen to see if there was a response.
				a7105.readRegister(A7105_00_MODE,status_byte);
				if (CHECK_BIT(status_byte,0)==false){
					response = true;
					break;
				}
				delay(1);
			}
			if (response){
				break;
			}
			a7105.sendStrobe(A7105_STANDBY);
		}
		rxPacket(_rxpacket);
	}
	a7105.writeRegister(A7105_1F_CODE_I,0x0F); // Enable FEC.
	a7105.sendStrobe(A7105_STANDBY);
}

void libHubsan::txPacket(byte ppacket[])
{
	// Transmit packet and wait until it has completed sending before returning.

	byte status_byte;
	a7105.sendStrobe(A7105_RST_WRPTR); // Reset FIFO Write Pointer.
	a7105.writeRegister(A7105_05_FIFO_DATA,16,ppacket);
	a7105.sendStrobe(A7105_TX); // Transmit packet.
	while (true){ // Check to see if the transmission has completed.
		a7105.readRegister(A7105_00_MODE,status_byte);
		if (CHECK_BIT(status_byte,0)==false){
			break;
		}
	}
}

void libHubsan::rxPacket(byte ppacket[])
{
	// Read received packet from FIFO buffer.

	a7105.sendStrobe(A7105_RST_RDPTR); // Reset the RX pointer.
	a7105.readRegister(A7105_05_FIFO_DATA,16,ppacket); // Read received packet.
}

void libHubsan::getChecksum(byte ppacket[])
{
	// Calculate checksum value. 256-(Sum of packet bytes 0-15, mod 256).

	int sum = 0;
	for (int i=0;i<15;i++){
		sum = sum + int(ppacket[i]);
	}
	ppacket[15] = byte(256-(sum % 256));	
}