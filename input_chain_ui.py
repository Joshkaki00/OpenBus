from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QPushButton, QLabel, QWidget, QHBoxLayout

class InputChain(QWidget):
    """Class representing a single audio input chain."""
    def __init__(self, input_id):
        super().__init__()
        self.input_id = input_id
        self.plugins = []

        # Layout for this input chain
        self.layout = QVBoxLayout()

        # Label to display the input ID
        self.label = QLabel(f"Input {self.input_id}")
        self.layout.addWidget(self.label)

        # Button to add plugins
        self.add_plugin_btn = QPushButton(f"Add Plugin to Input {self.input_id}")
        self.add_plugin_btn.clicked.connect(self.add_plugin)
        self.layout.addWidget(self.add_plugin_btn)

        # Label to display the list of plugins
        self.plugins_label = QLabel("Plugins: None")
        self.layout.addWidget(self.plugins_label)

        self.setLayout(self.layout)

    def add_plugin(self):
        """Simulate adding a plugin."""
        plugin_name = f"Plugin {len(self.plugins) + 1}"
        self.plugins.append(plugin_name)

        # Update plugins display
        self.plugins_label.setText(f"Plugins: {', '.join(self.plugins)}")
        print(f"Added {plugin_name} to Input {self.input_id}")


class MainUI(QMainWindow):
    """Main window containing all input chains."""
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Audio Mixer - Input Chains")
        self.setGeometry(100, 100, 600, 400)

        # Main layout to hold input chains
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
