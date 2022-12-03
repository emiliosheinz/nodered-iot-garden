🌱 IoT Garden
==========

## MQTT

- Broker: broker.hivemq.com
- Porta TCP: 1883
- Porta Websocket: 8000

### Tópicos

- iot-garden-parameters
  ```
    "SOIL_HUMIDITY %.2f,AIR_HUMIDITY %.2f,AIR_TEMPERATURE %.2f"
  ```
  
- get-iot-garden-global-config
  ```
    "MIN_HUMIDITY" | "MAX_HUMIDITY"
  ```
  
- iot-garden-global-config
  ```
    "MIN_HUMIDITY %.2f" | "MAX_HUMIDITY %.2f"
  ```
