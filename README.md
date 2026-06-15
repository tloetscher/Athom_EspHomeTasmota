# Athom_EspHomeTasmota
Minimal Tasmota API Extension for EspHome. This way the Athom Plug does not need to be flashed with Tasmota and can still be integrated into e.g. Solarmanager. -> Warning super experimental, use on your own risk!!

# Hardware:
I got a Athom Smart Plug V3 (CH Edition) from https://www.swiss-domotique.ch/en/wall-plugs/esphome-wifi-plug-in-swiss-format
This version runs with EspHome.

# What does this extension / modification do? 
This exension is a minimal REST API implementation of the Tasmota API. 
With this extension running on the Plug itself, it can be added into Systems which just accept Tasmota. (e.g. Solarmanager)

# How to run and build
To build and upload the firmware to the Plug following Steps are required:
1. Setup the Plug normaly using the inital procedure of EspHome and add it to your Wifi
2. Check in you browser if you can reach the plug by its ip, you should get a status page.
3. Install the EspHome cli
4. Clone this repo and navigate to it and run "esphome run athom-smart-plug.yaml --device <IP of Plug>"
5. It should build the firmware and upload it to the plug.
6. When its working, try to add it in the solarmanger using the "Tasmota Flashed" Device
7. Wait for a bit and it should find it and start reading the power measurements.


