"""
Example code for EcoSyn Lite SCPI commands over SPI. Requires Python 3.11 or later.

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

# Set the language to SCPI
cmd = bytes(f'SYST:LANG SCPI\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.002) # Delay 2ms between commands

# Query *IDN? by transferring the response one byte at a time
cmd = bytes(f'*IDN?\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.002) # Delay 2ms before reading query result
cmd = [ord('\n')]
response = spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))[0]
idnResponse = []
while response != [ord('\n')]:
    idnResponse += response
    response = spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))[0]

print(f'*IDN? Response: {bytes(idnResponse).decode("ascii", errors="ignore")}')

# As a faster alternative, we can transfer more bytes than required and
# trim off the excess line feed characters.
cmd = bytes(f'*IDN?\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.002) # Delay 2ms before reading query result
cmd = [ord('\n')] * 128
response = spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))[0]
response = bytes(response).decode("ascii", errors="ignore")
response = response.split('\n', maxsplit=1)[0]
print(f'*IDN? Response: {response}')

# Reset to start at a known state
cmd = bytes(f'*RST\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.02) # Delay 20ms for reset

# Set up a list sweep from 4.5 Ghz to 5.5 GHz with 50 points
cmd = bytes(f'FREQ:MODE LIST1\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.002) # Delay 2ms between commands

step = (5.5e9 - 4.5e9) / (50 - 1)
freqs = ','.join([str(4.5e9 + x * step) for x in range(50)])
cmd = bytes(f'LIST1:FREQ {freqs}\n', 'ascii')

# For long commands, we need to send them in chunks of 256 bytes
for i in range(0, len(cmd), 256):
    sub_cmd = cmd[i:i + 256]
    spiDevice.ni845xSpiWriteRead(len(sub_cmd), sub_cmd, len(sub_cmd))
    sleep(0.002) # Delay 2ms between commands

cmd = bytes(f'LIST1:STAR 0\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.002) # Delay 2ms between commands

cmd = bytes(f'LIST1:STOP 49\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.002) # Delay 2ms between commands

cmd = bytes(f'LIST1:DWEL 500ms\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.002) # Delay 2ms between commands

cmd = bytes(f'OUTP 1\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.002) # Delay 2ms between commands

cmd = bytes(f'*TRG\n', 'ascii')
spiDevice.ni845xSpiWriteRead(len(cmd), cmd, len(cmd))
sleep(0.002) # Delay 2ms between commands
