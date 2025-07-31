import socket
import ssl


class InstrumentSocketConnection:

    def __init__(self, address="TCPIP0::127.0.0.1::9003::SOCKET", timeout=5000):
        self.host_ip = address[address.find("::") + 2: address.find("::", address.find("::") + 1)]
        self.port = int(address[address.find(self.host_ip) + len(self.host_ip) + 2: address.find("::", address.find(self.host_ip) + len(self.host_ip)+1)])
        ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
        ssl_context.check_hostname = False
        ssl_context.verify_mode = ssl.CERT_NONE
        raw_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.ssl_socket = ssl_context.wrap_socket(raw_socket, server_hostname=self.host_ip)
        self.ssl_socket.settimeout(timeout)
        self.connect()

    def connect(self):
        try:
            self.ssl_socket.connect((self.host_ip, self.port))
        except socket.gaierror as e:
            print(f"Connection failed, error message is: {e}")
            print("Exiting code")
            exit()
        except TimeoutError as e:
            print(f"Connection failed, timeout exceeded, message is :{e}")
            print("Exiting code")
            exit()

    def write(self, w_command):
        self.ssl_socket.send((w_command + "\n").encode())

    def query(self, q_command):
        self.ssl_socket.send((q_command + "\n").encode())
        query_response1 = self.ssl_socket.recv(1).decode()
        if query_response1 != "#":
            query_response2 = self.ssl_socket.recv(1024).decode()
            return (query_response1 + query_response2).rstrip()
        else:
            query_response2 = self.return_block_data()
            return query_response1 + query_response2

    def close(self):
        self.ssl_socket.close()

    def return_block_data(self):
        data_block_size_read = False
        data_block_size_characters_read = False
        data_block_size_characters_data = ''
        data_block_size_characters_data_length = 0
        data_block_point_data_read = False
        data_block_point_data = ''

        # first while = reads the first character after "#"
        while not data_block_size_read:
            data_block_size = self.ssl_socket.recv(1).decode()
            data_block_size_l = len(data_block_size)
            if data_block_size.isdigit() and data_block_size_l == 1:
                data_block_size_read = True
                data_block_size_number = int(data_block_size)  # this value used further

        # second while = reads the above determined x number of characters after "#x"
        while not data_block_size_characters_read:
            data_block_size_characters_data += str(self.ssl_socket.recv(data_block_size_number).decode())
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
                self.ssl_socket.close()
                exit()

        # third while = the entire rest of the data block
        while not data_block_point_data_read:
            data_block_point_data_aux = self.ssl_socket.recv(data_block_size_characters).decode()
            data_block_point_data += str(data_block_point_data_aux)
            data_block_point_data_length = len(data_block_point_data_aux)
            if data_block_point_data_length == data_block_size_characters:
                data_block_point_data_read = True
            elif data_block_point_data_length < data_block_size_characters:
                data_block_size_characters -= data_block_point_data_length
            else:
                print("Unexpected if..else result (1), exiting code")
                self.ssl_socket.close()
                exit()
        return data_block_size + data_block_size_characters_data + data_block_point_data

def get_data_from_block(value):
    if len(value) < 2 or value[0:1] != "#":
        raise RuntimeError(f"Unexpected block header: {value!r}")
    header_data_length = int(value[1:2])
    return value[header_data_length + 2:]

def make_block_data(data):
    size_str = str(len(data))
    block = f"#{len(size_str)}{size_str}".encode() + data
    return block

def main(address, timeout):

    # 0. Instrument connection
    anritsu_instrument = InstrumentSocketConnection(address, timeout)

    # 1. Read Instrument type
    instrument_info = anritsu_instrument.query("*IDN?")
    print(instrument_info)

    # 2. Upload a local file to Instrument
    with open("fileToUpload", "rb") as f:
       file_data = f.read()

    block = make_block_data(file_data).decode()
    anritsu_instrument.write(f'MMEMory:DATA "/UserFiles/fileToUpload","Internal",{block}')\

    # 3. Recall file from Instrument and Save to Local file.
    # This assumes you have a file "setup.stp" located "/UserConfigs" of the Internal Storage.
    setup_data = anritsu_instrument.query('MMEMory:DATA? "/UserFiles/fileToDownload","Internal"')

    # Remove the Header information since its not needed for saving file.
    setup_data = get_data_from_block(setup_data)
    with open("fileToSaveLocally", "wb") as f:
        f.write(setup_data.encode())

    # 4. Connection closing
    anritsu_instrument.close()


if __name__ == "__main__":
    main(address="TCPIP0::127.0.0.1::9003::SOCKET", timeout=5000)
