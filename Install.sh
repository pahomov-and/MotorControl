#!/bin/bash
SERVICE_NAME=MotorControl
SERVICE_FILE=/etc/systemd/system/$SERVICE_NAME.service

Install() {
    sudo cp $SERVICE_NAME /root/

    echo $SERVICE_FILE

    echo "[Unit]
Description=$SERVICE_NAME
After=network.target
StartLimitIntervalSec=0
[Service]
Type=simple
Restart=always
RestartSec=1
ExecStart=/usr/bin/env /root/$SERVICE_NAME

[Install]
WantedBy=multi-user.target
" > $SERVICE_NAME.service

    sudo cp $SERVICE_NAME.service $SERVICE_FILE

    sudo systemctl start $SERVICE_NAME
    sudo systemctl enable $SERVICE_NAME

}


if  rm -rf build ; mkdir build && cd build && cmake .. && make ; then
    Install
fi