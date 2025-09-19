import unittest
from unittest.mock import patch, MagicMock
import struct
import engine_data_pb2
import test_client

class TestRecvN(unittest.TestCase):
    def test_recv_n_exact(self):
        sock = MagicMock()
        sock.recv.side_effect = [b'abc', b'de']
        data = test_client.recv_n(sock, 5)
        self.assertEqual(data, b'abcde')

    def test_recv_n_none(self):
        sock = MagicMock()
        sock.recv.return_value = b''
        data = test_client.recv_n(sock, 4)
        self.assertIsNone(data)

class TestClientProtocol(unittest.TestCase):
    @patch('socket.create_connection')
    def test_client_receives_and_parses_message(self, mock_create_conn):
        # Prepare a fake socket
        fake_sock = MagicMock()
        # Prepare a fake EngineData protobuf message
        msg = engine_data_pb2.EngineData()
        msg.rpm = 1234
        msg.temperature = 56
        msg_bytes = msg.SerializeToString()
        size_bytes = struct.pack('!I', len(msg_bytes))
        # Simulate recv_n: first call returns size, second returns message, then None to break
        recv_n_side_effects = [size_bytes, msg_bytes, None]
        def fake_recv_n(sock, n):
            return recv_n_side_effects.pop(0)
        mock_create_conn.return_value.__enter__.return_value = fake_sock
        # Patch time.sleep to avoid delay
        with patch('time.sleep', return_value=None):
            # Patch print to capture output
            with patch('builtins.print') as mock_print:
                # Run the client for one message
                test_client.run_client(
                    host='127.0.0.1',
                    port=5555,
                    sleep_func=lambda x: None,
                    print_func=mock_print,
                )
                # Check print was called with expected output
                found = any(
                    'Received: RPM=1234, TEMP=56.7' in str(call)
                    for call in mock_print.call_args_list
                )
                self.assertTrue(found)

    @patch('test_client.recv_n')
    @patch('socket.create_connection')
    def test_client_handles_server_disconnect(self, mock_create_conn, mock_recv_n):
        fake_sock = MagicMock()
        mock_create_conn.return_value.__enter__.return_value = fake_sock
        # Simulate server closes connection immediately
        mock_recv_n.return_value = None
        with patch('builtins.print') as mock_print:
            with patch('time.sleep', return_value=None):
                # Run the client, should print server closed connection and then retry
                with self.assertRaises(StopIteration):
                    def break_after_one():
                        yield
                        raise StopIteration
                    gen = break_after_one()
                    def fake_print(*args, **kwargs):
                        next(gen)
                    mock_print.side_effect = fake_print
                    test_client.run_client(
                        host='127.0.0.1',
                        port=5555,
                        sleep_func=lambda x: None,
                        print_func=mock_print,
                    )

if __name__ == '__main__':
    unittest.main()
