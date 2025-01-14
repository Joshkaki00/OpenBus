import zmq
import time

def test_zmq_server(server_address, timeout=5000):
    """
    Test connection to a ZeroMQ server with a specified timeout.

    Args:
        server_address (str): The address of the server to connect to.
        timeout (int): Timeout in milliseconds for send/receive operations.
    """
    # Create ZeroMQ context and REQ socket
    context = zmq.Context()
    socket = None
    try:
        socket = context.socket(zmq.REQ)
        socket.connect(server_address)
        print(f"Connected to server at {server_address}")

        # Set send and receive timeouts
        socket.setsockopt(zmq.RCVTIMEO, timeout)
        socket.setsockopt(zmq.SNDTIMEO, timeout)

        # Sending a ping message
        try:
            socket.send_json({"action": "ping"})
            print("Message sent, waiting for response...")

            # Receiving the response with timeout
            response = socket.recv_json()
            print(f"Response from server: {response}")
        except zmq.error.Again:
            print("Timeout: No response received within the specified timeout.")

    except Exception as e:
        print(f"Error: {e}")

    finally:
        # Properly close the socket and terminate the context
        if socket:
            try:
                socket.close(linger=0)  # Avoid waiting for pending messages
            except Exception as e:
                print(f"Error while closing socket: {e}")

        # Delay to ensure sockets are released before context termination
        time.sleep(0.1)

        try:
            context.term()  # Now safe to terminate
        except Exception as e:
            print(f"Error while terminating context: {e}")

if __name__ == "__main__":
    test_zmq_server("tcp://localhost:5555", timeout=5000)  # Timeout set to 5 seconds