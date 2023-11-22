# Michael Worklog

9/11 - 9/17

First TA meeting 

Project Proposal

Conversation with Machine Shop


9/18 - 9/24

Second TA meeting

Second conversation with Machine Shop

CAD modeling of the Litter Box

Working on Design Document

Revising Proposal

# Shihua Worklog

## 09/11-09/17
- Completed project proposal
- Initial conversation with machine shop

## 09/18-09/24

- Met with TA for mechanical design part and talked with machine shop again
- Finalized design block diagram
- Ordered weight sensor and ESP-C3 dev board for development
- Start learning about ESP-C3 bluetooth connectivity and peripherals that we need, such as SPI and ADC

## 09/25-10/01

- Ordered 12V DC motors and learn about encoders
- Worked on design document
- Purchased ESP32C3 dev kits for prototyping
- Installed and configured ESP-IDF SDK and Eclipse IDE for software dev

## 10/02-10/08

- Completed design review, switching from Bluetooth to Wi-Fi
- Finalized on software structure, made a software block diagram:
![software block diagram](software-block-diagram.jpg "Block diagram")

## 10/09-10/15

- Made a fully functional [web application](https://yumiweidemao.github.io/ece445-web-app/) running on a Github server. This includes UI and MQTT protocol functionalities. It can communicate with the ESP32C3 dev kit over the Internet.
- Completed code skeleton, hardware interaction to be added. (code can be found in [commit history](https://github.com/yumiweidemao/ECE445-Repo/commits/main))

## 10/16-10/22

- Started learning about the HX711 ADC serial interface and how to read weight value based on the sensor values
- Started writing code on weight sensor submodule, modified the read_weight_sensor() function so that it now utilizes GPIO4 and GPIO5 to communicate to HX711 through SCK and DOUT. Testing and calibration still needed in the future.

## 10/23-10/29

- Picked up the completed litter box from machine shop and tested the motor functions in lab using the +12V DC power supply. The two motors are connected in reverse directions by machine shop, so we need to be careful when connecting the motors to H-bridge (the input to one motor should be reversed to the other)
- Confirmed that the motor will likely have enough torque to push the sand, next step is to wait for the PCB, and try to control the motor and read encoder values using the MCU.

## 10/30-11/5

- Nothing particular. Still waiting for the components to come in order to perform further tests.

## 11/6-11/12

- Michael finished the code for odor sensor (ADC module).
