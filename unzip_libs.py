import zipfile
import os

files= ['mqtt_src/install_cv1800b_cjson.zip', 'mqtt_src/install_cv1800b_openssl.zip', 'mqtt_src/install_cv1800b_mqtt.zip']
for file in files:
    if os.path.exists(file):
        if os.path.exists(f'{os.path.splitext(file)[0]}/'):
            print('folder exists')
        else:
            with zipfile.ZipFile(file) as zip:
                zip.extractall(path='mqtt_src/')
            
    else:
        print(f"{file} not found")

