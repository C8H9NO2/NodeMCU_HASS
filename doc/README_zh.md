NodeMCU_HASS
=====
## 使用NodeMcu的简易工具
>使用了BME280和MAX44009和红外管实现的简易遥控  
**项目属于个人自用，仅供参考，不保证能否运行**

![NodeMcu](https://github.com/C8H9NO2/NodeMCU_HASS/blob/master/doc/nodemcu.jpg)

### 关于HASS的配置
在HASS的configuration.yaml中添加如下内容
```
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
