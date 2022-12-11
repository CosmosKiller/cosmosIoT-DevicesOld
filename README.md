# :zap: :bulb: CosmosIoT Devices :satellite: :electric_plug:

> Here you'll find the code for all of the devices manufactured @CosmosIoT 

## Before you start
* It's crucial that before actually start working on any project, you know and apply the conventions regarding code writing style and formatting, to ensure your code is consistent with the codebase of the platform. 

### C-style formatting
* Here, we apply the classic c-style formatting, following the guidelines proposed in the second edition of 'The C Programming Language' and the LLVM style. Which includes, break before braces only when defining functions, alignment of consecutive macros and an indent width of 4 spaces, among others parameters.
* To guarantee that the formatting style, will be the same as the described before, yo can copy the following line in the **Clang_format_fallback Style** option from the C/C++ extension for VS Code:
 ```js
 { BasedOnStyle: LLVM, UseTab: Never, IndentWidth: 4, TabWidth: 4, BreakBeforeBraces: Linux, AllowShortIfStatementsOnASingleLine: false, IndentCaseLabels: false, ColumnLimit: 0, AccessModifierOffset: -4, NamespaceIndentation: All, FixNamespaceComments: false, AlignConsecutiveMacros: true }
 ```

> Press [Ctrl+,] (Linux/Win) or [Command+,] to open VS Code settings and then type *Clang_format_fallback Style* in the browse bar. Then paste the line described above.
>
> You should end up with something similar to the picture below.

<img src='https://i.imgur.com/NP5YZ5A.png'>

### Macros, Variables, Derived data types and Functions naming
* As a general rule, when naming variables/functions we use lowercase and underscore if the name consists of more than one word. E.g.:
```C
int this_is_my_variable;
```
* Only when declaring pointer variables, we use camel casing; the name of the pointer must start with the letter 'p'. E.g.:
```C
const char *pMyPointer;
```
* Same rules that apply for varibles/funtions, also applies when defining new derivade data types (structs/enums). When declaring a new typedef struct, be suere to append '_t' at the end of the name. Similarly, when declaring a typedef enum append '_e' at the end of the name; Also the states must be declared using uppercase and underscore. E.g.:
```C
typedef struct new_struct {
	int my_int;
	unsigned int my_uint;
	char my_char;
	float my_float_array[];
} new_struct_t;

typedef enum new_enum {
	VALUE_0 = 0,
	VALUE_1,
	VALUE_2,
	VALUE_3,
	VALUE_4,
	VALUE_5,
} new_enum_e;

```
* Unlike variables/functions, Macros, must always be declared using uppercase and underscore if the name consists of multiple words. E.G.:
```C
#define OBJECT_AS_MARCO "My Macro"

#define FUNCTION_AS_MACRO(a, b) ((a+b)/(a*b))
```

### Coding style sample
* Below you will see a code sample of all the points discussed above
```C
#ifndef MAIN_COSMOSPUMP_H_
#define MAIN_COSMOSPUMP_H_

#include "driver/adc.h"

#include "cosmos_devices.h"

#define ENGAGE_NO     0
#define ENGAGE_OK     1
#define ENGAGE_RN     2
#define NO_OF_SAMPLES 64
/*
 * Because the GPIO pin is expresed as an int
 * we need to convert it to an adc1_channel_t
 */
#define INT_TO_ADC_CHANNEL(snr_pin) (snr_pin > 35 ? snr_pin - 36 : snr_pin - 28)

/**
 * @brief Use this scruct within the
 * cosmos_pump_t to store the parameters
 * of the sensors
 *
 */
typedef struct cosmos_pump_snr_info {
    /* Threshold Level*/
    const int thr_lvl;
    /* pin_num in which the sensor is connected */
    const int pin_num;
} cosmos_pump_snr_info_t;

/**
 * @brief Use this struct to define all the parameters
 * that needs to be checked in order to get the pump
 * going
 *
 * @note If you are going to do without any of the sensors,
 * just create an arry of zeros -> {0, 0}
 */
typedef struct cosmos_pump {
    /* Pump relay serial number, pins and state */
    cosmos_devices_t *pPumpInfo;
    /* Pins in which the pump's LED is connected */
    const int *pLedInfo;
    /* Water level sensor */
    cosmos_pump_snr_info_t snr_wl;
    /* Temperature sensor */
    cosmos_pump_snr_info_t snr_th;
    /* Soil moisture sensor */
    cosmos_pump_snr_info_t snr_sm;
} cosmos_pump_t;

/**
 * @brief Struct in which the sensed data will be stored
 */
typedef struct cosmos_snr_data {
    int wl_data;
    int sm_data;
    float tm_data;
    float hm_data;
} cosmos_snr_data_t;

/**
 * @brief Call this function whenever you want to check the
 * status of the pump
 *
 * @param pPump Pointer to the struct that defines the pump
 * @param engage If you want to turn on the pump ENGAGE_OK ;
 * If you want to leave it in idle ENGAGE_NO ;
 * If pump is already running ENGAGE_RN
 * @param pSensorData Pointer to the struct in which the sensed data will be stored
 */
void cosmos_pump_control(cosmos_pump_t *pPump, int engage, cosmos_snr_data_t *pSensorData);

#endif /* MAIN_COSMOSPUMP_H_ */
```
---

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
* Be sure to be as explicit and illustrative as possible when naming your project/folder ; e.g., 'automatedIrrigationSystem'
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
lib_ldf_mode = chain+
build_flags = -D CIOT_ESP8266
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