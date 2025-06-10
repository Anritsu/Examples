"""
Example code for EcoSyn Lite QSYN commands over SPI. Requires Python 3.11 or later.

Prerequisites:
Install NI 845x drivers from:
https://www.ni.com/en-in/support/downloads/drivers/download.ni-845x-driver-software.html#346270

Download and install the NI8452 package from:
https://github.com/sabari-saravanan-m/NI8452
"""

from NI8452 import spi
from time import sleep


# Initialize NI8452 SPI Driver
spiDevice = spi.SPI()
spiDevice.ni845xOpen()
spiDevice.ni845xSetIoVoltageLevel(33)
spiDevice.ni845xSpiConfigurationOpen()
spiDevice.ni845xSpiConfigurationSetChipSelect(0)
spiDevice.ni845xSpiConfigurationSetClockPhase(0)
spiDevice.ni845xSpiConfigurationSetClockPolarity(0)
spiDevice.ni845xSpiConfigurationSetClockRate(12500) # Supports up to 12.5 MHz
spiDevice.ni845xSpiConfigurationSetNumBitsPerSample(8)

# Set the language to QSYN
cmd = bytes(f'SYST:LANG QSYN\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.002) # Delay 2ms to switch the language

# Query the identification info
cmd = [0x01] + [0x0] * 11
response = spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))[0]

model = response[1] # Max Freq GHz
revision = chr(ord('A') + (response[2] - 1))
options = 'None' # Not yet supported

if (response[5] >= 25 and response[5] <= 99): # V20YY.MM.RR
    version = f'V20{response[5]}.{response[6] >> 4}.{response[6] & 0xf}'
else: # Build Number
    version = int.from_bytes(response[5:7], "big")

serial = ''.join(list(map(lambda x : f'{x:02}', response[7:]))) # Each nibble = 1 digit

print(f'Model: {model}{revision}, Options: {options}, Version: {version}, Serial: {serial}')

# Reset to start at a known state
cmd = [0x27, 0x0]
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.5)

# Set CW frequency to 5 GHz
cmd = [0x0C] + list(int.to_bytes(int(5e9 * 1000), 6, "big", signed=False))
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))

# Enable RF output
cmd = [0x0F, 0x01]
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))

# Query the CW frequency
cmd = [0x04] + [0x0] * 6
response = spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))[0]
print(f'CW Frequency: {int.from_bytes(response[1:], "big") / 1000.0} Hz')

# Set the language back to SCPI
cmd = bytes(f'SYST:LANG SCPI\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))

