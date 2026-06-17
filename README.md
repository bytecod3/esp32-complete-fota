## esp32-complete-fota
End-to-end over the air firmware update leveraging secondary storage for new firmware updates

### ESP-IDF build steps 
The following steps are built on ubuntu-linux:

#### Ubuntu-Linux
##### activate venv
```c source ~/.espressif/tools/activate_idf_v6.0.1.sh```

##### cd into target project root
```c cd ~/workspace/project_name```

##### set target 
```c idf.py set-target esp32```

##### to config - optional 
```c idf.py menuconfig```

##### build 
```c idf.py build```

#### flash 
##### view serial port number (linux)
```c ls /dev/ttyUSB*```

##### flash 
```c idf.py -p /dev/ttyUSB0 flash ```

##### monitor serial 
```c idf.py monitor```

#### Creating components

