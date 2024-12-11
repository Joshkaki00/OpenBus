from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QLabel, QPushButton, QWidget

class MainUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Test UI")
        self.setGeometry(100, 100, 400, 300)

        layout = QVBoxLayout()

        label = QLabel("Hello, PyQt5!")
        layout.addWidget(label)

        button = QPushButton("Click Me")
        button.clicked.connect(self.on_click)
        layout.addWidget(button)

        container = QWidget()
        container.setLayout(layout)
        self.setCentralWidget(container)

    def on_click(self):
        print("Button Clicked!")

if __name__ == "__main__":
    app = QApplication([])
    window = MainUI()
    window.show()
    app.exec_()
