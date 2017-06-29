#include "smartport.h" 
#include <stdio.h> 

#ifdef SMARTPORT


void smartport_read(telemetry_data_t *td, uint8_t *buf, int buflen) 
{
    static uint8_t s = 0;
    static uint8_t e = 0;
    static uint8_t tBuffer[7];
    uint8_t b;
    int i;

    for(i=0; i<buflen; i++ )
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
			if( 0 != u8CheckCrcSPORT(tBuffer) )
			{
				smartport_check(td,tBuffer);
			}
			s=0;
		}
    }
}
uint8_t u8CheckCrcSPORT( uint8_t *b ) {
    uint16_t u16Crc = DATA_FRAME;
    uint8_t res;

    int i;

    for( i=0; i<6; i++ )
    {
        u16Crc += b[i];
        u16Crc += u16Crc >> 8;
        u16Crc &= 0x00ff;
    }

    if( (uint8_t)(0xFF-u16Crc) == b[6] )
    {
	return 1u;
    }
    else
    {
    	printf( "smartport crc fail (%x != %x)", (uint8_t)(0xFF-u16Crc), b[6] );
        return 0u;
    }
}

void smartport_check(telemetry_data_t *td, uint8_t *b) 
{
    tSPortData tel;

    tel.id   = (uint16_t)b[1];
    tel.id <<= 8u;
    tel.id  += (uint16_t)b[0];

    tel.data.b[0] = b[2];
    tel.data.b[1] = b[3];
    tel.data.b[2] = b[4];
    tel.data.b[3] = b[5];

    tel.crc = b[6];

    switch( tel.id )
	{
        case FR_ID_VFAS:
            // uint32_t , 100=10V
            td->voltage = (float)tel.data.u16 / 10.0;
            printf( "\r\nsmartport FR_ID_VFAS is %f", td->voltage );
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
			}
*/
                printf( "\r\nsmartport FR_ID_LATLONG is %f", td->longitude );
                }
            else{
                td->latitude = (float)(tel.data.u32 & 0x3fffffff);
                td->latitude /= 600000;
                if( tel.data.u32 & 0x40000000 ){
                    td->latitude = -td->latitude;
                }
/*
                if( (u8HomeFix & 2) == 0 ){
                    if( u8UavFixType > 3 ){
                        fHomeLat = fUavLat;
                        u8HomeFix |= 2;
                        }
                    }
*/
                printf( "\r\nsmartport FR_ID_LATLONG is %f", td->latitude );
                }
            break;
        case FR_ID_GPS_ALT:
            // uint32_t , 100=1m
            td->altitude = (float)(tel.data.i32) / 100.0;
            printf( "\r\nsmartport FR_ID_GPS_ALT is %f", td->altitude );
            break;
        case FR_ID_SPEED:
            // uint32_t , 2000=1kmh ???
            td->speed = (float)( tel.data.u32 ) / 2000.0;
            printf( "\r\nsmartport FR_ID_SPEED is %f", td->speed );
            break;
        case FR_ID_GPS_COURSE:
            // uint32_t , // 10000 = 100 deg
            td->heading = (float)( tel.data.u32 ) / 100.0;
            printf( "\r\nsmartport FR_ID_GPS_COURSE is %f", td->heading );
            break;
        case FR_ID_T1: // iNac, CF flight modes / arm
            //u16Modes = tel->d.data.u16; // see inav smartport.c //printf( "T1: %x", tel->d.data.u32 );
            printf( "\r\nsmartport FR_ID_T1" );
            break;
        case FR_ID_T2: // iNav, CF sat fix / home
            td->fix = (uint8_t)( tel.data.u32 / 1000 );
            td->sats = (uint8_t)( tel.data.u32 % 1000 );
            printf( "\r\nsmartport FR_ID_T2" );
            break;
        case FR_ID_GPS_SAT: // car ctrl sat fix
            td->fix = (uint8_t)( tel.data.u16 % 10 );
            td->sats = (uint8_t)( tel.data.u16 / 10 ); //printf( "Sat: %x", tel->d.data.u32 );
            printf( "\r\nsmartport FR_ID_GPS_SAT" );
            break;
        case FR_ID_RSSI:
            //u8UavRssi = tel.data.u8; //printf( "RSSI: %x - %x", u8UavRssi, tel->d.data.u32 );
            printf( "\r\nsmartport FR_ID_RSSI" );
            break;
        case FR_ID_RXBATT:
            //u8UavRxBatt = tel.data.u8;
            // factor is: 3.3 / 255 * 4 //printf( "Batt: %x - %x", u8UavRxBatt, tel->d.data.u32 );
            printf( "\r\nsmartport FR_ID_RXBATT" );
            break;
        case FR_ID_SWR:
            //u8UavSWR = tel.data.u8; //??? //printf( "SWR: %x - %x", u8UavSWR, tel->d.data.u32 );
            printf( "\r\nsmartport FR_ID_SWR" );
            break;
        case FR_ID_ADC1:
            //u8UavAdc1 = tel.data.u8;
            // factor is: 3.3 / 255
            break;
        case FR_ID_ADC2:
            //u8UavAdc2 = tel.data.u8;
            // factor is: 3.3 / 255
            printf( "\r\nsmartport FR_ID_ADC2" );
            break;
        case FR_ID_ALTITUDE:
            // uint32_t, from barometer, 100 = 1m
            td->baro_altitude = (float)(tel.data.i32) / 100.0;
            printf( "\r\nsmartport FR_ID_ALTITUDE is %f", td->baro_altitude );
            break;
        case FR_ID_VARIO:
            // uint32_t , 100 = 1m/s
            //i16UavVario = (int16_t)( tel.data.i32 );
            printf( "\r\nsmartport FR_ID_VARIO" );
            break;
        case FR_ID_ACCX:
            td->x = tel.data.i16;
            printf( "\r\nsmartport FR_ID_ACCX" );
            break;
        case FR_ID_ACCY:
            td->y = tel.data.i16;
            printf( "\r\nsmartport FR_ID_ACCY" );
            break;
        case FR_ID_ACCZ:
            td->z = tel.data.i16;
            printf( "\r\nsmartport FR_ID_ACCZ" );
            break;
        case FR_ID_CURRENT:
            td->ampere = (float)tel.data.u16 / 10.0;  // this is guessed
            printf( "\r\nsmartport FR_ID_CURRENT is %f", td->ampere );
            break;
        case FR_ID_CELLS:
        case FR_ID_CELLS_LAST:
        case FR_ID_RPM:
        case FR_ID_FUEL:
        case FR_ID_GPS_TIME_DATE:
        case FR_ID_A3_FIRST:
        case FR_ID_A4_FIRST:
        case FR_ID_AIR_SPEED_FIRST:
        case FR_ID_FIRMWARE:
        	printf( "\r\nsmartport id %x not used in osd", tel.id );
            break;
        default:
        	printf( "\r\nsmartport unknown id: %x , %x", (uint16_t)tel.id, tel.data.u32 );
            break;
        }
}

#endif

