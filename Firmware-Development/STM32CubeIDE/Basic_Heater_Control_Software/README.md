# Water Monitor Firmware Flashing Guide
 Guide for flashing firmware onto Water Monitor Board and verifying operation.

## Required Software
 You will need to create an account on stm32 website inorder to download STM32CubeProgrammer. 
 [STM32 Cube Programmer](https://www.st.com/en/development-tools/stm32cubeprog.html)  
 Any serial monitor software. 
 [CoolTerm](https://coolterm.en.lo4d.com/windows) 


## Firmware Link
[Firmware Link](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/tree/6-llrofs-purification/Firmware-Development/STM32CubeIDE/Basic_Heater_Control_Software/Feature_Basic_Heater_Settings/Binaries)

## Uploading/Testing provided firmware

1. Download Firmware Binaries.

### Uploading Firmware
1. Use a jumper to short pins 1 and 2 on "BOOT0 SWITCH" header.
![Screenshot_9](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/03bcb6e1-4401-4dce-b076-14c71e92acda)
2. Open STM32CubeProgrammer and select "USB" option in the drop down menu on right pane.  
![Screenshot_1](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/3ac79ac3-ee2b-42e2-8206-c9a0180288e6)
3. Connect Water Monitor Board to your computer via USB cable.
4. Press the refresh button on STM32CubeProgrammer and press connect when your device is found.
![Screenshot_2](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/625769ce-154b-484c-bb89-9c25ad5648ff)
![Screenshot_3](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/da447865-bfa7-4cda-b484-03008872d292)
5. Press open file button and navigate to the folder containing firmware and select it.
![Screenshot_4](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/32d5bbc0-a5f4-4386-bfc5-691962077f8b)
6. Once file opens up, press download button to start flashing the firmware and wait till "the download complete" dialog to appear.
![Screenshot_5](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/a6f58a47-3dfb-4b88-90e0-5f56846d2883)
![Screenshot_6](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/cf163fe6-1798-4c7f-a294-adab78a5562b)
7. Remove the jumper on "BOOT0 SWITCH" header and press reset button on Water Monitor Board.
8. Open device manager and find out the COM port of your Water Monitor.
![Screenshot_7](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/2fb4de22-8d26-42ef-bc41-2093ad95b134)
9. Open up serial monitor and select the COM port shown in your device manager and press connect.
![Screenshot_8](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/9c77eca8-4e24-44e0-92cf-68d8701ba10f)
10. Navigate through the menu to verify operation.
11. Connect 12V Power source the green header with label "BATTERY" on the board.

 Button functions are
![Screenshot_10](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/05debff8-a7b3-4b66-874a-6e8e8899add5)

## Water Purification Sequence is as follows

![image (1)](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/8c9a4b34-b2fd-4cdf-b677-c2e790f6a153)
![image](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/d8d37932-b1a2-442c-b26a-f58fe83714bd)
![image (0)](https://github.com/eSpark-Consultants/OliverStanton-WaterMonitor-Firmware/assets/152279596/3371bf60-1f48-4283-9013-69ec9bd62d76)


1. You can change the Fill on Demand setting. 
  Select "ON" to fill water tank automatically. 
  Select "OFF" to fill water tank manually. 
2. You can change the Mem Saver Setting.
  Select "ON" to activate setting. 
  Select "OFF" to deactivate setting. 
3. You can change the threshhold value for TDS.

