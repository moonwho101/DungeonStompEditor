import sys
import os

sys.path.insert(0, os.path.abspath("src"))

from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import QSize
from gui import MainWindow

def capture_screenshot():
    app = QApplication(sys.argv)
    app.setStyle("Fusion")
    win = MainWindow()
    win.resize(1100, 750)
    win.show()

    # Process events so the window renders
    app.processEvents()

    # Capture screenshot of the window
    pixmap = win.grab()
    screenshot_file = "tests/editor_ui.png"
    pixmap.save(screenshot_file, "PNG")
    print(f"UI Screenshot saved to {screenshot_file}")

if __name__ == "__main__":
    capture_screenshot()
