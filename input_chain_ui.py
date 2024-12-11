from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, QPushButton, QLabel, QWidget, QDialog, QSlider, QPushButton
)

from PyQt5.QtCore import Qt

class InputChain(QWidget):
    """Class representing a single audio input chain with plugin management."""
    def __init__(self, input_id):
        super().__init__()
        self.input_id = input_id
        self.plugins = []

        # Main layout for the input chain
        self.layout = QVBoxLayout()

        # Label to display input ID
        self.label = QLabel(f"Input {self.input_id}")
        self.layout.addWidget(self.label)

        # Button to add plugins
        self.add_plugin_btn = QPushButton(f"Add Plugin to Input {self.input_id}")
        self.add_plugin_btn.clicked.connect(self.add_plugin)
        self.layout.addWidget(self.add_plugin_btn)

        # Container for plugin list
        self.plugins_container = QVBoxLayout()
        self.layout.addLayout(self.plugins_container)

        # Set the main layout
        self.setLayout(self.layout)

    def add_plugin(self):
        """Simulate adding a plugin."""
        plugin_name = f"Plugin {len(self.plugins) + 1}"
        self.plugins.append(plugin_name)

        # Create a horizontal layout for the plugin entry
        plugin_layout = QHBoxLayout()

        # Label for plugin name
        plugin_label = QLabel(plugin_name)
        plugin_layout.addWidget(plugin_label)

        # Remove button
        remove_btn = QPushButton("Remove")
        remove_btn.clicked.connect(lambda: self.remove_plugin(plugin_name, plugin_label, remove_btn))
        plugin_layout.addWidget(remove_btn)

        # Add the plugin layout to the container
        self.plugins_container.addLayout(plugin_layout)

        # Print feedback
        print(f"Added {plugin_name} to Input {self.input_id}")

    def remove_plugin(self, plugin_name, label, button):
        """Remove a plugin from the input chain."""
        self.plugins.remove(plugin_name)

        # Remove the label and button from the layout
        self.plugins_container.removeWidget(label)
        self.plugins_container.removeWidget(button)
        label.deleteLater()
        button.deleteLater()

        # Print feedback
        print(f"Removed {plugin_name} from Input {self.input_id}")


class MainUI(QMainWindow):
    """Main window containing all input chains."""
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Audio Mixer - Refined Input Chains")
        self.setGeometry(100, 100, 600, 400)

        # Main layout for the window
        self.layout = QVBoxLayout()

        # Create 5 input chains
        self.input_chains = []
        for i in range(1, 6):
            chain = InputChain(i)
            self.input_chains.append(chain)
            self.layout.addWidget(chain)

        # Container widget for the main layout
        container = QWidget()
        container.setLayout(self.layout)
        self.setCentralWidget(container)


if __name__ == "__main__":
    app = QApplication([])

    # Create and display the main UI
    window = MainUI()
    window.show()

    app.exec_()

class PluginSettingsDialog(QDialog):
    """Dialog to adjust plugin parameters."""
    def __init__(self, plugin_name):
        super().__init__()
        self.setWindowTitle(f"Settings for {plugin_name}")
        self.setGeometry(200, 200, 300, 200)

        # Main layout
        layout = QVBoxLayout()

        # Example parameter: Gain
        self.gain_label = QLabel("Gain: 0")
        self.gain_slider = QSlider(Qt.Horizontal)
        self.gain_slider.setMinimum(0)
        self.gain_slider.setMaximum(100)
        self.gain_slider.setValue(0)
        self.gain_slider.valueChanged.connect(self.update_gain_label)
        layout.addWidget(self.gain_label)
        layout.addWidget(self.gain_slider)

        # Example parameter: Frequency
        self.freq_label = QLabel("Frequency: 0 Hz")
        self.freq_slider = QSlider(Qt.Horizontal)
        self.freq_slider.setMinimum(20)
        self.freq_slider.setMaximum(20000)
        self.freq_slider.setValue(1000)
        self.freq_slider.valueChanged.connect(self.update_freq_label)
        layout.addWidget(self.freq_label)
        layout.addWidget(self.freq_slider)

        # Close button
        close_btn = QPushButton("Close")
        close_btn.clicked.connect(self.close)
        layout.addWidget(close_btn)

        self.setLayout(layout)

    def update_gain_label(self, value):
        """Update the Gain label dynamically."""
        self.gain_label.setText(f"Gain: {value}")

    def update_freq_label(self, value):
        """Update the Frequency label dynamically."""
        self.freq_label.setText(f"Frequency: {value} Hz")