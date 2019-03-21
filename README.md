Install ESP8266 SDK for arduino...

dependencies:

| Library                   | Link                                                            | Use                 |
|---------------------------|-----------------------------------------------------------------|---------------------|
|arduinoWebSockets          |https://github.com/Links2004/arduinoWebSockets                   |websocket comm impl  |
|PahoMQTT                   |https://projects.eclipse.org/projects/technology.paho/downloads  |mqtt comm impl       |
|ArduinoJSON 6.5.0-beta     |https://github.com/bblanchon/ArduinoJson/releases                |data                 |



Library for connecting to the AWS IoT service from an ESP8266 using websockets. Authenticates using AWS IAM credentials and sigV4.


Install the folloing libraries to arduino to facilitate of integration of AWS iot core.

----------https://github.com/bblanchon/ArduinoJson (use v6.xx)
----------https://github.com/debsahu/esp8266-arduino-aws-iot-ws
----------https://github.com/debsahu/esp8266-arduino-aws-iot-ws
----------https://projects.eclipse.org/projects/technology.paho/downloads (download Arduino version)


Note: the first code with the name ""AWS_Hackathon_arduino_nano_code"" need not reuire any of above libraries - just selct the board as ARDUINO NANO and COM PORT-- you can upload code.
 the second code need to be complied and uploaded by selcting NODE MCU and COM PORT.

