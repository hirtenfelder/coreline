#ifndef SPEEDOMETER_UBLOX_NEO6M_H
#define SPEEDOMETER_UBLOX_NEO6M_H
#include <stdbool.h>

// $GPRMC,040302.663,A,3939.7,N,10506.6,W,0.27,358.86,200804,,*1A
struct gps_data {
    // UTC timestamp, HHMMSS + null terminator
    char gprmc_timestamp[7];
    // Position status (A = data valid, V = data invalid)
    char gprmc_status;
    // ddmm.mmmm format
    char gprmc_latitude[12];
    // Latitude direction: (N = North, S = South)
    char gprmc_latitude_direction;
    // dddmm.mmmm format
    char gprmc_longitude[12];
    // Longitude direction: (E = East, W = West)
    char gprmc_longitude_direction;
    // Speed over ground, knots
    char gprmc_speed_knots[12];
};

int ublox_neo6m_init();

bool ublox_neo6m_read_next_nmea_sentence();

void ublox_neo6m_parse_nmea_sentence();

char* ublox_neo6m_get_timestamp();

char* ublox_neo6m_get_status();

#endif //SPEEDOMETER_UBLOX_NEO6M_H
