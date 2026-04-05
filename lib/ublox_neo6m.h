#ifndef SPEEDOMETER_UBLOX_NEO6M_H
#define SPEEDOMETER_UBLOX_NEO6M_H
#include <stdbool.h>

// $GPRMC,040302.663,A,3939.7,N,10506.6,W,0.27,358.86,200804,,*1A
struct gps_data {
    // UTC timestamp, HHMMSS
    char gprmc_timestamp[12];
    // Position status (A = data valid, V = data invalid)
    char gprmc_status[2];
    // ddmm.mmmm format
    char gprmc_latitude[12];
    // Latitude direction: (N = North, S = South)
    char gprmc_latitude_direction[2];
    // dddmm.mmmm format
    char gprmc_longitude[12];
    // Longitude direction: (E = East, W = West)
    char gprmc_longitude_direction[2];
    // Speed over ground, knots
    char gprmc_speed_knots[12];
    // Course over ground
    char gprmc_course_over_ground[12];
    // UTC date, DDMMYY
    char gprmc_date[12];
};

/**
 * @brief Initialize the u-blox NEO-6M GPS interface.
 *
 * Configures UART1 for GPS communication at 9600 baud and assigns
 * GPIO 4 as UART TX and GPIO 5 as UART RX.
 *
 * This function only performs local UART/GPIO setup. It does not
 * verify that the GPS module is connected, powered, or already
 * transmitting valid NMEA sentences.
 *
 * @return EXIT_SUCCESS on completion.
 */
int ublox_neo6m_init();

/**
 * @brief Read UART data until a complete NMEA sentence is available.
 *
 * Consumes all currently readable bytes from the GPS UART and stores
 * characters of the active sentence in the internal NMEA buffer.
 * Collection starts when a '$' start marker is received and ends when
 * a '\n' line terminator is received. Carriage returns are ignored.
 *
 * If the internal buffer overflows before the sentence is complete,
 * the current sentence is discarded.
 *
 * On success, the completed sentence is null-terminated and kept in the
 * module's internal buffer for later parsing with
 * `ublox_neo6m_parse_nmea_sentence()`.
 *
 * @return `true` if one complete NMEA sentence was received,
 *         otherwise `false`.
 */
bool ublox_neo6m_read_next_nmea_sentence();

/**
 * @brief Parse the last received NMEA sentence.
 *
 * Examines the module's internal NMEA buffer and, if the sentence is a
 * `$GPRMC` record, extracts selected fields into the internal `gps_data`
 * structure:
 * timestamp, status, latitude, latitude direction, longitude,
 * longitude direction, speed in knots course over ground and the date.
 *
 * If the buffered sentence is not a `$GPRMC` sentence, it is ignored.
 * If required fields cannot be parsed, the function prints an error and
 * leaves previously stored GPS values unchanged or partially updated.
 *
 * This function does not return a parse status; parsed data must be read
 * through the module's getter functions.
 */
void ublox_neo6m_parse_nmea_sentence();

/**
 * @brief Get the parsed GPS timestamp as a formatted time string.
 *
 * Converts the parsed GPRMC timestamp from `HHMMSS...` format to
 * `HH:MM:SS` and returns it from a static internal buffer.
 *
 * If the stored timestamp does not begin with at least six numeric
 * characters, the function returns the placeholder string `--:--:--`.
 *
 * @return Pointer to a static null-terminated string containing the
 *         formatted CEST time.
 *
 * @note The returned pointer refers to internal static storage and must
 *       not be modified or freed.
 */
char* ublox_neo6m_get_timestamp();

/**
 * @brief Get the parsed GPS date as a formatted date string.
 *
 * Converts the parsed GPRMC date from `DDMMYY` format to `DD.MM.YYYY`
 * and returns it from a static internal buffer.
 *
 * If the stored date does not begin with six numeric characters, the
 * function returns the placeholder string `--.--.----`.
 *
 * @return Pointer to a static null-terminated string containing the
 *         formatted UTC date.
 *
 * @note The returned pointer refers to internal static storage and must
 *       not be modified or freed.
 */
char* ublox_neo6m_get_date();

/**
 * @brief Get the parsed GPRMC status field.
 *
 * Returns the currently stored GPS status from the last parsed `$GPRMC`
 * sentence. Typical values are `A` for valid data and `V` for invalid data.
 *
 * @return Pointer to the internal null-terminated status string.
 *
 * @note The returned pointer refers to internal module storage and must
 *       not be modified or freed.
 */
char* ublox_neo6m_get_status();

/**
 * @brief Get the parsed GPS speed in kilometers per hour.
 *
 * Converts the stored `$GPRMC` speed value from knots to km/h and returns
 * the result as a formatted decimal string with two fractional digits.
 *
 * If no speed has been parsed yet, or the parsed field is empty or marked
 * as unknown (`?`), the function returns `"0"`.
 *
 * @return Pointer to a static null-terminated string containing the speed
 *         in km/h.
 *
 * @note The returned pointer refers to internal static storage and must
 *       not be modified or freed.
 */
char *ublox_neo6m_get_speed_kmh();

#endif //SPEEDOMETER_UBLOX_NEO6M_H
