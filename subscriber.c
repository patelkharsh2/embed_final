#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <cjson/cJSON.h>

#include <wiringx.h>
#include "oled_src/ssd1306.h"
#include "bmp_src/bmp280_i2c.h"
#include "morse_src/morse.h"
#include "morse_src/blink.h"

void oled_print()
{
    ssd1306_oled_clear_screen();
    ssd1306_oled_set_XY(0, 0);

}

void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (message->payloadlen)
    {
        //printf("%s %s\n", message->topic, (char *)message->payload);
        cJSON *root = cJSON_Parse(message->payload);
        if (root)
        {
            const cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
            if (cJSON_IsString(name) && (name->valuestring != NULL))
            {
                printf("Name: %s\n", name->valuestring);
            }

            const cJSON *number = cJSON_GetObjectItemCaseSensitive(root, "number");
            if (cJSON_IsNumber(number))
            {
                printf("Number: %d\n", number->valueint);
            }

            const cJSON *string_msg = cJSON_GetObjectItemCaseSensitive(root, "string_msg");
            if (cJSON_IsString(string_msg))
            {
                char buff[50];
                printf("OLED String: %s\n", string_msg->valuestring);
                sprintf(buff, "String:\\n%s", string_msg->valuestring);
                oled_print();
                ssd1306_oled_write_string(0, buff);
            }

            const cJSON *int_msg = cJSON_GetObjectItemCaseSensitive(root, "int_msg");
            if (cJSON_IsString(int_msg))
            {
                char buff[50];
                printf("OLED Int: %d\n", int_msg->valueint);
                sprintf(buff, "Number:\\n%s", int_msg->valuestring);
                oled_print();
                ssd1306_oled_write_string(0, buff);
                
            }

            const cJSON *task = cJSON_GetObjectItemCaseSensitive(root, "task");
            if (cJSON_IsString(task))
            {
                printf("Task: %s\n", task->valuestring);

                if(strcmp(task->valuestring, "get_temperature")==0)
                {
                    char buff[30];
                    struct bmp280_i2c result = read_temp_pressure();

                    sprintf(buff, "Temp: %.2fC / %.2fF", result.temperature, result.temp_f);
                    printf("Temp: %.2fC / %.2fF\n", result.temperature, result.temp_f);

                    oled_print();
                    ssd1306_oled_write_string(0, buff);

                }

                else if(strcmp(task->valuestring, "get_pressure")==0)
                {
                    char buff[30];
                    struct bmp280_i2c result = read_temp_pressure();

                    printf("Pressure: %.3f kPa\n", result.pressure);
                    sprintf(buff, "Pressure: %.3f kPa", result.pressure);

                    oled_print();
                    ssd1306_oled_write_string(0, buff);
                }

                else if(strcmp(task->valuestring, "get_temperature_pressure")==0)
                {
                    char buff[50];
                    struct bmp280_i2c result = read_temp_pressure();

                    printf("Temperature: %.2f°C/%.2f°F Pressure: %.3fkPa\n", result.temperature, result.temp_f, result.pressure);
                    sprintf(buff, "Temp: %.2fC/%.2fF \\nPressure: %.3fkPa", result.temperature, result.temp_f, result.pressure);

                    oled_print();
                    ssd1306_oled_write_string(0, buff);

                }

                else
                {
                    printf("Unknown Task...");
                    ssd1306_oled_clear_screen();
                    ssd1306_oled_set_XY(0, 0);
                }
            }

            // The above are examples of ways strings and numbers can be sent
            // via mqtt. You'll need to add support here for additional tasking.

            // Add an object named "task". (similar to name)
            // You'll need to use strncmp or strcmp to check if the valuestring
            // is "get_temperature" or "get_pressure" or "get_temperature_pressure"
            // Based on that value, you will use your bmp280 library function
            // to get the temperature, pressure or both.
            // Use your ssd1306 library to write the value(s) to the OLED.

            // Add an object named "string_msg". (similiar to name)
            // This is simply a message that can be printed onto the OLED.

            // Add an object named "int_msg". (similiar to number)
            // This is simply a message that can be printed onto the OLED.

            // Bonus: Add a case for "morse". This will use your morse code project
            // to blink the led in morse code.

            cJSON_Delete(root);
        }
        else
        {
            printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        }
    }
    else
    {
        printf("%s (null)\n", message->topic);
    }
}

int main(int argc, char *argv[])
{
    struct mosquitto *mosq;

    // Initialize the Mosquitto library
    mosquitto_lib_init();

    // Intialize WiringX
    if(wiringXSetup("duo", NULL) == -1) {
    wiringXGC();
    }

    //Intialize BMP280
    bmp280_i2c_init();

    //Intialize SSD1306
    ssd1306_init(0);
    ssd1306_oled_default_config(64, 128);
    ssd1306_oled_clear_screen();
    ssd1306_oled_set_XY(0, 0);

    // Create a new Mosquitto runtime instance with a random client ID
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        fprintf(stderr, "Could not create Mosquitto instance\n");
        exit(-1);
    }

    // Assign the message callback
    mosquitto_message_callback_set(mosq, message_callback);

    // Connect to an MQTT brokers

    if (mosquitto_connect(mosq, "104.236.198.67", 1883, 60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Could not connect to broker\n");
        exit(-1);
    }

    // Subscribe to the topic
    //mosquitto_subscribe(mosq, NULL, "test/topic", 0);
    mosquitto_subscribe(mosq, NULL, "cpe4953/spring2024/group2", 0);

    // Start the loop
    mosquitto_loop_start(mosq);

    printf("Press Enter to quit...\n");
    getchar();

    // Cleanup
    mosquitto_loop_stop(mosq, true);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
