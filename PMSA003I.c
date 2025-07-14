#include "PMSA003I.h"

#include <pigpio.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

int pmsa003i_begin(pmsa003i_t *dev, const char *bus, uint8_t reset_pin, uint8_t set_pin) {
    dev->reset_pin = reset_pin;
    dev->set_pin = set_pin;
    
    if (gpioInitialise() < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to init gpio", __FILE__, __LINE__);
        return 1;
    }
    gpioSetMode(reset_pin, PI_INPUT);
    gpioSetMode(set_pin, PI_INPUT);
    pmsa003i_reset(dev);

    dev->i2c_fd = pmsa003i_get_handle(bus);
    dev->i2c_addr = PMSA003I_I2C_ADDR;
    return 0;
}

int pmsa003i_reset(pmsa003i_t *dev) {
    gpioWrite(dev->reset_pin, 0);
    sleep(1);
    gpioWrite(dev->reset_pin, 1);
    sleep(3);
    return 0;
}

int pmsa003i_set_WS(pmsa003i_t *dev, uint8_t working_state) {
    gpioWrite(dev->set_pin, working_state);
    return 0;
}

int pmsa003i_close(pmsa003i_t *dev) {
    if (dev->i2c_fd != 0) {
        close(dev->i2c_fd);
        dev->i2c_fd = 0;
    }
    gpioTerminate();
    return 0;
}

uint8_t pmsa003i_read_register(pmsa003i_t *dev, uint8_t reg) {
    if (ioctl(dev->i2c_fd, I2C_SLAVE, PMSA003I_I2C_ADDR) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to talk to slave", __FILE__, __LINE__);
        close(dev->i2c_fd);
        return 1;
    }

    if (write(dev->i2c_fd, &reg, 1) != 1) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to write register address", __FILE__, __LINE__);
        return 1;
    }

    uint8_t value;
    if (read(dev->i2c_fd, &value, 1) != 1) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to read from register", __FILE__, __LINE__);
        return 1;
    }

    return value;
}

int pmsa003i_get_handle(const char *bus) {
    int fd = 0; 
    
    if((fd = open(bus, O_RDWR)) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to get handle\n", __FILE__, __LINE__);
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, PMSA003I_I2C_ADDR) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to talk to slave", __FILE__, __LINE__);
        close(fd);
        return 1;
    }
    return fd;
}

uint16_t pmsa003i_read_PM_factory_calibrated(pmsa003i_t *dev, uint8_t size) {
    
    uint16_t rslt = 0;
    if (size == PMSA003I_PM_1_0) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA1_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA1_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_1_0_factory = rslt;

    } else if (size == PMSA003I_PM_2_5) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA2_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA2_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_2_5_factory = rslt;

    } else if (size == PMSA003I_PM_10_0) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA3_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA3_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_10_0_factory = rslt;

    } else {
        fprintf(stderr, "[ERROR] %s:%d: Size doesn't exist for this category", __FILE__, __LINE__);
        return 1;
    }
    
    return rslt;
}

uint16_t pmsa003i_read_PM_atmospheric_environment(pmsa003i_t *dev, uint8_t size) {
    uint16_t rslt = 0;
    if (size == PMSA003I_PM_1_0) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA4_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA4_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_1_0_atmospheric_environment = rslt;

    } else if (size == PMSA003I_PM_2_5) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA5_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA5_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_2_5_atmospheric_environment = rslt;

    } else if (size == PMSA003I_PM_10_0) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA6_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA6_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_10_0_atmospheric_environment = rslt;

    } else {
        fprintf(stderr, "[ERROR] %s:%d: Size doesn't exist for this category", __FILE__, __LINE__);
        return 1;
    }
    
    return rslt;
}

uint16_t pmsa003i_read_PM_size_and_greater_in_0_1L(pmsa003i_t *dev, uint8_t size) {
    uint16_t rslt = 0;
    if (size == PMSA003I_PM_0_3) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA7_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA7_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_0_3_and_greater_in_0_1L = rslt;

    } else if (size == PMSA003I_PM_0_5) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA8_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA8_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_0_5_and_greater_in_0_1L = rslt;

    } else if (size == PMSA003I_PM_1_0) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA9_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA9_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_1_0_and_greater_in_0_1L = rslt;

    } else if (size == PMSA003I_PM_2_5) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA10_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA10_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_2_5_and_greater_in_0_1L = rslt;
    } else if (size == PMSA003I_PM_5_0) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA11_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA11_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_5_0_and_greater_in_0_1L = rslt;
    } else if (size == PMSA003I_PM_10_0) {
        uint16_t high = pmsa003i_read_register(dev, PMSA003I_REG_DATA12_H);
        uint16_t low = pmsa003i_read_register(dev, PMSA003I_REG_DATA12_L);
        rslt = ((uint16_t)high << 8) | low;
        dev->PM_10_0_and_greater_in_0_1L = rslt;
    } else {
        fprintf(stderr, "[ERROR] %s:%d: Size doesn't exist at all or for this category", __FILE__, __LINE__);
        return 1;
    }
    
    return rslt;
}



