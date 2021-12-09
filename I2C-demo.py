#!/usr/bin/env python3
from ev3dev2.sensor import INPUT_4
from ev3dev2.port import LegoPort
from time import sleep
from smbus import SMBus

lego_port = LegoPort(INPUT_4)
lego_port.mode = 'other-i2c'
sleep(0.5)

# Settings for I2C (SMBus(4) for INPUT_2)
lego_bus = SMBus(6)
address = 0x04

lego_bus.write_byte(address, 12) #address, value
sleep(0.5)
