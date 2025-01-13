import zmq
import json
import os
import shutil
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, QPushButton, QLabel, QWidget, QDialog, QSlider, QFileDialog, QMessageBox, QInputDialog, QStatusBar
)
from PyQt5.QtCore import Qt

# Directories for presets and backups
PRESET_DIR = './presets'
BACKUP_DIR = './presets_backup'
os.makedirs(PRESET_DIR, exist_ok=True)
os.makedirs(BACKUP_DIR, exist_ok=True)

class ZMQClient:
    """ZeroMQ client to send commands to the JUCE backend."""
    def __init__(self, address="tcp://localhost:5555"):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect(address)

    def send_command(self, command):
        """Send a JSON command to the backend."""
        self.socket.send_json(command)
        reply = self.socket.recv_json()
        return reply

class PluginSettingsDialog(QDialog):
    """Dialog to adjust plugin parameters."""
    def __init__(self, plugin_name, parent, zmq_client):
        super().__init__()
        self.plugin_name = plugin_name
        self.parent = parent
        self.zmq_client = zmq_client
        self.setWindowTitle(f"Settings for {plugin_name}")
        self.setGeometry(200, 200, 300, 200)

        layout = QVBoxLayout()

        # Gain slider
        self.gain_label = QLabel("Gain: 0")
        self.gain_slider = QSlider(Qt.Horizontal)
        self.gain_slider.setRange(0, 100)
        self.gain_slider.setValue(0)
        self.gain_slider.valueChanged.connect(self.update_gain_label)
        layout.addWidget(self.gain_label)
        layout.addWidget(self.gain_slider)

        # Frequency slider
        self.freq_label = QLabel("Frequency: 0 Hz")
        self.freq_slider = QSlider(Qt.Horizontal)
        self.freq_slider.setRange(20, 20000)
        self.freq_slider.setValue(1000)
        self.freq_slider.valueChanged.connect(self.update_freq_label)
        layout.addWidget(self.freq_label)
        layout.addWidget(self.freq_slider)

        # Apply button
        apply_btn = QPushButton("Apply")
        apply_btn.clicked.connect(self.apply_settings)
        layout.addWidget(apply_btn)

        # Close button
        close_btn = QPushButton("Close")
        close_btn.clicked.connect(self.close)
        layout.addWidget(close_btn)

        self.setLayout(layout)

    def update_gain_label(self, value):
        self.gain_label.setText(f"Gain: {value}")

    def update_freq_label(self, value):
        self.freq_label.setText(f"Frequency: {value} Hz")

    def apply_settings(self):
        """Send updated plugin settings to the backend."""
        settings = {
            "action": "set_plugin_param",
            "plugin_name": self.plugin_name,
            "parameters": {
                "gain": self.gain_slider.value(),
                "frequency": self.freq_slider.value(),
            }
        }
        reply = self.zmq_client.send_command(settings)
        QMessageBox.information(self, "Reply", reply.get("status", "No reply"))

class InputChain(QWidget):
    """Single audio input chain with plugin management."""
    def __init__(self, input_id, status_bar, zmq_client):
        super().__init__()
        self.input_id = input_id
        self.plugins = []
        self.status_bar = status_bar
        self.zmq_client = zmq_client

        layout = QVBoxLayout()
        self.label = QLabel(f"Input {self.input_id}")
        layout.addWidget(self.label)

        self.add_plugin_btn = QPushButton(f"Add Plugin to Input {self.input_id}")
        self.add_plugin_btn.clicked.connect(self.add_plugin)
        layout.addWidget(self.add_plugin_btn)

        self.plugins_container = QVBoxLayout()
        layout.addLayout(self.plugins_container)
        self.setLayout(layout)

    def add_plugin(self):
        plugin_name, ok = QInputDialog.getText(self, "Add Plugin", "Enter Plugin Name:")
        if ok and plugin_name:
            command = {"action": "add_plugin", "input": self.input_id, "path": plugin_name}
            reply = self.zmq_client.send_command(command)
            QMessageBox.information(self, "Reply", reply.get("status", "No reply"))

class MainUI(QMainWindow):
    """Main application window."""
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Audio Mixer - Preset Management")
        self.setGeometry(100, 100, 600, 400)

        self.zmq_client = ZMQClient()
        self.layout = QVBoxLayout()
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)

        self.input_chains = []
        for i in range(1, 4):
            chain = InputChain(i, self.status_bar, self.zmq_client)
            self.input_chains.append(chain)
            self.layout.addWidget(chain)

        save_btn = QPushButton("Save Preset")
        save_btn.clicked.connect(self.save_preset)
        self.layout.addWidget(save_btn)

        load_btn = QPushButton("Load Preset")
        load_btn.clicked.connect(self.load_preset)
        self.layout.addWidget(load_btn)

        container = QWidget()
        container.setLayout(self.layout)
        self.setCentralWidget(container)

    def save_preset(self):
        """Save the current preset."""
        command = {"action": "save_preset"}
        reply = self.zmq_client.send_command(command)
        QMessageBox.information(self, "Reply", reply.get("status", "No reply"))

    def load_preset(self):
        """Load a preset."""
        command = {"action": "load_preset"}
        reply = self.zmq_client.send_command(command)
        QMessageBox.information(self, "Reply", reply.get("status", "No reply"))

if __name__ == "__main__":
    app = QApplication([])
    window = MainUI()
    window.show()
    app.exec_()