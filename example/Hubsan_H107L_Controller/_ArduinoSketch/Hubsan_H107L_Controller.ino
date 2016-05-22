/*
 Hubsan X4 H107L Quadcopter Controller for Arduino Due
  by: Jim Hung (www.jimhung.co.uk) - 2014-04-11

 - Serial baud: 115200
 - A7105 Transceiver SPI Chip Select pin: 4
*/
#include <SPI.h>
#include <libA7105.h>
#include <libHubsan.h>

int rev = 1; // Revision
byte controlbuffer[4] = {byte(0),byte(128),byte(128),byte(128)}; // Array to contain flight axis control values.
byte smoothbuffer[4] = {byte(0),byte(128),byte(128),byte(128)};
byte controlpacket[16]; // Array of control packet bytes.
byte inctype;
long prevMillis = 0;
long interval = 12;
int smooth = 0; // control interpolation/smoothing
libHubsan hubsan;

void setup() {
  // Initialize Serial comms
  Serial.begin(115200);
  Serial.println("Hubsan X4 H107L Quadcopter Controller for Arduino Due");
  Serial.println("Revision: " + String(rev));
  
  // Set up control packet structure.
  for (int i=0;i<16;i++){
    controlpacket[i] = 0x00;
  }
  controlpacket[0] = 0x20; // Bind state for control packets.
  controlpacket[9] = 0x0E; // Set initial control flags.
  controlpacket[10] = 0x19; // Always 19?
  
  // HARDCODED INITIAL TRIM!
  controlpacket[4] = 0x80;
  controlpacket[6] = 0x80; // MANUAL TRIM
  controlpacket[8] = 0x80; // MANUAL TRIM
  
  // Commence Hubsan Binding procedure.
  Serial.println("** Initializing Transceiver **");
  hubsan.init(4);
  Serial.println("** Commencing Bind Choreography **");
  hubsan.bind();
  Serial.println("** Status: READY. **");
}

void loop() {
  
  if (Serial.available() >= 2) {
    inctype = Serial.read(); // Get incoming control byte type.
    switch(inctype) {
      case '0':controlbuffer[0] = byte(Serial.read());break; // Elevation
      case '1':controlbuffer[1] = byte(Serial.read());break; // Yaw
      case '2':controlbuffer[2] = byte(Serial.read());break; // Pitch
      case '3':controlbuffer[3] = byte(Serial.read());break; // Roll
      case '4':toggleSmoothing();break;
      default:break;
    }
  }
  unsigned long currMillis = millis();
  if (currMillis - prevMillis > interval) {
    prevMillis = currMillis;
    updateControlPacket();
    sendControlPacket();
  }
}

void updateControlPacket() {
  if (smooth == 0) {
    controlpacket[2] = controlbuffer[0]; // Elevation
    controlpacket[4] = controlbuffer[1]; // Yaw
    controlpacket[6] = controlbuffer[2]; // Pitch
    controlpacket[8] = controlbuffer[3]; // Roll
  }
  else {
    smoothbuffer[0] = controlbuffer[0];
    smoothbuffer[1] = byte(int(smoothbuffer[1]) + ((int(smoothbuffer[1])-int(controlbuffer[1]))/2));
    smoothbuffer[2] = byte(int(smoothbuffer[2]) + ((int(smoothbuffer[2])-int(controlbuffer[2]))/2));
    smoothbuffer[3] = byte(int(smoothbuffer[3]) + ((int(smoothbuffer[3])-int(controlbuffer[3]))/2));

    controlpacket[2] = smoothbuffer[0]; // Elevation
    controlpacket[4] = smoothbuffer[1]; // Yaw
    controlpacket[6] = smoothbuffer[2]; // Pitch
    controlpacket[8] = smoothbuffer[3]; // Roll
  }
}

void sendControlPacket() {
  hubsan.getChecksum(controlpacket);
  hubsan.txPacket(controlpacket);
}

void toggleSmoothing() {
  if (smooth == 0){
    smooth = 1;
    Serial.println("Smoothing enabled.");
  }
  else {
    smooth = 0;
    Serial.println("Smoothing disabled.");
  }
}

