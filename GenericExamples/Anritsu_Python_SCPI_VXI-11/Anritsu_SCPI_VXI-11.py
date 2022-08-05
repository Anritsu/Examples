import pyvisa


def main(resource_name, timeout):

    rm = pyvisa.ResourceManager()
    
    # 0. Instrument connection
    anritsu_instrument = rm.open_resource(resource_name)
    anritsu_instrument.timeout = timeout

    # 1. Read Instrument type
    instrument_info = anritsu_instrument.query("*IDN?")
    print(instrument_info)

    # 2. Reset
    anritsu_instrument.write("*RST")
    
    # 3. Connection closing
    anritsu_instrument.close()


if __name__ == "__main__":
    main(resource_name="TCPIP0::127.0.0.1::inst0::INSTR", timeout=20000)
