import zmq

def test_zmq_server(server_address, timeout=5000):
    """
    Test connection to a ZeroMQ server with a specified timeout.
    
    Args:
        server_address (str): The address of the server to connect to.
        timeout (int): Timeout in milliseconds for send/receive operations.
    """
    try:
        # Create ZeroMQ context and REQ socket
        context = zmq.Context()
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
        # Clean up
        socket.close()
        context.term()

if __name__ == "__main__":
    test_zmq_server("tcp://localhost:5555", timeout=5000)  # Timeout set to 5 seconds