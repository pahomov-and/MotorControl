#include <iostream>
#include <chrono>
#include <csignal>

#include "Configs/Configs.h"
#include "Control.h"

using namespace std::chrono_literals;


void intHandler(int dummy) {
    exit(0);
}

int main() {

    std::string configs_file("configs.json");
//    std::string home_dir(getenv("HOME"));
    std::string home_dir("/etc");
    home_dir += "/MotorControl";

    signal(SIGINT, intHandler);

    Configs::get()->Init(home_dir,  configs_file);
    Configs::get()->Load();

    Control control(2s);
    control.Init();
    control.Run();
    control.Join();

    return 0;
}