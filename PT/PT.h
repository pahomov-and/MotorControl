//
// Created by Andrey Pahomov
//

#ifndef PT_H
#define PT_H

#include "SPI/SPI.h"

namespace Eagle {

    const uint32_t PACKAGE_SIZE = 5;

    enum Command {
        LEFT    = 3,
        RIGHT   = 4,
        UP      = 5,
        DOWN    = 6,

        FIRE            = 230,
        PAN_STEP_LEFT   = 231,
        PAN_STEP_RIGHT  = 232,
        TILT_STEP_UP    = 233,
        TILT_STEP_DOWN  = 234,


    };

    class PT {
    private:
        bool _debug;
        uint8_t _cs;
        SPI &_spi;

        enum PosByte {
            START = 0,
            ADRESS = 1,
            COMAND = 2,
            OPTION = 3,
            CHECK_SUM = 4
        };



    public:
        PT(SPI &spi, bool debug = false);
        void TestPattern();
        void TestPatternLoop();

        bool SendCMD(uint8_t addres_driver, enum Command command, uint8_t options);
        uint8_t CRC(uint8_t *buf);
    };
}

#endif
