#!/bin/bash

###
# Commandline passed arguments
###

MQTT_server="$1"
MQTT_topic="$2"

###
# The actual script payload
###

function PAYLOAD()
{
  if relay-v5.sh ${1} &>/dev/null
  then
    sudo -u nobody mosquitto_pub -r -m ${3} -t stat/${MQTT_topic}/${2}/POWER -q 1 -h ${MQTT_server}
    sudo -u nobody mosquitto_pub -r -m Offline -t tele/${MQTT_topic}/relays/LWT -q 1 -h ${MQTT_server}
    sleep 30s
  else
    sudo -u nobody mosquitto_pub -r -m Offline -t tele/${MQTT_topic}/relays/LWT -q 1 -h ${MQTT_server}
  
    until sudo -u serial screen -X -S relay quit &>/dev/null
    do
      sudo -u serial screen -d -m -S relay /dev/ttyUSB0 19200 &>/dev/null
      sleep 30s
    done
  fi
}

###
# MAIN
###

while true
do
  sudo -u nobody mosquitto_pub -r -m Online -t tele/${MQTT_topic}/relays/LWT -q 1 -h ${MQTT_server}

  MQTTmsg=$(sudo -u nobody mosquitto_sub -C 2 -t cmnd/${MQTT_topic}/relays/power -q 1 -h ${MQTT_server} \
            --will-payload Offline --will-retain --will-topic tele/${MQTT_topic}/relays/LWT -q 1 -h ${MQTT_server} |
            tail -n 1)

  case ${MQTTmsg} in
    AMP_ON)
      PAYLOAD "AMP ON" amp AMP_ON
      ;;
    AMP_OFF)
      PAYLOAD "AMP OFF" amp AMP_OFF
      ;;
    HDD_ON)
      PAYLOAD "HDD ON" hdd HDD_ON
      ;;
    HDD_OFF)
      PAYLOAD "HDD OFF" hdd HDD_OFF
      ;;
  esac
done
