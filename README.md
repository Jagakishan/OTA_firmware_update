# MQTT & HTTP - Based OTA Update System

## Overview

This repository contains a **MQTT-triggered HTTP-based OTA (Over-The-Air) update mechanism** developed for the previously created **Custom Bootloader Project**.

Important:

* This OTA implementation is developed **separately**.
* It is **NOT integrated** into the original custom bootloader repository.
* This project demonstrates the update trigger and version management logic only.

The purpose of this repository is to provide a clean proof-of-concept for:

* Firmware version checking
* MQTT-based update trigger mechanism(Uses **hivemq broker**)
* HTTP-based firmware update

---

## Architecture

This implementation follows a **request–response MQTT design pattern**.

### Files involved

1. **ESP (Client in both cases)**

   * Subscribes to:
     `device/<device_id>/update/response`
   * Publishes to:
     `device/<device_id>/update/check`

2. **MQTT Server (Update Availabilty Publisher)**

   * Subscribes to:
     `device/<device_id>/update/check`
   * Publishes to:
     `device/<device_id>/update/response`

3. **HTTP Server** **(Actual Firmware Sender)**
   
   * Responds with meta data about the new firmware and after right acknowledgements, sends the actual payload which is the firmware to the Wi-Fi coworker.
   * Then UART based transfer to STM32 for flash erase and write.

---

## What's been done

This project includes:

* MQTT connection setup
* Topic-based update check system
* Version comparison logic
* JSON-based update messaging
* HTTP based request and response
* Firmware meta data transfer
* Firmware binary transfer
* HTTPS download logic
* UART firmware transfer between ESP01S and STM32 
* Flash writing implementation
  
---

## Relation to Custom Bootloader Project

This OTA system is designed to work with the previously developed Custom Bootloader project.

However:

* The bootloader repository remains unchanged.
* No direct integration has been done yet.
* This project acts as a standalone OTA trigger module.

Future work may involve:

* Integrating MQTT trigger directly into bootloader flow
* Implementing firmware hash validation

---

## Future Improvements-If anyone forks this repo, you can implement.....

* TLS-secured MQTT connection
* Retained message usage(I have not retained messages sent to the broker)
* Device authentication
* Scalable multi-device support
* Cloud integration (AWS IoT / Azure IoT)

---

## Author

Jagakishan S.K
