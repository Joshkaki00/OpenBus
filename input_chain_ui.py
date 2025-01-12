from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, QPushButton, QLabel, QWidget, QDialog, QSlider, QFileDialog, QMessageBox, QInputDialog, QStatusBar
)

from PyQt5.QtCore import Qt
import json
import os
import shutil

# Directories for presets and backups
PRESET_DIR = './presets'
BACKUP_DIR = './presets_backup'
os.makedirs(PRESET_DIR, exist_ok=True)
os.makedirs(BACKUP_DIR, exist_ok=True)

class PluginSettingsDialog(QDialog):
    """Dialog to adjust plugin parameters."""
    def __init__(self, plugin_name, parent):
        super().__init__()
        self.plugin_name = plugin_name
        self.parent = parent
        self.setWindowTitle(f"Settings for {plugin_name}")
        self.setGeometry(200, 200, 300, 200)

        layout = QVBoxLayout()

        # Gain slider
        self.gain_label = QLabel("Gain: 0")
        self.gain_slider = QSlider(Qt.Horizontal)
        self.gain_slider.setRange(0, 100)
        self.gain_slider.setValue(0)
        self.gain_slider.valueChanged.connect(self.update_gain_label)
        self.gain_slider.valueChanged.connect(self.update_parent_gain)
        layout.addWidget(self.gain_label)
        layout.addWidget(self.gain_slider)

        # Frequency slider
        self.freq_label = QLabel("Frequency: 0 Hz")
        self.freq_slider = QSlider(Qt.Horizontal)
        self.freq_slider.setRange(20, 20000)
        self.freq_slider.setValue(1000)
        self.freq_slider.valueChanged.connect(self.update_freq_label)
        self.freq_slider.valueChanged.connect(self.update_parent_frequency)
        layout.addWidget(self.freq_label)
        layout.addWidget(self.freq_slider)

        # Close button
        close_btn = QPushButton("Close")
        close_btn.clicked.connect(self.close)
        layout.addWidget(close_btn)

        self.setLayout(layout)

    def update_gain_label(self, value):
        self.gain_label.setText(f"Gain: {value}")

    def update_freq_label(self, value):
        self.freq_label.setText(f"Frequency: {value} Hz")

    def update_parent_gain(self, value):
        self.parent.update_gain(value)

    def update_parent_frequency(self, value):
        self.parent.update_frequency(value)

    def get_parameters(self):
        return {'gain': self.gain_slider.value(), 'frequency': self.freq_slider.value()}

class InputChain(QWidget):
    """Single audio input chain with plugin management."""
    def __init__(self, input_id, status_bar):
        super().__init__()
        self.input_id = input_id
        self.plugins = []
        self.status_bar = status_bar

        layout = QVBoxLayout()

        self.label = QLabel(f"Input {self.input_id}")
        layout.addWidget(self.label)

        self.add_plugin_btn = QPushButton(f"Add Plugin to Input {self.input_id}")
        self.add_plugin_btn.clicked.connect(self.add_plugin)
        layout.addWidget(self.add_plugin_btn)

        self.plugins_container = QVBoxLayout()
        layout.addLayout(self.plugins_container)

        self.reset_btn = QPushButton("Reset Chain")
        self.reset_btn.clicked.connect(self.reset_chain)
        layout.addWidget(self.reset_btn)

        self.setLayout(layout)

    def add_plugin(self):
        plugin_name = f"Plugin {len(self.plugins) + 1}"
        settings = PluginSettingsDialog(plugin_name, self)
        self.plugins.append({'name': plugin_name, 'settings': settings})

        plugin_layout = QHBoxLayout()
        plugin_label = QLabel(plugin_name)
        plugin_layout.addWidget(plugin_label)

        self.gain_display = QLabel("Gain: 0")
        self.freq_display = QLabel("Freq: 1000 Hz")
        plugin_layout.addWidget(self.gain_display)
        plugin_layout.addWidget(self.freq_display)

        settings_btn = QPushButton("Settings")
        settings_btn.clicked.connect(lambda: settings.exec_())
        plugin_layout.addWidget(settings_btn)

        remove_btn = QPushButton("Remove")
        remove_btn.clicked.connect(lambda: self.remove_plugin(plugin_name, plugin_label, settings_btn, remove_btn))
        plugin_layout.addWidget(remove_btn)

        self.plugins_container.addLayout(plugin_layout)
        self.status_bar.showMessage(f"Added {plugin_name} to Input {self.input_id}")

    def update_gain(self, value):
        self.gain_display.setText(f"Gain: {value}")

    def update_frequency(self, value):
        self.freq_display.setText(f"Freq: {value} Hz")

    def reset_chain(self):
        self.plugins.clear()
        for i in reversed(range(self.plugins_container.count())):
            widget = self.plugins_container.itemAt(i).layout()
            if widget:
                while widget.count():
                    item = widget.takeAt(0)
                    if item.widget():
                        item.widget().deleteLater()
                self.plugins_container.removeItem(widget)
        self.status_bar.showMessage(f"Reset Input {self.input_id}")

class MainUI(QMainWindow):
    """Main application window."""
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Audio Mixer - Preset Management")
        self.setGeometry(100, 100, 600, 400)

        self.layout = QVBoxLayout()
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)

        self.input_chains = []
        for i in range(1, 6):
            chain = InputChain(i, self.status_bar)
            self.input_chains.append(chain)
            self.layout.addWidget(chain)

        save_btn = QPushButton("Save Preset")
        save_btn.clicked.connect(self.save_preset)
        self.layout.addWidget(save_btn)

        load_btn = QPushButton("Load Preset")
        load_btn.clicked.connect(self.load_preset)
        self.layout.addWidget(load_btn)

        backup_btn = QPushButton("Backup Presets")
        backup_btn.clicked.connect(self.backup_presets)
        self.layout.addWidget(backup_btn)

        export_btn = QPushButton("Export Presets")
        export_btn.clicked.connect(self.export_presets)
        self.layout.addWidget(export_btn)

        container = QWidget()
        container.setLayout(self.layout)
        self.setCentralWidget(container)

    def backup_presets(self):
        try:
            os.makedirs(BACKUP_DIR, exist_ok=True)
            for preset in os.listdir(PRESET_DIR):
                shutil.copy(
                    os.path.join(PRESET_DIR, preset),
                    os.path.join(BACKUP_DIR, preset)
                )
            QMessageBox.information(self, "Success", "Presets backed up successfully.")
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to backup presets: {str(e)}")

    def save_preset(self):
        preset_name, ok = QInputDialog.getText(self, "Save Preset", "Preset Name:")
        if ok and preset_name:
            filename = os.path.join(PRESET_DIR, f"{preset_name}.json")
            data = [
                [{'name': p['name'], 'parameters': p['settings'].get_parameters()} for p in chain.plugins]
                for chain in self.input_chains
            ]
            with open(filename, 'w') as file:
                json.dump(data, file)
            QMessageBox.information(self, "Success", f"Preset '{preset_name}' saved.")

    def load_preset(self):
        presets = [f.split('.')[0] for f in os.listdir(PRESET_DIR) if f.endswith('.json')]
        preset_name, ok = QInputDialog.getItem(self, "Load Preset", "Select Preset:", presets, 0, False)
        if ok and preset_name:
            filename = os.path.join(PRESET_DIR, f"{preset_name}.json")
            with open(filename, 'r') as file:
                data = json.load(file)
            for chain, plugins in zip(self.input_chains, data):
                chain.plugins = []
                for plugin in plugins:
                    settings = PluginSettingsDialog(plugin['name'], chain)
                    settings.gain_slider.setValue(plugin['parameters']['gain'])
                    settings.freq_slider.setValue(plugin['parameters']['frequency'])
                    chain.plugins.append({'name': plugin['name'], 'settings': settings})
            QMessageBox.information(self, "Success", f"Preset '{preset_name}' loaded.")

    def export_presets(self):
        export_dir = QFileDialog.getExistingDirectory(self, "Select Export Directory")
        if export_dir:
            for preset in os.listdir(PRESET_DIR):
                shutil.copy(
                    os.path.join(PRESET_DIR, preset),
                    os.path.join(export_dir, preset)
                )
            QMessageBox.information(self, "Success", "Presets exported successfully.")

if __name__ == "__main__":
    app = QApplication([])
    window = MainUI()
    window.show()
    app.exec_()