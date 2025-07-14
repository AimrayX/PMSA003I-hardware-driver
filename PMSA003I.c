#include "PMSA003I.h"

#include <pigpio.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

int pmsa003i_begin(pmsa003i_t *dev, const char *bus) {
    dev->i2c_fd = pmsa003i_get_handle(bus);
    return 0;
}

int pmsa003i_reset(pmsa003i_t *dev) {

    return 0;
}

int pmsa003i_get_handle(const char *bus) {
    int handle = 0; 
    
    if((handle = open(bus, O_RDWR)) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to get handle\n", __FILE__, __LINE__);
        return -1;
    }

    if (ioctl(handle, I2C_SLAVE, PMSA003I_I2C_ADDR) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to talk to slave", __FILE__, __LINE__);
        close(handle);
        return -1;
    }
    return handle;
}