import zmq
import json

def test_server():
    # Initialize ZeroMQ context and REQ socket
    context = zmq.Context()
    socket = context.socket(zmq.REQ)

    # Set a timeout for receiving messages (e.g., 5000ms or 5 seconds)
    socket.RCVTIMEO = 5000  # Timeout in milliseconds
    socket.SNDTIMEO = 5000  # Optional: Timeout for sending messages

    # Connect to the server
    server_address = "tcp://localhost:5555"
    print(f"Connecting to server at {server_address}...")
    socket.connect(server_address)

    # Define test commands
    test_commands = [
        {"action": "get_device_list"},  # Command to get the device list
        {"action": "set_input", "device_name": "InputDevice1"},  # Set input device
        {"action": "set_output", "device_name": "OutputDevice1"},  # Set output device
        {"action": "invalid_command"}  # Invalid command for testing error handling
    ]

    for command in test_commands:
        try:
            # Send the command
            print(f"\nSending command: {json.dumps(command)}")
            socket.send_json(command)

            # Receive the response (timeout applies here)
            reply = socket.recv_json()
            print(f"Received reply: {json.dumps(reply, indent=4)}")

        except zmq.Again as e:
            print(f"Timeout occurred: {e}")
        except zmq.ZMQError as e:
            print(f"ZeroMQ error: {e}")
        except json.JSONDecodeError as e:
            print(f"JSON decode error: {e}")
        except Exception as e:
            print(f"Unexpected error: {e}")

    # Clean up
    socket.close()
    context.term()
    print("\nTest completed.")

if __name__ == "__main__":
    test_server()