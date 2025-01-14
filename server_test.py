import zmq
import json

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")
socket.RCVTIMEO = 5000  # Timeout in milliseconds

print("Test server running on tcp://*:5555")
while True:
    message = socket.recv_json()
    print(f"Received message: {message}")
    response = {"status": "success", "data": "Test response"}
    socket.send_json(response)