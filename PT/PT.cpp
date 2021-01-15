//
// Created by Andrey Pahomov
//

#include "PT.h"
#include <thread>
#include <chrono>

using namespace Eagle;

PT::PT(SPI &spi, bool debug) : _debug(debug), _spi(spi) {}

    void PT::TestPattern() {
        using namespace std::chrono_literals;
        uint8_t pattern[2][5] = {
                {255, 1, 3, 240, 116},
                {255, 1, 3, 0, 4},
        };

        for(int i = 0; i < 2; i++) {
            _spi.transferArray(&pattern[i][0], 5);
            std::this_thread::sleep_for(1s);
        }
    }


    void PT::TestPatternLoop() {
        uint8_t inc = 0;
        while (1) {
            using namespace std::chrono_literals;
            uint8_t pattern[2][5] = {
                    {255, inc, inc, inc, inc},
            };

            _spi.transferArray(&pattern[0][0], 5);
            std::this_thread::sleep_for(1s);

            inc++;
        }
    }


    bool PT::SendCMD(uint8_t addres_driver, enum Command command, uint8_t options) {
        uint8_t buf[PACKAGE_SIZE] = {0xFF, 0, 0, 0, 0};

        buf[PosByte::START] = 0xFF;
        buf[PosByte::ADRESS] = addres_driver;
        buf[PosByte::COMAND] = command;
        buf[PosByte::OPTION] = options;
        buf[PosByte::CHECK_SUM] = CRC(buf);

        /*
        for (int i = 0; i < PACKAGE_SIZE; i++) {
            printf("%d ", buf[i]);
        }
        printf("\n");
         */

        return _spi.transferArray(buf, PACKAGE_SIZE);
    }

    uint8_t PT::CRC(uint8_t *buf) {
        uint8_t ret = 0;

        if (buf != NULL) {
            for (uint32_t i = 1; i < PACKAGE_SIZE - 1; i++) {
                ret+=buf[i];
            }
            return ret % 128;
        }

        return 0;
    }
