import zmq
import json
import time
import threading

class AudioRouterTester:
    def __init__(self, server_address="tcp://localhost:5555", timeout=5000):
        """
        Initializes the tester with the server address and timeout.
        """
        self.server_address = server_address
        self.timeout = timeout / 1000.0  # Convert to seconds for Python's time module
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect(self.server_address)
        print(f"Connected to server at {self.server_address}")

    def send_command(self, command):
        """
        Sends a command to the server and waits for the response.
        """
        try:
            # Send command
            print(f"\nSending command: {json.dumps(command)}")
            self.socket.send_json(command)

            # Wait for response with timeout
            poller = zmq.Poller()
            poller.register(self.socket, zmq.POLLIN)
            socks = dict(poller.poll(self.timeout * 1000))

            if self.socket in socks and socks[self.socket] == zmq.POLLIN:
                reply = self.socket.recv_json()
                print(f"Received reply: {reply}")
                return reply
            else:
                print("Timeout: No response from server.")
                return None
        except Exception as e:
            print(f"Error communicating with server: {e}")
            return None

    def run_tests(self):
        """
        Runs a series of test commands to verify server communication.
        """
        test_commands = [
            {"action": "set_input", "device_name": "InputDevice1"},
            {"action": "set_output", "device_name": "OutputDevice1"},
            {"action": "route_audio", "input": "InputDevice1", "output": "OutputDevice1"},
            {"action": "get_status"},
            {"action": "invalid_command"}
        ]

        for command in test_commands:
            response = self.send_command(command)
            if response:
                # Validate the response
                if "status" in response:
                    print("Server status:", response["status"])
                elif "error" in response:
                    print("Server error:", response["error"])
                else:
                    print("Unexpected reply:", response)
            time.sleep(1)

    def cleanup(self):
        """
        Cleans up the ZeroMQ resources.
        """
        self.socket.close()
        self.context.term()
        print("\nTester cleaned up.")


if __name__ == "__main__":
    tester = AudioRouterTester()
    try:
        tester.run_tests()
    finally:
        tester.cleanup()