/*
 Hubsan X4 H107L Quadcopter Controller for Arduino Uno
  by: Jim Hung (www.jimhung.co.uk) - 2016-05-27
 - Serial baud: 115200
*/

int rev = 1; // Revision
int controlbuffer[4] = {0,128,128,128}; // Array to contain flight axis control values.
byte inctype;
long prevMillis = 0;

int throttlepin = 10;
int yawpin = 11;
int pitchpin = 9;
int rollpin = 6;

void setup() {
  // Initialize output pins
  pinMode(throttlepin, OUTPUT);
  pinMode(yawpin, OUTPUT);
  pinMode(pitchpin, OUTPUT);
  pinMode(rollpin, OUTPUT);    
  
  // Initialize Serial comms
  Serial.begin(115200);
  Serial.println("Hubsan X4 H107L Quadcopter Input Emulator Controller for Arduino Uno");
  Serial.println("Revision: " + String(rev));

  // Commence Hubsan Binding procedure.
  Serial.println("** Status: READY. **");
}

void loop() {
  
  if (Serial.available() >= 2) {
    inctype = Serial.read(); // Get incoming control byte type.
    switch(inctype) {
      case '0':controlbuffer[0] = int(Serial.read());break; // Elevation
      case '1':controlbuffer[1] = int(Serial.read());break; // Yaw
      case '2':controlbuffer[2] = int(Serial.read());break; // Pitch
      case '3':controlbuffer[3] = int(Serial.read());break; // Roll
      default:break;
    }
  } 
  analogWrite(throttlepin,map(controlbuffer[0],0,255,0,168));
  analogWrite(yawpin,map(controlbuffer[1],0,255,0,168));
  analogWrite(pitchpin,map(controlbuffer[2],0,255,0,168));
  analogWrite(rollpin,map(controlbuffer[3],0,255,0,168));
}


