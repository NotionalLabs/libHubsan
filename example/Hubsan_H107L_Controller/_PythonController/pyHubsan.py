# ---------------------------------------------------------------
# Hubsan X4 H107L - Python head-end for Arduino Hubsan Controller
# ---------------------------------------------------------------
# v0.1 - 20141201 - James Hung (jimhung.com)
# ---------------------------------------------------------------
import serial,sys,configparser

from serial.tools import list_ports
import pygame
from time import sleep

def toggleSmoothing(smooth):
	if smooth == 0:
		smooth = 1
	else:
		smooth = 0
	return smooth

def flip(ser,def_dir,throttle,axis):

	# Blip the throttle...
	ser.write('0')
	ser.write(chr(254))
	sleep(0.2)
	# Pitch back momentarily...
	ser.write(axis)
	ser.write(chr(254))
	sleep(0.2)
	# ..then Pitch forward hard.
	ser.write(axis)
	ser.write(chr(1))
	sleep(0.5)
	# Return controls to default and compensate throttle momentarily...
	ser.write(axis)
	ser.write(chr(def_dir))
	ser.write('0')
	ser.write(chr(255))
	sleep(1)
	# ...and finally even out the throttle.
	ser.write('0')
	ser.write(chr(throttle))

def main():

	print "\nHubsan X4 H107L - Python head-end for Arduino Hubsan Controller."
	print "----------------------------------------------------------------"

	print "> Loading config settings...\n"
	config = configparser.ConfigParser()
	config.read('pyHubsan.config')

	print "> Detecting COM ports...\n"
	ports = list()
	i = 0
	for port in list_ports.comports():
		print "  %s: [%s] %s" % (str(i),port[0],port[1])
		ports.append(port)
		i+=1
	if (len(ports)==0):
		print "Error: No Arduino Serial ports detected! Try reconnecting the USB cable."
		sys.exit(1)
	portnum = input("\n  Enter the number of the desired COM port: ")
	comport = ports[portnum]
	print "\n- [%s] Selected." % comport[0]

	print "\n> Opening Serial port..."
	ser = serial.Serial(comport[0],115200,timeout=1)
	print " - Successful."

	print "\n> TERMINAL STATUS:"
	print "------------------"
	while True:
		line = ser.readline().strip()
		if line <> "":
			print " %s" % line	
		if (line == "** Status: READY. **"):
			break

	print "\n> INITIALIZING CONTROL INTERFACE."
	print "---------------------------------"

	# Set up control values
	def_t = throttle = 0
	def_y = yaw = 128 + int(config['flight']['yaw_trim'])
	def_p = pitch = 128 + int(config['flight']['pitch_trim'])
	def_r = roll = 128 + int(config['flight']['roll_trim'])
	print " Post-Trim Flight Defaults = Yaw: %s Pitch: %s Roll: %s" % (str(yaw),str(pitch),str(roll))
	smooth = 0

	move_throttle = move_yaw = move_pitch = move_roll = 0 # vars to aggregate control inputs
	last_t = last_y = last_p = last_r = last_s = 0 # vars to detect changes in control values.

	# Set up PyGame event handlers and event loop
	pygame.init()
	window = pygame.display.set_mode((640,480))
	pygame.display.set_caption("Hubsan X4 H107L Python Controller v0.1")
	clock = pygame.time.Clock()

	print "\nTHR:YAW:PIT:ROL\n---------------"

	while True:

		clock.tick(200) # Lock framerate of the input for consistency.

		for event in pygame.event.get():
			if (event.type == pygame.QUIT):
				sys.exit(0)
			if (event.type == pygame.KEYDOWN):
				if (event.key == pygame.K_SPACE): # KILL ENGINE AND RESET ALL CONTROLS
					throttle = def_t 
					yaw = def_y
					pitch = def_p
					roll = def_r
				if (event.key == pygame.K_w):
					move_throttle = 1
				if (event.key == pygame.K_s):
					move_throttle = -1
				if (event.key == pygame.K_a):
					yaw = def_y - (255-def_y)/3
				if (event.key == pygame.K_d):
					yaw = def_y + (255-def_y)/3
				if (event.key == pygame.K_UP):
					pitch = def_p - (255-def_p)/3
				if (event.key == pygame.K_DOWN):
					pitch = def_p + (255-def_p)/3
				if (event.key == pygame.K_LEFT):
					roll = def_r + (255-def_r)/3
				if (event.key == pygame.K_RIGHT):
					roll = def_r - (255-def_r)/3
				if (event.key == pygame.K_q):
					smooth = toggleSmoothing(smooth)
				if (event.key == pygame.K_e):
					flip(ser,def_p,throttle,"2")
			if (event.type == pygame.KEYUP):
				if (event.key == pygame.K_w):
					move_throttle = 0
				if (event.key == pygame.K_s):
					move_throttle = 0
				if (event.key == pygame.K_a):
					yaw = def_y
				if (event.key == pygame.K_d):
					yaw = def_y
				if (event.key == pygame.K_UP):
					pitch = def_p
				if (event.key == pygame.K_DOWN):
					pitch = def_p
				if (event.key == pygame.K_LEFT):
					roll = def_r
				if (event.key == pygame.K_RIGHT):
					roll = def_r
		
		throttle+=move_throttle

		# Constrain the control values to a rational range
		if (throttle > 255): throttle = 255
		if (throttle < 0): throttle = 0

		# See if the control value has changed, and if so, send a control message.
		if (throttle <> last_t):
			last_t = throttle
			ser.write('0') # Control flag: Throttle
			ser.write(chr(throttle))
			sys.stdout.write("\r%03d:%03d:%03d:%03d" % (throttle,yaw,pitch,roll))
			sys.stdout.flush()
		if (yaw <> last_y):
			last_y = yaw
			ser.write('1') # Control flag: Yaw
			ser.write(chr(yaw))
			sys.stdout.write("\r%03d:%03d:%03d:%03d" % (throttle,yaw,pitch,roll))
			sys.stdout.flush()
		if (pitch <> last_p):
			last_p = pitch
			ser.write('2') # Control flag: Pitch
			ser.write(chr(pitch))
			sys.stdout.write("\r%03d:%03d:%03d:%03d" % (throttle,yaw,pitch,roll))
			sys.stdout.flush()
		if (roll <> last_r):
			last_r = roll
			ser.write('3') # Control flag: Roll
			ser.write(chr(roll))
			sys.stdout.write("\r%03d:%03d:%03d:%03d" % (throttle,yaw,pitch,roll))
			sys.stdout.flush()
		if (smooth <> last_s):
			last_s = smooth
			ser.write('4') # Control flag: Toggle Smoothing
			print str(smooth)

if __name__ == '__main__':
    main()