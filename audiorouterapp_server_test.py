import zmq
import json
import time
import threading


class AudioRouterTester:
    """
    Tests communication between AudioRouterApp and its server.
    """

    def __init__(self, server_address="tcp://localhost:5555", timeout=5000):
        """
        Initializes the tester with server address and timeout settings.
        """
        self.server_address = server_address
        self.timeout = timeout / 1000.0  # Convert milliseconds to seconds
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect(self.server_address)
        self.poller = zmq.Poller()
        self.poller.register(self.socket, zmq.POLLIN)
        print(f"Connected to server at {self.server_address}")

    def send_command(self, command):
        """
        Sends a command to the server and waits for a response.
        """
        try:
            # Send command
            print(f"\nSending command: {json.dumps(command)}")
            self.socket.send_json(command)

            # Poll for a response within the timeout
            socks = dict(self.poller.poll(self.timeout * 1000))
            if self.socket in socks and socks[self.socket] == zmq.POLLIN:
                response = self.socket.recv_json()
                print(f"Received reply: {response}")
                return response
            else:
                print("Timeout: No response from server.")
                return None
        except Exception as e:
            print(f"Error communicating with server: {e}")
            return None

    def run_tests(self):
        """
        Runs a series of test commands to validate server communication.
        """
        test_commands = [
            {"action": "ping"},  # Basic ping to check server connectivity
            {"action": "set_input", "device_name": "InputDevice1"},
            {"action": "set_output", "device_name": "OutputDevice1"},
            {"action": "route_audio", "input": "InputDevice1", "output": "OutputDevice1"},
            {"action": "get_status"},
            {"action": "invalid_command"}  # Intentionally invalid command to test error handling
        ]

        for command in test_commands:
            response = self.send_command(command)
            if response:
                # Check and handle server responses
                if "status" in response:
                    print(f"Server status: {response['status']}")
                elif "error" in response:
                    print(f"Server error: {response['error']}")
                else:
                    print(f"Unexpected response: {response}")
            time.sleep(1)  # Delay to avoid overwhelming the server

    def cleanup(self):
        """
        Cleans up sockets and context after testing.
        """
        print("\nCleaning up resources...")
        self.socket.close()
        self.context.term()
        print("Resources cleaned up. Test completed.")


if __name__ == "__main__":
    try:
        tester = AudioRouterTester()
        tester.run_tests()
    except KeyboardInterrupt:
        print("\nTest interrupted by user.")
    finally:
        tester.cleanup()