/** BATTERY **/
#define CELLS 3
#define CELL_MAX 4.20
#define CELL_MIN 3.20
// warning level 1 -> orange, low
#define CELL_WARNING1 3.50
// warning level 2 -> red, critical
#define CELL_WARNING2 3.40

/** ARTIFICIAL HORIZON **/
// set to 1 or -1
#define INVERT_ROLL -1
#define INVERT_PITCH 1
// uncomment if roll and pitch are exchanged
//#define EXCHANGE_ROLL_AND_PITCH


/** HOME ARROW **/
// uncomment if home arrow is pointing in the wrong direction
//#define INVERT_HOME_ARROW


/** UNITS **/
// uncomment if you want to use imperial units
//#define IMPERIAL


/** DETAILS **/
// set to true if you want to show the ladders
#define DRAW_ALT_LADDER true
#define DRAW_SPEED_LADDER true
#define DRAW_COURSE_LADDER true
#define DRAW_AHI_LADDER true

/** MISC **/
// set to "true" for airspeed or "false" for groundspeed
#define USEAIRSPEED false
// set to "true" to use barometer for altitude or "false" for GPS altitude
#define USEBAROALT false

/** OSD ELEMENTS **/
// comment to disable feature
//#define RC_RSSI
#define VIDEO_RSSI
#define VIDEO_RSSI_DETAILED
//#define HEADING
//#define HOME_ARROW
//#define HORIZON
//#define BATT_REMAINING
#define BATT_STATUS
//#define DRAW_CURRENT
#define ALT
//#define SPEED
#define POSITION
//#define DISTANCE

/** DISPLAY **/
// Opacity of OSD, 1.0 = fully visible, 0.0 = fully opaque
// (black outline is not affected)
#define OPACITY 1.0
// Fontsize, 1.0 is default, useable range is from about 0.6 to 1.4
#define FONTSIZE 1.0
// comment to disable drawing of black outline
#define DRAW_OUTLINE

/** PROTOCOL **/
// FRSKY -> Frsky protocol (not much tested ...)
// MAVLINK -> Mavlink protocol (APM/Pixhawk/...)
// LTM -> Light Telemetry (Inav/Cleanflight/Betaflight ...)
//
//#define MAVLINK
#define SMARTPORT

