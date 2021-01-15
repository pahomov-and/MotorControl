//
// Created by Andrey Pahomov
//

#include "Configs.h"
#include "Log/LOG.h"
#include <sys/types.h>
#include <sys/stat.h>

std::atomic<Configs*> Configs::instance;
std::mutex Configs::mutexConfigs;

void Configs::Init(std::string dirConfig, std::string fileConfig) {
    struct stat info;

    _dirConfig = dirConfig;
    _fileConfig = _dirConfig + "/" +fileConfig;

    if (stat(_dirConfig.c_str(), &info) == 0 && S_ISDIR(info.st_mode)) {
        LOG_INFO("Check directory: ", _dirConfig);
    } else {
        LOG_INFO("Make directory: ", _dirConfig);
        mkdir(_dirConfig.c_str(), 0700);
    }
}

void Configs::SetConfig(Setup setup) {
    _setup = setup;

}

Setup Configs::GetConfigs() {
    return _setup;
}


void Configs::SetIsStreamingRun(bool onoff) {
    _isStreamingRun = onoff;
}

bool Configs::GetIsStreamingRun() {
    return _isStreamingRun;
}

void Configs::Load() {
    Json::Reader reader;
    Json::Value obj;
    Json::StreamWriterBuilder builder;
    std::ifstream ifs(_fileConfig);

    if (!ifs) {
        _setup.ServerPort = "6600";
        _setup.DevSPI = "/dev/spidev0.0";
        _setup.SpeedSPI = 1100000;
        _setup.NumOfPTDevise = 1;

        Save();
    }

    reader.parse(ifs, root);

    _setup.ServerPort = root["ServerPort"].asString();
    _setup.DevSPI = root["DevSPI"].asString();
    _setup.SpeedSPI = root["SpeedSPI"].asUInt();
    _setup.NumOfPTDevise = root["NumOfPTDevise"].asUInt();

}

void Configs::Load(Json::Value &config) {
    Json::Reader reader;
    Json::Value obj;
    Json::StreamWriterBuilder builder;
    std::ifstream ifs(_fileConfig);

    if (!ifs) {

        std::ofstream ofs(_fileConfig);

        Json::StreamWriterBuilder builder;
        Json::StyledWriter styledWriter;

        ofs << styledWriter.write(config);
        ofs.close();

        const std::string json_file = Json::writeString(builder, root);

        return;
    }

    config.clear();
    reader.parse(ifs, config);
}

void Configs::Save() {

    std::ofstream ofs(_fileConfig);

    Json::StreamWriterBuilder builder;
    Json::StyledWriter styledWriter;

    root["ServerPort"] = _setup.ServerPort;
    root["DevSPI"] = _setup.DevSPI;
    root["SpeedSPI"] = _setup.SpeedSPI;
    root["NumOfPTDevise"] = _setup.NumOfPTDevise;

    ofs << styledWriter.write(root);
    ofs.close();

    const std::string json_file = Json::writeString(builder, root);
}