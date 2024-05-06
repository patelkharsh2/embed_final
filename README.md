# Instructions for MQTT Final

## Building the Binary

### Unzip the MQTT libraries
* Use the python script to unzip the three zip folders in `mqtt_src`
    ```bash
    py unzip_libs.py
    ```
### Pull the Cross-Compile Docker Image
```bash
docker pull patelkharsh2/duo-sdk:latest
```

### Compile Libraries
```bash
docker run --rm -v ${PWD}:/app -v ${PWD}/mqtt_src/install_cv1800b_openssl:/app/install_cv1800b_openssl -v ${PWD}/mqtt_src/install_cv1800b_cjson:/app/install_cv1800b_cjson -v ${PWD}/mqtt_src/install_cv1800b_mqtt:/app/install_cv1800b_mqtt patelkharsh2/duo-sdk bash -c "mkdir build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=/app/milkv_duo.cmake .."
```

### Build Binary
```bash
docker run --rm -v ${PWD}:/app -v ${PWD}/mqtt_src/install_cv1800b_openssl:/app/install_cv1800b_openssl -v ${PWD}/mqtt_src/install_cv1800b_cjson:/app/install_cv1800b_cjson -v ${PWD}/mqtt_src/install_cv1800b_mqtt:/app/install_cv1800b_mqtt patelkharsh2/duo-sdk bash -c "cd build && make"
```

## Transfer the Binary to the Duo

1. First connect the Milk-V Duo to your computer using a USB-C Cable. 

2. Then transfer the binary using `scp`
    ```bash
    scp .\build\subscriber root@192.168.42.1:/root/ 
    ```
3. Finally, log into the Duo using SSH
    * *If you have not made your binary an executable, do so through* `chmod +x`
    * Run the binary, `./subscriber.c`


## Local Testing
### Testing with `publisher.c`
* Change the ip parameter of `mosquitto_connect` in the main function *subscriber.c* to "localhost" instead of the ip, "104.236.198.67"
* Change the topic parameter in `mosquitto_subscribe` in main function to "topic/test"

## Challenges
* `get_temperature_pressure` not displaying on OLED
    * String was too long, had to break the line and print on the next one
* Getting the subscriber to work with the grader
    * Had to change the IP and topic
* If we had more time, we would implement the morse code functionality and some custom tasking


TODO:
* Add the updated docker build command to the build.py
* Adding the ability for morse code translation