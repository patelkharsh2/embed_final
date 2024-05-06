cmake_minimum_required(VERSION 3.10)
project(final_project)

# Find cJSON library
find_library(CJSON_LIB
  NAMES cjson
  PATHS "/app/install_cv1800b_cjson/lib"
  NO_DEFAULT_PATH
)

# Find Mosquitto library
find_library(MQTT_LIB
  NAMES mosquitto
  PATHS "/app/install_cv1800b_mqtt/lib"
  NO_DEFAULT_PATH
)

# Find SSL library
find_library(OPEN_SSL_LIB
  NAMES ssl
  PATHS "/app/install_cv1800b_openssl/lib"
  NO_DEFAULT_PATH
)

# Find CRYPTO library
find_library(CRYPTO_LIB
  NAMES crypto
  PATHS "/app/install_cv1800b_openssl/lib"
  NO_DEFAULT_PATH
)

# Check if all libraries were found
if(NOT CJSON_LIB OR NOT MQTT_LIB OR NOT OPEN_SSL_LIB OR NOT CRYPTO_LIB)
  message(FATAL_ERROR "One or more libraries (cJSON, Mosquitto, SSL) not found")
endif()

# Set variables for the OLED and BMP libraries
file(GLOB OLED_SOURCES_C "oled_src/*.c")
file(GLOB BMP_SOURCES_C "bmp_src/*.c")
file(GLOB MORSE_SOURCES_C "morse_src/*.c")

# Add a static library for the libraries
add_library(ssd1306 STATIC ${OLED_SOURCES_C})
add_library(bmp280 STATIC ${BMP_SOURCES_C})
add_library(morse STATIC ${MORSE_SOURCES_C})

# Add executables
add_executable(subscriber subscriber.c)
add_executable(publisher publisher.c)

# Link libraries
target_link_libraries(subscriber ${CJSON_LIB} ${MQTT_LIB} ${OPEN_SSL_LIB} ${CRYPTO_LIB} ssd1306 bmp280)
target_link_libraries(publisher ${CJSON_LIB} ${MQTT_LIB} ${OPEN_SSL_LIB} ${CRYPTO_LIB})
