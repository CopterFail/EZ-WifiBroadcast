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
TELEMETRY_TRANSMISSION=external
...
EXTERNAL_TELEMETRY_SERIALPORT_GROUND=/dev/serial0
EXTERNAL_TELEMETRY_SERIALPORT_GROUND_BAUDRATE=57600
...
ENABLE_SERIAL_TELEMETRY_OUTPUT=N


# connect the smartport line to RX of serial0
# connect GND
# TX line is not needed
