A digital NTP clock and thermomter/hydrometer written in Arduino for the Raspberry Pi Pico using an SH1106 OLED dispaly and a BME280 environment sensor

Pin definitions and display drivers (as long as the display is 128x64) can easily be changed in the code

Theoretical battery life is ~153 hours (6 days 9 hours) with a standard 2000mAh 18650 Li-Ion battery, power draw is around 12.5-13.5 mA

The battery level is read by the battery going into a simple 10k resistor voltage divider into ADC1

The OLED display and BME280 share the same I2C lines into the Pico

Pin definitions and display drivers (as long as the display is 128x64) can easily be changed in the code

28/09/25 WARNING: This code has bugs and holes. It is the first piece of software ever written by me, and since then, I've (unfotunetly) given up programming. 
I am not responsible for anything that happens if you use this neither do I care.
If you want to run this, do so at your own risk and on an isolated IOT network.
