


#include "smartport.h"
#include <stdio.h>

#ifdef SMARTPORT



void smartport_read(telemetry_data_t *td, uint8_t *buf, int buflen)
{
    static uint8_t s = 0; 
    static uint8_t e = 0; 
    static tBuffer[7];

    uint8_t b;
    
    for( int i=0; i<buflen; i++ )
    {
    	b = *buf++;

		if( (e==0) && (b==0x7d) )
		{
			e = 1;
			continue;
		}
		if( e==1 )
		{
			e = 0;
			b = b + 0x20;
		}

		if( s==0 )
		{
			if( b==DATA_FRAME )
			{
				s++;
			}
		}
		else if( s<=6 )
		{
			tBuffer[s-1]=b;
			s++;
		}
		else
		{
			tBuffer[6]=b;
			if( u8CheckCrcSPORT(tBuffer) )
			{
				if( u8DecodeSPORT(tBuffer) )
					u32Counter++;
			}
			s=0;
		}
    }

}

bool u8CheckCrcSPORT( uint8_t *b )
{
    uint16_t u16Crc = DATA_FRAME;
    bool res;
    
    for( int i=0; i<6; i++ )
    {   
        u16Crc += b[i];
        u16Crc += u16Crc >> 8;  
        u16Crc &= 0x00ff;
    }
    res = ((uint8_t)(0xFF-u16Crc) == b[6]);
    if( !res )
    	printf( "smartport crc fail (%x != %x)", (uint8_t)(0xFF-u16Crc), b[6]  );
    return res;
}
 
void smartport_check(telemetry_data_t *td, uint8_t *b)
{
    tSPortData tel;
    tel.id        = (uint16_t)b[0] + ((uint16_t)b[1]) << 8;
    tel.data.b[0] = b[2];
    tel.data.b[1] = b[3];
    tel.data.b[2] = b[4];
    tel.data.b[3] = b[5];
    tel.crc       = b[6];

    switch( (uint16_t)tel.id ){
        case FR_ID_VFAS:
            // uint32_t , 100=10V
            u16UavBatt2 = tel.data.u16;
            break;
        case FR_ID_LATLONG:
            if( tel.data.u32 & 0x80000000 ){
            	td->longitude = (float)(tel.data.u32 & 0x3fffffff);
            	td->longitude /= 600000;
                if( tel.data.u32 & 0x40000000 ){
                	td->longitude = -td->longitude;
                    }
/*
                  if( (u8HomeFix & 1) == 0 ){
                    if( u8UavFixType > 3 ){
                        fHomeLon = fUavLon;
                        u8HomeFix |= 1;
                        }
*/
                    }
printf( "smartport LON: %x", tel.data.u32 );
                }
            else{    
                fUavLat = (float)(tel.data.u32 & 0x3fffffff);
                fUavLat /= 600000;
                if( tel.data.u32 & 0x40000000 ){
                    fUavLat = -fUavLat;
                }
                if( (u8HomeFix & 2) == 0 ){
                    if( u8UavFixType > 3 ){
                        fHomeLat = fUavLat;
                        u8HomeFix |= 2;
                        }
                    }
printf( "smartport LAT: %x", tel.data.u32 );
                }            
            break;
        case FR_ID_GPS_ALT:
            // uint32_t , 100=1m
            i16UavGpsAlt = (int16_t)( tel.data.i32 / 100 );
            break;
        case FR_ID_SPEED:
            // uint32_t , 2000=1kmh ???
            u16UavGpsSpeed = (uint16_t)( tel.data.u32 / 2000 );
            break;
        case FR_ID_GPS_COURSE:
            // uint32_t , // 10000 = 100 deg
            u16UavGpsCourse = (uint16_t)( tel.data.u32 / 100 );
            break;
        case FR_ID_T1:  // iNac, CF flight modes / arm
            u16Modes = tel->d.data.u16; // see inav smartport.c

//printf( "T1: %x", tel->d.data.u32 );
            break;
        case FR_ID_T2:  // iNav, CF sat fix / home
            u8UavFixType = (uint8_t)( tel.data.u32 / 1000 );
            u8UavSatCnt = (uint8_t)( tel.data.u32 % 1000 );
            break;
        case FR_ID_GPS_SAT: // car ctrl sat fix
            u8UavFixType = (uint8_t)( tel.data.u16 % 10 );
            u8UavSatCnt = (uint8_t)( tel.data.u16 / 10 );
//printf( "Sat: %x", tel->d.data.u32 );
            break;
        case FR_ID_RSSI:
            u8UavRssi = tel.data.u8;
//printf( "RSSI: %x - %x", u8UavRssi, tel->d.data.u32 );
            break;
        case FR_ID_RXBATT:
            u8UavRxBatt = tel.data.u8;
            // factor is: 3.3 / 255 * 4 
//printf( "Batt: %x - %x", u8UavRxBatt, tel->d.data.u32 );
            break;
        case FR_ID_SWR:
            u8UavSWR = tel.data.u8; //???
//printf( "SWR: %x - %x", u8UavSWR, tel->d.data.u32 );
            break;
        case FR_ID_ADC1:
            u8UavAdc1 = tel.data.u8;
            // factor is: 3.3 / 255
            break; 
        case FR_ID_ADC2:
            u8UavAdc2 = tel.data.u8;
            // factor is: 3.3 / 255
            break; 
        case FR_ID_ALTITUDE:
            // uint32_t, from barometer, 100 = 1m
            i16UavBaroAlt = (int16_t)( tel.data.i32 / 100 );
            break;
        case FR_ID_VARIO:
            // uint32_t , 100 = 1m/s
            i16UavVario = (int16_t)( tel.data.i32 );
            break;
        case FR_ID_CURRENT:
        case FR_ID_CELLS:
        case FR_ID_CELLS_LAST:
        case FR_ID_RPM:
        case FR_ID_FUEL:
        case FR_ID_ACCX:
        case FR_ID_ACCY:
        case FR_ID_ACCZ:
        case FR_ID_GPS_TIME_DATE:
        case FR_ID_A3_FIRST:
        case FR_ID_A4_FIRST:
        case FR_ID_AIR_SPEED_FIRST:
        case FR_ID_FIRMWARE:
            break;
        default:
        	printf( "smartport unknown id: %x - %x", (uint16_t)tel->d.id, tel->d.data.u32 );
            break;
        }
}


