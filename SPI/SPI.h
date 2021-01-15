//
// Created by Andrey Pahomov
//

#ifndef DRIVERS_SPI_H
#define DRIVERS_SPI_H

#include <string>

#ifdef __linux__
#include <iostream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#elif _WIN32
// windows code goes here
#else

#endif


class ISPI {
public:

    enum SpiMode {
        MODE0 = 0|0,
        MODE1 = 0|SPI_CPHA,
        MODE2 = SPI_CPOL|0,
        MODE3 = SPI_CPOL|SPI_CPHA,

    };

    virtual int Init(std::string device, uint8_t mode, uint8_t bits, uint32_t speed, uint8_t cs) = 0;
    virtual uint16_t transfer16(uint16_t data) = 0;
    virtual uint8_t  transfer8(uint8_t data) = 0;
    virtual bool transferArray(uint8_t *data, uint32_t len) = 0;

    int fd;

    uint8_t _mode;

    uint8_t _cs;
    uint8_t _bits;
    uint32_t _speed;
    std::string _device;
};


class SPI : public ISPI {
public:

    int Init(std::string device, uint8_t mode, uint8_t bits, uint32_t speed, uint8_t cs);
    uint16_t transfer16(uint16_t data);
    uint8_t  transfer8(uint8_t data);
    bool transferArray(uint8_t *data, uint32_t len);

};

#endif
