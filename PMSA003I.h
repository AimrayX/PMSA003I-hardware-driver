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

int pmsa003i_begin(pmsa003i_t *dev, const char *bus, uint8_t reset_pin, uint8_t set_pin);

int pmsa003i_reset(pmsa003i_t *dev);

int pmsa003i_get_handle(const char *bus);

int pmsa003i_set_WS(pmsa003i_t *dev, uint8_t working_state);

uint8_t pmsa003i_read_register(pmsa003i_t *dev);

uint16_t pmsa003i_read_PM_factory_calibrated(pmsa003i_t *dev, uint8_t size);

uint16_t pmsa003i_read_PM_atmospheric_environment(pmsa003i_t *dev, uint8_t size);

uint16_t pmsa003i_read_PM_size_and_greater_in_0_1L(pmsa003i_t *dev, uint8_t size);

#endif