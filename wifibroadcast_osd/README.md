# Wifibroadcast OSD 

copy files to /tmp/wifibroadcast_osd/

smartport.h ( new file )
smartport.c ( new file )
Makefile ( add smartport.o )
render.c
telemetry.h ( add data available on smartport only )
main.c ( call to smartport )


// select SMARTPORT in /boot/osdconfig.h 
#define SMARTPORT


// select in /boot/wifibroadcast-1.txt for example:
# Telemetry transmission method:
# wbc = use wifibroadcast as telemetry up/downlink
# external = use external means as telemetry up/downlink (LRS or 3DR dongles)
# if set to external, set serialport to which LRS or 3DR dongle is connected
# both on ground and air pi
TELEMETRY_TRANSMISSION=external
...
# serial port settings if using TELEMETRY_TRANSMISSION=external
EXTERNAL_TELEMETRY_SERIALPORT_GROUND=/dev/serial0
EXTERNAL_TELEMETRY_SERIALPORT_GROUND_BAUDRATE=57600
#
# set to "Y" to enable output of telemetry to serialport on ground Pi (for antenna tracker etc.)
ENABLE_SERIAL_TELEMETRY_OUTPUT=N

# connect the smartport line to RX of serial0
# connect GND
# TX line is not needed
