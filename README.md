# 🌱 IoT Garden

This Node-RED code repository encapsulates the culmination of my Computer Science Degree project—an IoT Garden simulation. Step into the world of community gardening, reimagined through IoT technology. Explore the Node-RED flows and configurations that power this virtual green oasis, showcasing how IoT devices can be harnessed to control, monitor, and optimize a communal garden. Dive into the codebase to gain insights into the intersection of IoT and sustainable agriculture, and discover how technology can transform our connection with the natural world.

## MQTT

- Broker: broker.hivemq.com
- Porta TCP: 1883
- Porta Websocket: 8000

## Topics

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
  
