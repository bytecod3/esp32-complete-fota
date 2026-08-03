## esp32-complete-fota
End-to-end over the air firmware update leveraging secondary storage for new firmware updates

### ESP-IDF build steps 
The following steps are built on ubuntu-linux:

#### Ubuntu-Linux
##### activate venv
```source ~/.espressif/tools/activate_idf_v6.0.1.sh```

##### cd into target project root
```cd ~/workspace/project_name```

##### set target 
```idf.py set-target esp32```

##### to config - optional 
```idf.py menuconfig```

##### build 
```idf.py build```

#### flash 
##### view serial port number (linux)
```ls /dev/ttyUSB*```

##### flash 
```idf.py -p /dev/ttyUSB0 flash ```

##### monitor serial 
```idf.py monitor```

#### Creating components
TO create a components folder on root and add a component
```idf.py create-component -C components component_name```


## Adding cJSON lib
```idf.py add-dependency "espressif/cjson^1.7.19~2" ```
