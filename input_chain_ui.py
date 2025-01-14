from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QPushButton,
    QLabel, QWidget, QInputDialog, QStatusBar, QMessageBox
)
from PyQt5.QtCore import Qt
import os
import zmq

# Directories for presets
PRESET_DIR = './presets'
os.makedirs(PRESET_DIR, exist_ok=True)

class ZeroMQClient:
    """Handles communication with the JUCE backend via ZeroMQ."""
    def __init__(self):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://localhost:5555")

    def send_command(self, command):
        """Send JSON commands to JUCE backend and receive a response."""
        try:
            # Using nlohmann::json compatible JSON format
            self.socket.send_string(command)
            reply = self.socket.recv_string()
            print("Reply from JUCE:", reply)
            return reply
        except Exception as e:
            print("Error communicating with JUCE:", e)
            return '{"status": "error", "message": "' + str(e) + '"}'

class MainUI(QMainWindow):
    """Main application window."""
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Audio Mixer - Preset Management")
        self.setGeometry(100, 100, 600, 400)

        self.client = ZeroMQClient()
        self.layout = QVBoxLayout()
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)

        # Dropdowns for input and output selection
        self.input_label = QLabel("Input")
        self.output_label = QLabel("Output")
        self.input_button = QPushButton("Select Input")
        self.output_button = QPushButton("Select Output")
        self.input_button.clicked.connect(self.set_input)
        self.output_button.clicked.connect(self.set_output)
        self.layout.addWidget(self.input_label)
        self.layout.addWidget(self.input_button)
        self.layout.addWidget(self.output_label)
        self.layout.addWidget(self.output_button)

        # Save and load preset buttons
        save_btn = QPushButton("Save Preset")
        save_btn.clicked.connect(self.save_preset)
        load_btn = QPushButton("Load Preset")
        load_btn.clicked.connect(self.load_preset)
        self.layout.addWidget(save_btn)
        self.layout.addWidget(load_btn)

        # Main widget container
        container = QWidget()
        container.setLayout(self.layout)
        self.setCentralWidget(container)

    def set_input(self):
        """Handle input selection."""
        input_id, ok = QInputDialog.getInt(self, "Set Input", "Enter Input ID:")
        if ok:
            command = f'{{"action": "set_input", "input_id": {input_id}}}'
            reply = self.client.send_command(command)
            QMessageBox.information(self, "Response", reply)

    def set_output(self):
        """Handle output selection."""
        output_id, ok = QInputDialog.getInt(self, "Set Output", "Enter Output ID:")
        if ok:
            command = f'{{"action": "set_output", "output_id": {output_id}}}'
            reply = self.client.send_command(command)
            QMessageBox.information(self, "Response", reply)

    def save_preset(self):
        """Handle preset saving."""
        preset_name, ok = QInputDialog.getText(self, "Save Preset", "Preset Name:")
        if ok and preset_name:
            command = f'{{"action": "save_preset", "preset_name": "{preset_name}"}}'
            reply = self.client.send_command(command)
            QMessageBox.information(self, "Response", reply)

    def load_preset(self):
        """Handle preset loading."""
        presets = [f.split('.')[0] for f in os.listdir(PRESET_DIR) if f.endswith('.json')]
        if not presets:
            QMessageBox.warning(self, "No Presets", "No presets found to load.")
            return

        preset_name, ok = QInputDialog.getItem(self, "Load Preset", "Select Preset:", presets, 0, False)
        if ok and preset_name:
            command = f'{{"action": "load_preset", "preset_name": "{preset_name}"}}'
            reply = self.client.send_command(command)
            QMessageBox.information(self, "Response", reply)

if __name__ == "__main__":
    app = QApplication([])
    window = MainUI()
    window.show()
    app.exec_()