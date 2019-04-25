# Pi-Hole_DisableBox
Disable a Pi-Hole via wireless using an ESP8266 NodeMCU (12F) using API

Arduino ESP8266 NodeMCU Pi-Hole Disable Code

This sketch for Arduino IDE configures an ESP8266 to disable the Pi-Hole via API for a specified period of time. It uses four buttons (1) 1 minute, (2) 5 Minutes, (3) 10 Minutes, (4) for Enable. There is an RGB LED for visual BLUE = configuring, RED = Disabled, GREEN = Enabled.

NOTE: I'm still working on setting up the delay and ability to press Enable to cancel a disable request

You need to provide your: SSID, PASSWORD, and IP to PI-HOLE and API_Key where asked.
