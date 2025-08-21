import socket
import time

HOST = '127.0.0.1'  # or use the server's IP address
PORT = 5555

while True:
    try:
        with socket.create_connection((HOST, PORT)) as s:
            while True:
                # Send a request (any data, e.g., a newline)
                s.sendall(b'get\n')
                data = s.recv(1024)
                if not data:
                    print("Server closed connection.")
                    break
                print("Received:", data.decode().strip())
                time.sleep(1)
    except (ConnectionRefusedError, ConnectionResetError):
        print("Unable to connect to server, retrying in 2 seconds...")
        time.sleep(2)
    except KeyboardInterrupt:
        print("Exiting client.")
        break
