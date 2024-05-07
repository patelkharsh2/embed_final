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
    scp build/subscriber root@192.168.42.1:/root/ 
    ```
3. Finally, log into the Duo using SSH
    * *If you have not made your binary an executable, do so through* `chmod +x`
    * Run the binary, `./subscriber`

4. Once it's running, you should see the OLED display turn on and cycle through different messages
  <img src="/hardware/result.jpg" alt="drawing" width="400"/>

## Local Testing

### Testing with `publisher`
* Change the ip parameter of `mosquitto_connect` in the main function *subscriber.c* to `localhost` instead of the ip, `104.236.198.67`
* Change the topic parameter in `mosquitto_subscribe` in main function to `topic/test`
* Cmake file will compile both `publisher` and `subscriber`. Copy the publisher over with 
    ```bash
    scp build/publisher root@192.168.42.1:/root/
    ```
### Test with `publisher_instructor`
* This does local testing but with the topics
    * Change the ip parameter of `mosquitto_connect` in the main function *subscriber.c* to `localhost` instead of the ip, `104.236.198.67`
    * Change the topic parameter in `mosquitto_subscribe` in main function to `cpe4953/spring2024/group2`

### Running it Locally
* Run the broker 
    ```bash
    mosquitto -c /etc/mosquitto/mosquitto.conf -v
    ```
* Run the subscriber
    ```bash
    ./subscriber
    ```
* Run the publisher
    ```bash
    ./publisher
    ```
    or
    ```bash
    ./publisher_instructor
    ```

## Wiring Diagram

### Components
* RJ45 Expansion Module
    <br><img src="/hardware/rj45.jpg" alt="drawing" width="200"/>
* Barmometric Pressure Sensor (BMP280)
    <br><img src="/hardware/bmp280.jpg" alt="drawing" width="200"/>
* OLED Display (SSD1306)
    <br><img src="/hardware/ssd1306.jpg" alt="drawing" width="200"/>
* Milk-V Duo
    <br><img src="/hardware/duo-v1.2.png" alt="drawing" width="200"/>

### Connection Diagram
* Duo Pinout
![pinout](/hardware/duo-pinout.png)

* Diagram
    * **Power:** Pin 36
    * **Ground:** Pin 38
    * **SSD1306**
        * SCL: Pin 1
        * SDA: Pin 2
    * **BMP280**
        * SCL: Pin 15
        * SDA: Pin 14
<br><img src="/hardware/setup.jpg" alt="drawing" width="500"/><br>




## Challenges
* `get_temperature_pressure` not displaying on OLED
    * String was too long, had to break the line and print on the next one
* Getting the subscriber to work with the grader
    * Had to change the IP and topic
* If we had more time, we would implement the morse code functionality and some custom tasking


TODO:
* Add the updated docker build command to the build.py
* Adding the ability for morse code translation