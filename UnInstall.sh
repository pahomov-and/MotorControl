#!/bin/bash
SERVICE_NAME=MotorControl
SERVICE_FILE=/etc/systemd/system/$SERVICE_NAME.service


echo $SERVICE_FILE

sudo systemctl stop $SERVICE_NAME
sudo systemctl disable $SERVICE_NAME

sudo rm $SERVICE_FILE