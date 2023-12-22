# OliverStanton-WaterMonitor-Firmware


lcd.h                       //or OLED.h   will hold function/methods for operating OLED screen
switches.hpp                //handles interrupt calls and button pushes
motion.hpp                  //dont know what it does yet
LED.h                       //handles WLED?
menu.h                      //generates and navigates menus
menuList.h                  //contains all the structure of menus
motorDriver.h               //drives pump motors
flash.h                     //handles data fetching and pushing into flash memory
callbacks.h                 //have to remove?
Display.h                   //handles output displaying on OLED/Serial
CDC_Serial.h                //uses Serial for displaying
flagHandler.h               //handles input/output status/settings flags
controls.h                  //functions/methods are declared here and defined in controls.cpp (is not a class)