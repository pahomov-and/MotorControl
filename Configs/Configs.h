//
// Created by Andrey Pahomov
//

#ifndef CONFIGS_H
#define CONFIGS_H

#include <string>
#include <mutex>
#include <memory>
#include <atomic>
#include <vector>
#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

struct Setup {
    std::string ServerPort;     // default 6600
    std::string DevSPI;         // default /dev/spidev0.0
    unsigned int SpeedSPI;      // default 1100000
    unsigned int NumOfPTDevise;  // 0..31 default 1
};

class Configs {

public:
    static Configs* get() {


        Configs* sin = instance.load(std::memory_order_acquire);

        if ( !sin ){
            std::lock_guard<std::mutex> myLock(mutexConfigs);
            sin = instance.load(std::memory_order_relaxed);
            if( !sin ){
                sin = new Configs();
                instance.store(sin, std::memory_order_release);
            }
        }
        return sin;

    }

    void SetConfig(Setup setup);
    Setup GetConfigs();

    void SetIsStreamingRun(bool onoff);
    bool GetIsStreamingRun();

    void Init(std::string dirConfig, std::string fileConfig);
    void Load();
    void Load(Json::Value &config);
    void Save();

private:

    Configs()= default;
    ~Configs()= default;
    Configs(const Configs&)= delete;
    Configs& operator=(const Configs&)= delete;

    static std::atomic<Configs*> instance;
    static std::mutex mutexConfigs;

    Setup _setup;
    bool _isStreamingRun;

    std::string _dirConfig;
    std::string _fileConfig;

    Json::Value root;

};


#endif
