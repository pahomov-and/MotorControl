//
// Created by Andrey Pahomov
//

#include <map>

#include "Control.h"
#include "Configs/Configs.h"
#include "Log/LOG.h"

constexpr char STATUS_OK[]  = "OK";
constexpr char STATUS_ERROR[]  = "ERROR";

Setup setup;

std::map <std::string, int> MapingCMD = {
        {"LEFT",    Eagle::Command::LEFT},
        {"RIGHT",   Eagle::Command::RIGHT},
        {"UP",      Eagle::Command::UP},
        {"DOWN",    Eagle::Command::DOWN},
};

Control::Control(std::chrono::milliseconds watch_dog_threshold):
    _watch_dog_threshold(watch_dog_threshold),
    _pt(_spi, false) {

    setup = Configs::get()->GetConfigs();
}

Control::~Control() {}

void Control::Init() {
    _spi.Init(setup.DevSPI, SPI::SpiMode::MODE1, 8, setup.SpeedSPI, 0); //11000000
    _NumOfPTDevise = setup.NumOfPTDevise;
}

void Control::Run() {
    _control = std::thread([this] {_Control ();} );
    _watch_dog = std::thread([this] {_WatchDog ();} );
}

bool Control::Join() {
    _control.join();
    return true;
}

bool Control::Executor(std::string cmd) {
    try {
        Json::Value root;
        Json::Reader reader;
        reader.parse(cmd, root);

        std::string _cmd = root["cmd"].asString();
        unsigned int _arg = root["arg"].asUInt();

        switch (MapingCMD[_cmd]) {
            case Eagle::Command::LEFT:
                _pt.SendCMD(_NumOfPTDevise, Eagle::Command::LEFT, _arg);
                _watch_dog_last_tick = GetTime();
                break;
            case Eagle::Command::RIGHT:
                _pt.SendCMD(_NumOfPTDevise, Eagle::Command::RIGHT, _arg);
                _watch_dog_last_tick = GetTime();
                break;
            case Eagle::Command::UP:
                _pt.SendCMD(_NumOfPTDevise, Eagle::Command::UP, _arg);
                _watch_dog_last_tick = GetTime();
                break;
            case Eagle::Command::DOWN:
                _pt.SendCMD(_NumOfPTDevise, Eagle::Command::DOWN, _arg);
                _watch_dog_last_tick = GetTime();
                break;
            default:
                LOG_ERROR("Not exist command");
                return false;
        }

        LOG_INFO("cmd: ", _cmd,  ", arg: ", _arg);
        return true;

    } catch (Json::LogicError json_err) {
        LOG_ERROR(json_err.what());
        return false;
    }
}

void Control::_Control() {
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    std::string url = "tcp://*:" + setup.ServerPort;
    socket.bind(url);

    LOG_WARNING("Motor control is run");

    isRun = true;
    while (isRun) {
        try {

            zmq::message_t request;
            socket.recv(&request);

            std::string str(reinterpret_cast<char *>(request.data()));

            _watch_dog_last_tick = GetTime();

            if (Executor(str)) {
                socket.send(STATUS_OK, sizeof(STATUS_OK));
            } else {
                socket.send(STATUS_ERROR, sizeof(STATUS_ERROR));
            }

        } catch (zmq::error_t err) {
            LOG_ERROR(err.what());
        }
    }
}


void Control::_WatchDog() {
    using namespace std::chrono_literals;

    LOG_WARNING("Watch dog control is run");

    isRun = true;
    while (isRun) {

        if(GetTime() - _watch_dog_last_tick > _watch_dog_threshold) {
            _pt.SendCMD(_NumOfPTDevise, Eagle::Command::LEFT, 0);
            _pt.SendCMD(_NumOfPTDevise, Eagle::Command::DOWN, 0);
            _watch_dog_last_tick = GetTime();
        }

        std::this_thread::sleep_for(10ms);
    }
}

inline std::chrono::milliseconds Control::GetTime() {
    return std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch());
}