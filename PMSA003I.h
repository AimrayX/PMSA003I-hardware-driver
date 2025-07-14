#ifndef _PMSA003I_H_
#define _PMSA003I_H_

#include "PMSA003I_defs.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct PMSA003I
{
    uint8_t i2c_addr;
    int i2c_fd;
    bool initialized;
    uint8_t last_error;
    uint16_t PM_1_0;
    uint16_t PM_2_5;
    uint16_t PM_10_0;
    uint16_t PM_1_0_atmospheric_environment;
    uint16_t PM_2_5_atmospheric_environment;
    uint16_t PM_10_0_atmospheric_environment;
    uint16_t PM_0_5_and_greater_in_0_1L;
    uint16_t PM_1_5_and_greater_in_0_1L;
    uint16_t PM_2_5_and_greater_in_0_1L;
    uint16_t PM_5_0_and_greater_in_0_1L;
    uint16_t PM_10_0_and_greater_in_0_1L;

}pmsa003i_t;

int pmsa003i_begin(pmsa003i_t *dev);

int pmsa003i_reset(pmsa003i_t *dev);

#endif