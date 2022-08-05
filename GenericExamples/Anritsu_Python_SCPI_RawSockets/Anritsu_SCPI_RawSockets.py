import socket
# main() is defined at line 96
# main() is called at line 112


class InstrumentSocketConnection:

    def __init__(self, address="TCPIP0::127.0.0.1::5001::SOCKET", timeout=5000):
        self.host_ip = address[address.find("::") + 2: address.find("::", address.find("::") + 1)]
        self.port = int(address[address.find(self.host_ip) + len(self.host_ip) + 2: address.find("::", address.find(self.host_ip) + len(self.host_ip)+1)])
        self.shockline_socket = socket.socket()
        self.shockline_socket.settimeout(timeout)
        self.connect()

    def connect(self):
        try:
            self.shockline_socket.connect((self.host_ip, self.port))
        except socket.gaierror as e:
            print(f"Connection failed, error message is: {e}")
            print("Exiting code")
            exit()
        except TimeoutError as e:
            print(f"Connection failed, timeout exceeded, message is :{e}")
            print("Exiting code")
            exit()

    def write(self, w_command):
        self.shockline_socket.send((w_command + "\n").encode())

    def query(self, q_command):
        self.shockline_socket.send((q_command + "\n").encode())
        query_response1 = self.shockline_socket.recv(1).decode()
        if query_response1 != "#":
            query_response2 = self.shockline_socket.recv(1024).decode()
            return (query_response1 + query_response2).rstrip()
        else:
            query_response2 = self.return_block_data()
            return (query_response1 + query_response2).rstrip()

    def close(self):
        self.shockline_socket.close()

    def return_block_data(self):
        data_block_size_read = False
        data_block_size_characters_read = False
        data_block_size_characters_data = ''
        data_block_size_characters_data_length = 0
        data_block_point_data_read = False
        data_block_point_data = ''

        # first while = reads the first character after "#"
        while not data_block_size_read:
            data_block_size = self.shockline_socket.recv(1).decode()
            data_block_size_l = len(data_block_size)
            if data_block_size.isdigit() and data_block_size_l == 1:
                data_block_size_read = True
                data_block_size_number = int(data_block_size)  # this value used further

        # second while = reads the above determined x number of characters after "#x"
        while not data_block_size_characters_read:
            data_block_size_characters_data += str(self.shockline_socket.recv(data_block_size_number).decode())
            data_block_size_characters_data_length += len(data_block_size_characters_data)
            if data_block_size_characters_data_length == data_block_size_number and data_block_size_characters_data.isdigit():
                data_block_size_characters_read = True
                data_block_size_characters = int(data_block_size_characters_data)  # this value used further
            elif data_block_size_characters_data_length < data_block_size_number:
                data_block_size_number -= data_block_size_characters_data_length
                data_block_size_characters_data_length = 0
            elif data_block_size_characters_data_length > data_block_size_number:
                data_block_size_characters_data_length = 0
                data_block_size_characters_data = ''
            else:
                print("Unexpected if..else result (1), exiting code")
                self.shockline_socket.close()
                exit()

        # third while = the entire rest of the data block
        while not data_block_point_data_read:
            data_block_point_data_aux = self.shockline_socket.recv(data_block_size_characters).decode()
            data_block_point_data += str(data_block_point_data_aux)
            data_block_point_data_length = len(data_block_point_data_aux)
            if data_block_point_data_length == data_block_size_characters:
                data_block_point_data_read = True
            elif data_block_point_data_length < data_block_size_characters:
                data_block_size_characters -= data_block_point_data_length
            else:
                print("Unexpected if..else result (1), exiting code")
                self.shockline_socket.close()
                exit()
        # last bit (newline)
        last_bit = self.shockline_socket.recv(1).decode()
        data_block_point_data += str(last_bit)
        return data_block_size + data_block_size_characters_data + data_block_point_data


def main(address, timeout):

    # 0. Instrument connection
    anritsu_instrument = InstrumentSocketConnection(address, timeout)

    # 1. Read Instrument type
    instrument_info = anritsu_instrument.query("*IDN?")
    print(instrument_info)
    
    # 2. System Preset
    anritsu_instrument.write("*RST")

    # 3. Connection closing
    anritsu_instrument.close()


if __name__ == "__main__":
    main(address="TCPIP0::127.0.0.1::5001::SOCKET", timeout=5000)
