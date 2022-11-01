# :zap: :bulb: CosmosIoT Devices :satellite: :electric_plug:

> Here you'll find the code for all of the devices manufactured @CosmosIoT 

## Getting started
* First of all, you'll need to install the PlatformIO Core (CLI) :ant:
* On a terminal you can simply type `pip install platformio` to get the client globally installed
* Now you are able to use the `pio` command. For more information, type `pio -h` 
### Working on a existent project
* Navigate to the project's folder you want to work on and find the *src* folder inside of it
* Open the *main.cpp* inside the *src* folder
* Start coding! :computer:
* If you want to build/test/upload you code, type `pio run -e [theNameOfTheProjectYouAreWorkingOn]`. For more information, type `pio run -h`
### Creating a new project
* First you'll need to create the project's folder where the *main.cpp* and the *platformio.ini* files will live
* Be sure to be as explicit and illustrative as possible when naming your project/folder ; e.g.,
* Once you've created the project directory, you'll need to create the *platformio.ini* file. This file, is used as project's configuration file that PlatformIO will use to build/upload/test your source code.
* This file **must** include the following section (denoted by the `[header]` 'platformio')
```ini
[platformio]
libdeps_dir  = ../.commonFiles/.pio/libdeps
build_dir  = ../.commonFiles/.pio/build
lib_dir  = ../.commonFiles/lib
include_dir  = ../.commonFiles/include
```
* This header will make a 'global directory' (named '.commonFiles') where all you libdeps, custom libs and include files will be stored, so you use them in all of your projects without the need of downloading them over and over.
* You can configure the rest of the sections according to what the project needs. Here's an example for a project based of a NodeMCU ESP8266 board, which includes a library for servo motors control.
```ini
[env:autonomusPetFeeder]
platform  = espressif8266
board  = nodemcuv2
framework  = arduino
monitor_speed  = 115200
lib_deps  =
	khoih-prog/ESP8266_ISR_Servo@^1.3.0
```
* Next step, is to create the *src* folder and the *main.cpp* file inside of it.
* Finally is time to get your hands dirty and start coding! :keyboard:
> Below you can see a typical project structure for the Arduino framework
```sequence
global dir
|--.commonFiles
|  |--.pio
|  |  |--build
|  |  |--libdeps
|  |
|  |--include
|  |--lib
|	
|--project1
|  |--src
|  |  |--main.cpp
|  |	
|  |--platfomio.ini
|
|--project2
|  |--src
|  |  |--main.cpp
|  |
|  |--platfomio.ini
```
## Copyright © 2022 Marcel Nahir Samur