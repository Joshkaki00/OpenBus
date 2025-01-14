from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QPushButton,
    QLabel, QWidget, QInputDialog, QMessageBox, QStatusBar
)
from PyQt5.QtCore import Qt
import json
import zmq

class ZeroMQClient:
    """Handles communication with the JUCE backend via ZeroMQ."""
    def __init__(self):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://localhost:5555")

    def send_command(self, command):
        """Send JSON commands to JUCE and receive the response."""
        try:
            self.socket.send_json(command)
            reply = self.socket.recv_json()
            print("Reply from JUCE:", reply)
            return reply
        except Exception as e:
            print("Error communicating with JUCE:", e)
            return {"status": "error", "message": str(e)}

class MainUI(QMainWindow):
    """Main application window."""
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Audio Mixer - Device Management")
        self.setGeometry(100, 100, 600, 400)

        self.client = ZeroMQClient()
        self.layout = QVBoxLayout()
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)

        # Fetch devices
        self.devices = self.fetch_device_list()
        self.inputs = self.devices.get("inputs", [])
        self.outputs = self.devices.get("outputs", [])

        # Dropdowns for input and output
        self.input_label = QLabel("Input Device")
        self.input_dropdown = QPushButton("Select Input")
        self.input_dropdown.clicked.connect(self.select_input)
        self.layout.addWidget(self.input_label)
        self.layout.addWidget(self.input_dropdown)

        self.output_label = QLabel("Output Device")
        self.output_dropdown = QPushButton("Select Output")
        self.output_dropdown.clicked.connect(self.select_output)
        self.layout.addWidget(self.output_label)
        self.layout.addWidget(self.output_dropdown)

        # Container widget
        container = QWidget()
        container.setLayout(self.layout)
        self.setCentralWidget(container)

    def fetch_device_list(self):
        """Fetch available input and output devices from JUCE backend."""
        command = {"action": "get_device_list"}
        reply = self.client.send_command(command)
        return reply

    def select_input(self):
        """Select input device."""
        if not self.inputs:
            QMessageBox.warning(self, "No Devices", "No input devices available.")
            return
        input_device, ok = QInputDialog.getItem(self, "Select Input", "Choose an Input Device:", self.inputs, 0, False)
        if ok and input_device:
            command = {"action": "set_input", "device_name": input_device}
            reply = self.client.send_command(command)
            self.status_bar.showMessage(reply.get("message", "Unknown error"))

    def select_output(self):
        """Select output device."""
        if not self.outputs:
            QMessageBox.warning(self, "No Devices", "No output devices available.")
            return
        output_device, ok = QInputDialog.getItem(self, "Select Output", "Choose an Output Device:", self.outputs, 0, False)
        if ok and output_device:
            command = {"action": "set_output", "device_name": output_device}
            reply = self.client.send_command(command)
            self.status_bar.showMessage(reply.get("message", "Unknown error"))

if __name__ == "__main__":
    app = QApplication([])
    window = MainUI()
    window.show()
    app.exec_()