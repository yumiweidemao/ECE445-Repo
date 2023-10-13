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
