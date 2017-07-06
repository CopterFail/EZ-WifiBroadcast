# Wifibroadcast OSD 

copy files to /root/wifibroadcast_osd/

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


# Connect Taranis to Rx_Pi

At the Taranis the battery tray uart can be used (need to be enabled at the menu) or the module port.
You can use a 2 wire cable for a direct connection (not very comfortable) or
you can use 2 paired HC05 bluetooth modules (3.3V)

connect the taranis smartport line to serial0 RX of the Rx_Pi
connect taranis GND to Rx_Pi GND
Rx_Pi TX line is not needed

If you are using HC05 modules, 3.3V supply is needed ( Rx_Pi has a 3V3 pin, Taranis has a Battery pin (~8V) and a 3V3 regulator is needed )
