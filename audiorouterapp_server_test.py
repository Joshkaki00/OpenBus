import zmq

def test_zmq_server(server_address):
    try:
        context = zmq.Context()
        socket = context.socket(zmq.REQ)
        socket.connect(server_address)
        print(f"Connected to server at {server_address}")
        
        # Sending a ping message
        socket.send_json({"action": "ping"})
        response = socket.recv_json()
        print(f"Response from server: {response}")
        
    except Exception as e:
        print(f"Error: {e}")
    finally:
        socket.close()
        context.term()

if __name__ == "__main__":
    test_zmq_server("tcp://localhost:5555")