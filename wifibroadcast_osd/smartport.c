


#include "smartport.h"
#include <stdio.h>

#ifdef SMARTPORT



void smartport_read(telemetry_data_t *td, uint8_t *buf, int buflen)
{
    static uint8_t s = 0; 
    static uint8_t e = 0; 
    static tSPortData tel;

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
				s = 1;
			}
		}
		else if( s<8 )
		{
			tel.b[s-1] = b;
			s++;
		}

		if( s>=8 )
		{
			s = 0;
			if( u8CheckCrcSPORT(&tel) )
			{
				if( u8DecodeSPORT(&tel) )
					u32Counter++;
			}
		}
    }

}

uint8_t u8CheckCrcSPORT( tSPortData *tel )
{
    uint16_t u16Crc = DATA_FRAME;
    uint8_t res;
    
    for( int i=0; i<6; i++ )
    {   
        u16Crc += tel->b[i];
        u16Crc += u16Crc >> 8;  
        u16Crc &= 0x00ff;
    }
    res = ((uint8_t)(0xFF-u16Crc) == (uint8_t)tel->b[6]) ? 1 : 0;
    if( res == 0 )
    	printf( "crc %x - %x", (uint8_t)(0xFF-u16Crc), (uint8_t)tel->b[6]  );
    return res;
}
 
void smartport_check(telemetry_data_t *td, tSPortData *tel)
{
    switch( (uint16_t)tel->d.id ){
        case FR_ID_VFAS:
            // uint32_t , 100=10V
            u16UavBatt2 = tel->d.data.u16;
            break;
        case FR_ID_LATLONG:
            if( tel->d.data.u32 & 0x80000000 ){
                fUavLon = (float)(tel->d.data.u32 & 0x3fffffff);
                fUavLon /= 600000;
                if( tel->d.data.u32 & 0x40000000 ){
                    fUavLon = -fUavLon;
                    }
                if( (u8HomeFix & 1) == 0 ){
                    if( u8UavFixType > 3 ){
                        fHomeLon = fUavLon;
                        u8HomeFix |= 1;
                        }
                    }
//printf( "Lon: %x", tel->d.data.u32 );
                }
            else{    
                fUavLat = (float)(tel->d.data.u32 & 0x3fffffff);
                fUavLat /= 600000;
                if( tel->d.data.u32 & 0x40000000 ){
                    fUavLat = -fUavLat;
                }
                if( (u8HomeFix & 2) == 0 ){
                    if( u8UavFixType > 3 ){
                        fHomeLat = fUavLat;
                        u8HomeFix |= 2;
                        }
                    }
//printf( " Lat: %x", tel->d.data.u32 );
                }            
            break;
        case FR_ID_GPS_ALT:
            // uint32_t , 100=1m
            i16UavGpsAlt = (int16_t)( tel->d.data.i32 / 100 );
            break;
        case FR_ID_SPEED:
            // uint32_t , 2000=1kmh ???
            u16UavGpsSpeed = (uint16_t)( tel->d.data.u32 / 2000 );
            break;
        case FR_ID_GPS_COURSE:
            // uint32_t , // 10000 = 100 deg
            u16UavGpsCourse = (uint16_t)( tel->d.data.u32 / 100 );
            break;
        case FR_ID_T1:  // iNac, CF flight modes / arm
            u16Modes = tel->d.data.u16; // see inav smartport.c

//printf( "T1: %x", tel->d.data.u32 );
            break;
        case FR_ID_T2:  // iNav, CF sat fix / home
            u8UavFixType = (uint8_t)( tel->d.data.u32 / 1000 );
            u8UavSatCnt = (uint8_t)( tel->d.data.u32 % 1000 );
            break;
        case FR_ID_GPS_SAT: // car ctrl sat fix
            u8UavFixType = (uint8_t)( tel->d.data.u16 % 10 );
            u8UavSatCnt = (uint8_t)( tel->d.data.u16 / 10 );
//printf( "Sat: %x", tel->d.data.u32 );
            break;
        case FR_ID_RSSI:
            u8UavRssi = tel->d.data.u8;
//printf( "RSSI: %x - %x", u8UavRssi, tel->d.data.u32 );
            break;
        case FR_ID_RXBATT:
            u8UavRxBatt = tel->d.data.u8;
            // factor is: 3.3 / 255 * 4 
//printf( "Batt: %x - %x", u8UavRxBatt, tel->d.data.u32 );
            break;
        case FR_ID_SWR:
            u8UavSWR = tel->d.data.u8; //???
//printf( "SWR: %x - %x", u8UavSWR, tel->d.data.u32 );
            break;
        case FR_ID_ADC1:
            u8UavAdc1 = tel->d.data.u8;
            // factor is: 3.3 / 255
            break; 
        case FR_ID_ADC2:
            u8UavAdc2 = tel->d.data.u8;
            // factor is: 3.3 / 255
            break; 
        case FR_ID_ALTITUDE:
            // uint32_t, from barometer, 100 = 1m
            i16UavBaroAlt = (int16_t)( tel->d.data.i32 / 100 );
            break;
        case FR_ID_VARIO:
            // uint32_t , 100 = 1m/s
            i16UavVario = (int16_t)( tel->d.data.i32 );
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
        	printf( "Unknown SPORT: %x - %x", (uint16_t)tel->d.id, tel->d.data.u32 );
            break;
        }
}


