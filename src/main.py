import sys
import os

# Add src to python path
sys.path.insert(0, os.path.abspath(os.path.dirname(__file__)))

from PyQt5.QtWidgets import QApplication
from gui import MainWindow


def main():
    app = QApplication(sys.argv)
    app.setStyle("Fusion")
    win = MainWindow()
    win.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
