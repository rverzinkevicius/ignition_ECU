# ignition_ECU

Ignition for scooter with Piaggio LEADER engine using ESP8266

Will use combination of http://www.sportdevices.com/ignition/inverter.htm and https://transmic.net/2016/07/17/dc-cdi-v2/ to get 200-300V from 12V to charge CDI capacitor

Do not know how to use arrays for ignition advance table, so have used below approach to mimic standard ignition curve:

if (rpm<1000) {advance = 10;}

if ((rpm>=1000) && (rpm<1300)) {advance = 17;}

if ((rpm>=1300) && (rpm<1500)) {advance = 30-RPM/100;}

if ((rpm>=1500) && (rpm<3000)) {advance = 15;}

if ((rpm>=3000) && (rpm<4000)) {advance = 0.013*RPM-24;}

if ((rpm>=4000) && (rpm<7000)) {advance = 28;}

if ((rpm>=7000) && (rpm<8000)) {advance = 0.002*RPM+14;}

if (rpm>=8000) {advance = 30;}



![img](https://raw.githubusercontent.com/rverzinkevicius/ignition_ECU/master/advance.png)
