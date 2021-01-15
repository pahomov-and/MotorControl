//
// Created by Andrey Pahomov
//

#ifndef CONTROL_H
#define CONTROL_H

#include <thread>
#include <chrono>
#include <zmq.hpp>

#include "SPI/SPI.h"
#include "PT/PT.h"

class Control {
public:
    Control(std::chrono::milliseconds watch_dog_threshold);
    ~Control();

    void Init();
    void Run();
    bool Join();

    bool Executor(std::string cmd);


private:

    SPI _spi;
    unsigned int _NumOfPTDevise;

    Eagle::PT _pt;

    bool isRun;
    std::thread _control;
    std::thread _watch_dog;
    std::chrono::milliseconds _watch_dog_threshold;
    std::chrono::milliseconds _watch_dog_last_tick;

    void _Control();
    void _WatchDog();

    std::chrono::milliseconds GetTime();

};


#endif //MOTORCONTROL_CONTROL_H
