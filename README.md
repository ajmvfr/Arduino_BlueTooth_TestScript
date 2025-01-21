"# Arduino_BlueTooth_TestScript" 

This program repo is based on a 3 video set from youtube
Adding Bluetooth to Your Arduino Project with an HC-05 or HC-06 Bluetooth Module
    https://www.youtube.com/watch?v=NXlyo0goBrU&list=PL2q8_d7EICrQMgeTvHHibytvydzl5a7WR&index=1
        First test with simple echo.cpp

Using AT Commands to Set Up Your Bluetooth Module : HC-05 and HC-06
    https://www.youtube.com/watch?v=GulmSO4Iqk8&list=PL2q8_d7EICrQMgeTvHHibytvydzl5a7WR&index=2
        AppToAllowATCommand.cpp
            this is a little different than the video, I cleaned the code up slightly.

            Note:
            The module I am using is from DSD Tech, and it varried slightly from the video.  In order to get into
            AT command mode, all that needs done is to press the button after the unit is powered on.
            there are no obvious indication that it is in AT mode.

Arduino Bluetooth Control From Your Mobile Phone Using MIT App Inventor
    https://www.youtube.com/watch?v=jGoKdF8r7as&list=PL2q8_d7EICrQMgeTvHHibytvydzl5a7WR&index=3
        bluetooth_reg_testing.cpp
            I made some changes to this program to not use the adafruit array. I also changes this to accept a 0 to 100 percent, then adjust it to 0 to 255.
            mit app:BluetoothTest.aia

Bluetooth_test.cpp
    This is just a cleaned up pbluetooth_reg_testing.cpp script with a couple extra tweaks to experiment further.


"Bluetooth Arduino RECEIVE data + Chart"
    https://www.youtube.com/watch?v=JQ3tDhpmSFE&list=PL2q8_d7EICrQMgeTvHHibytvydzl5a7WR&index=15
        this video was more about reading data from arduino  3 video's

        Send_voltage_data_test.cpp reads from a potentiameter, Mit app:
        Send_Distance_data_test.cpp uses a HC-SR04 distance sensor,MIT app:
        Voltage_Graph.cpp reads from potentiometer scales 0 to 100, amd sends to chart, MIT app: 
