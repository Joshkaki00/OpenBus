import zmq
import json
import time

def test_audio_router_app(server_address="tcp://localhost:5555"):
    """
    Tests communication between AudioRouterApp and the server.
    """
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect(server_address)
    print(f"Connected to server at {server_address}")

    test_commands = [
        {"action": "set_input", "device_name": "InputDevice1"},
        {"action": "set_output", "device_name": "OutputDevice1"},
        {"action": "route_audio", "input": "InputDevice1", "output": "OutputDevice1"},
        {"action": "get_status"},
        {"action": "invalid_command"}
    ]

    for command in test_commands:
        try:
            # Send command
            print(f"\nSending command: {json.dumps(command)}")
            socket.send_json(command)

            # Wait for response
            try:
                reply = socket.recv_json(flags=zmq.NOBLOCK)
                print(f"Received reply: {reply}")
            except zmq.Again:
                print("Timeout: No response from server.")
                continue

            # Validate the response
            if "status" in reply:
                print("Server status:", reply["status"])
            elif "error" in reply:
                print("Server error:", reply["error"])
            else:
                print("Unexpected reply:", reply)
        except Exception as e:
            print(f"Error communicating with server: {e}")

        # Sleep to avoid overwhelming the server
        time.sleep(1)

    # Cleanup
    socket.close()
    context.term()
    print("\nTest completed.")

if __name__ == "__main__":
    test_audio_router_app()