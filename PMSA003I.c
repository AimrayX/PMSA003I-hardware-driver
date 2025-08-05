#include "PMSA003I.h"

#include <pigpio.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <errno.h>

int pmsa003i_begin(pmsa003i_t *dev, int fd, uint8_t reset_pin, uint8_t set_pin) {
    dev->reset_pin = reset_pin;
    dev->set_pin = set_pin;
    dev->i2c_fd = fd;
    dev->i2c_addr = PMSA003I_I2C_ADDR;
    
    if (gpioInitialise() < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to init gpio\n", __FILE__, __LINE__);
        return -1;
    }
    if (gpioSetMode(reset_pin, PI_INPUT) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to set gpio mode\n", __FILE__, __LINE__);
        return -1;
    }
    if (gpioSetMode(set_pin, PI_INPUT) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to set gpio mode\n", __FILE__, __LINE__);
        return -1;
    }

    pmsa003i_reset(dev);

    pmsa003i_set_WS(dev, PMSA003I_WS_NORMAL);

    return 0;
}

int pmsa003i_reset(pmsa003i_t *dev) {
    if (gpioWrite(dev->reset_pin, 0) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to reset\n", __FILE__, __LINE__);
        return -1;
    }
    sleep(1);
    if (gpioWrite(dev->reset_pin, 1) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to reset\n", __FILE__, __LINE__);
        return -1;
    }
    sleep(3);
    return 0;
}

int pmsa003i_set_WS(pmsa003i_t *dev, uint8_t working_state) {
    if (gpioWrite(dev->set_pin, working_state) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to set working state\n", __FILE__, __LINE__);
        return -1;
    }
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

int pmsa003i_read_register(pmsa003i_t *dev) {
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    uint8_t buf[32];
    uint8_t start_reg = 0x00;

    messages[0].addr  = dev->i2c_addr;
    messages[0].flags = 0; // Write
    messages[0].len   = 1;
    messages[0].buf   = &start_reg;

    messages[1].addr  = dev->i2c_addr;
    messages[1].flags = I2C_M_RD; // Read
    messages[1].len   = 32;
    messages[1].buf   = buf;

    packets.msgs  = messages;
    packets.nmsgs = 2;

    if (ioctl(dev->i2c_fd, I2C_RDWR, &packets) < 0) {
        fprintf(stderr, "[ERROR] %s:%d: Failed to read register\n", __FILE__, __LINE__);
        return -1;
    }

    dev->PM_1_0_factory = (buf[PMSA003I_REG_DATA1_H] << 8) | buf[PMSA003I_REG_DATA1_L];
    dev->PM_2_5_factory = (buf[PMSA003I_REG_DATA2_H] << 8) | buf[PMSA003I_REG_DATA2_L];
    dev->PM_10_0_factory = (buf[PMSA003I_REG_DATA3_H] << 8) | buf[PMSA003I_REG_DATA3_L];
    dev->PM_1_0_atmospheric_environment = (buf[PMSA003I_REG_DATA4_H] << 8) | buf[PMSA003I_REG_DATA4_L];
    dev->PM_2_5_atmospheric_environment = (buf[PMSA003I_REG_DATA5_H] << 8) | buf[PMSA003I_REG_DATA5_L];
    dev->PM_10_0_atmospheric_environment = (buf[PMSA003I_REG_DATA6_H] << 8) | buf[PMSA003I_REG_DATA6_L];
    dev->PM_0_3_and_greater_in_0_1L = (buf[PMSA003I_REG_DATA7_H] << 8) | buf[PMSA003I_REG_DATA7_L];
    dev->PM_0_5_and_greater_in_0_1L = (buf[PMSA003I_REG_DATA8_H] << 8) | buf[PMSA003I_REG_DATA8_L];
    dev->PM_1_0_and_greater_in_0_1L = (buf[PMSA003I_REG_DATA9_H] << 8) | buf[PMSA003I_REG_DATA9_L];
    dev->PM_2_5_and_greater_in_0_1L = (buf[PMSA003I_REG_DATA10_H] << 8) | buf[PMSA003I_REG_DATA10_L];
    dev->PM_5_0_and_greater_in_0_1L = (buf[PMSA003I_REG_DATA11_H] << 8) | buf[PMSA003I_REG_DATA11_L];
    dev->PM_10_0_and_greater_in_0_1L = (buf[PMSA003I_REG_DATA12_H] << 8) | buf[PMSA003I_REG_DATA12_L];

    return 0;
}

uint16_t pmsa003i_read_PM_factory_calibrated(pmsa003i_t *dev, uint8_t size) {
    if (pmsa003i_read_register(dev) != 0)
    {
        return -1;
    }
    
    if (size == PMSA003I_PM_1_0) {
        return dev->PM_1_0_factory;

    } else if (size == PMSA003I_PM_2_5) {
        return dev->PM_2_5_factory;

    } else if (size == PMSA003I_PM_10_0) {
        return dev->PM_10_0_factory;

    } else {
        fprintf(stderr, "[ERROR] %s:%d: Size doesn't exist for this category\n", __FILE__, __LINE__);
        return -1;
    }
    
    return -1;
}

uint16_t pmsa003i_read_PM_atmospheric_environment(pmsa003i_t *dev, uint8_t size) {
    if (pmsa003i_read_register(dev) != 0)
    {
        return -1;
    }
    
    if (size == PMSA003I_PM_1_0) {
        return dev->PM_1_0_atmospheric_environment;

    } else if (size == PMSA003I_PM_2_5) {
        return dev->PM_2_5_atmospheric_environment;

    } else if (size == PMSA003I_PM_10_0) {
        return dev->PM_10_0_atmospheric_environment;

    } else {
        fprintf(stderr, "[ERROR] %s:%d: Size doesn't exist for this category\n", __FILE__, __LINE__);
        return -1;
    }
    
    return -1;
}

uint16_t pmsa003i_read_PM_size_and_greater_in_0_1L(pmsa003i_t *dev, uint8_t size) {
    if (pmsa003i_read_register(dev) != 0)
    {
        return -1;
    }

    if (size == PMSA003I_PM_0_3) {
        return dev->PM_0_3_and_greater_in_0_1L;

    } else if (size == PMSA003I_PM_0_5) {
        return dev->PM_0_5_and_greater_in_0_1L;

    } else if (size == PMSA003I_PM_1_0) {
        return dev->PM_1_0_and_greater_in_0_1L;

    } else if (size == PMSA003I_PM_2_5) {
        return dev->PM_2_5_and_greater_in_0_1L;

    } else if (size == PMSA003I_PM_5_0) {
        return dev->PM_5_0_and_greater_in_0_1L;

    } else if (size == PMSA003I_PM_10_0) {
        return dev->PM_10_0_and_greater_in_0_1L;

    } else {
        fprintf(stderr, "[ERROR] %s:%d: Size doesn't exist for this category\n", __FILE__, __LINE__);
        return -1;
    }
    
    return -1;
}



