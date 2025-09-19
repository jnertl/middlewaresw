import socket
import time
import struct
import engine_data_pb2

HOST = '127.0.0.1'  # or use the server's IP address
PORT = 5555

def recv_n(sock, n):
    data = b''
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return None
        data += packet
    return data

def run_client(host=HOST, port=PORT, sleep_func=time.sleep, print_func=print):
    while True:
        try:
            with socket.create_connection((host, port)) as s:
                while True:
                    s.sendall(b'get\n')
                    # First, receive 4 bytes for the size
                    size_bytes = recv_n(s, 4)
                    if not size_bytes:
                        print_func("Server closed connection.")
                        break
                    msg_len = struct.unpack('!I', size_bytes)[0]
                    # Now receive the protobuf message
                    msg_bytes = recv_n(s, msg_len)
                    if not msg_bytes:
                        print_func("Server closed connection.")
                        break
                    msg = engine_data_pb2.EngineData()
                    msg.ParseFromString(msg_bytes)
                    print_func(f"Received: RPM={msg.rpm}, TEMP={msg.temperature}")
                    sleep_func(1)
        except (ConnectionRefusedError, ConnectionResetError):
            print_func("Unable to connect to server, retrying in 2 seconds...")
            sleep_func(2)
        except KeyboardInterrupt:
            print_func("Exiting client.")
            break

if __name__ == "__main__":
    run_client()
