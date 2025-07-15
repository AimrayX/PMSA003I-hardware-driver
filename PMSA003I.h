#ifndef _PMSA003I_H_
#define _PMSA003I_H_

#include "PMSA003I_defs.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PMSA003I
{
    uint8_t i2c_addr;
    int i2c_fd;
    bool initialized;
    uint8_t reset_pin;
    uint8_t set_pin;
    uint8_t last_error;
    uint16_t PM_1_0_factory;
    uint16_t PM_2_5_factory;
    uint16_t PM_10_0_factory;
    uint16_t PM_1_0_atmospheric_environment;
    uint16_t PM_2_5_atmospheric_environment;
    uint16_t PM_10_0_atmospheric_environment;
    uint16_t PM_0_3_and_greater_in_0_1L;
    uint16_t PM_0_5_and_greater_in_0_1L;
    uint16_t PM_1_0_and_greater_in_0_1L;
    uint16_t PM_2_5_and_greater_in_0_1L;
    uint16_t PM_5_0_and_greater_in_0_1L;
    uint16_t PM_10_0_and_greater_in_0_1L;

}pmsa003i_t;

/**
 * @brief Init and starts ensor
 *
 * 
 * @param dev Device pointer to pmsa003i struct
 * @param bus I2C bus of the sensor
 * @param reset_pin GPIO pin which the reset is connected to
 * @param set_pin GPIO pin which the set is connected to
 * @return Returns 0 if normal -1 for error
 */
int pmsa003i_begin(pmsa003i_t *dev, const char *bus, uint8_t reset_pin, uint8_t set_pin);

/**
 * @brief Resets sensor
 *
 * 
 * @param dev Device pointer to pmsa003i struct
 * @return Returns 0 if normal -1 for error
 */
int pmsa003i_reset(pmsa003i_t *dev);

/**
 * @brief Gets file descriptor of sensor
 *
 * 
 * @param dev Device pointer to pmsa003i struct
 * @return Returns 0 if normal -1 for error
 */
int pmsa003i_get_handle(const char *bus);

/**
 * @brief Sets working state of sensor
 *
 * 
 * @param dev Device pointer to pmsa003i struct
 * @param working_state PMSA003I_WS_NORMAL or PMSA003I_WS_SLEEP
 * @return Returns 0 if normal -1 for error
 */
int pmsa003i_set_WS(pmsa003i_t *dev, uint8_t working_state);

/**
 * @brief Reads register
 * 
 * This function does not return the read register value.
 * It reads the whole 32-bit register and sets the struct variables to the read values.
 * 
 * @param dev Device pointer to pmsa003i struct
 * @return Returns 0 if normal -1 for error
 */
int pmsa003i_read_register(pmsa003i_t *dev);

/**
 * @brief Reads the particles with factory calibration
 *
 * Supports PMSA003I_PM_1_0 / PMSA003I_PM_2_5 / PMSA003I_PM_10_0
 * 
 * @param dev Device pointer to pmsa003i struct
 * @param size The size of particles to read defined through \b macros
 * @return Returns factory calibrated values filtered by size
 */
uint16_t pmsa003i_read_PM_factory_calibrated(pmsa003i_t *dev, uint8_t size);

/**
 * @brief Reads the particles with atmospheric calibration
 *
 * Supports PMSA003I_PM_1_0 / PMSA003I_PM_2_5 / PMSA003I_PM_10_0
 * 
 * @param dev Device pointer to pmsa003i struct
 * @param size The size of particles to read defined through \b macros
 * @return Returns factory calibrated values filtered by size
 */
uint16_t pmsa003i_read_PM_atmospheric_environment(pmsa003i_t *dev, uint8_t size);

/**
 * @brief Reads the particles that are greater than the defined size
 *
 * Supports PMSA003I_PM_0_3 / PMSA003I_PM_0_5 / PMSA003I_PM_1_0 / PMSA003I_PM_2_5 / PMSA003I_PM_5_0 / PMSA003I_PM_10_0
 * 
 * @param dev Device pointer to pmsa003i struct
 * @param size The size of particles to read defined through \b macros
 * @return Returns factory calibrated values filtered by size
 */
uint16_t pmsa003i_read_PM_size_and_greater_in_0_1L(pmsa003i_t *dev, uint8_t size);

#endif