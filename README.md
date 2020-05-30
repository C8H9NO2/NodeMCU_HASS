# NodeMcuHass
A simple mqtt remote and sensor

ESP8266+MAX44009+BME280

And simple IRLED to remote TCL AirCondition

Use [Adafruit_BME280_Library](https://github.com/adafruit/Adafruit_BME280_Library) [ArduinoJson](https://github.com/bblanchon/ArduinoJson) [MAX44009](https://github.com/dantudose/MAX44009) [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) [pubsubclient](https://github.com/knolleary/pubsubclient)

HASS configuration.yaml add this
```yaml
sensor:
  - platform: mqtt
    name: "Temperature"
    state_topic: "sensor/bedroom"
    unit_of_measurement: '°C'
    value_template: "{{ value_json.Temperature }}"
  - platform: mqtt
    name: "Air pressure"
    icon: mdi:air-filter
    state_topic: "sensor/bedroom"
    unit_of_measurement: 'hPa'
    value_template: "{{ value_json.Press }}"
  - platform: mqtt
    name: "Humidity"
    icon: mdi:water-percent
    state_topic: "sensor/bedroom"
    unit_of_measurement: '%'
    value_template: "{{ value_json.Humidity }}"
  - platform: mqtt
    name: "Light(Lux)"
    icon: mdi:ceiling-light
    state_topic: "sensor/bedroom"
    unit_of_measurement: 'lux'
    value_template: "{{ value_json.Lux }}"
  - platform: mqtt
    name: "Signal"
    icon: mdi:signal-cellular-2
    state_topic: "sensor/bedroom"
    unit_of_measurement: 'dBm'
    value_template: "{{ value_json.RSSI }}"
    
climate:
 - platform: mqtt
   name: AirCondition
   modes:
     - "off"
     - "auto"
     - "cool"
     - "dry"
   fan_modes:
     - "auto"
     - "low"
     - "medium"
     - "high"
   power_command_topic: "ir_server/ac/cmnd/power"
   mode_command_topic: "ir_server/ac/cmnd/mode"
   temperature_command_topic: "ir_server/ac/cmnd/temp"
   fan_mode_command_topic: "ir_server/ac/cmnd/fanspeed"
   fan_mode_state_topic: "ir_server/ac/stat/fanspeed"
   min_temp: 16
   max_temp: 30
   temp_step: 1
   retain: false
```
