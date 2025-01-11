import sys
import os
import subprocess
import shutil
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QLabel, QPushButton, QComboBox, QFileDialog, QMessageBox, QProgressBar,
    QCheckBox, QStyleFactory, QDialog, QFormLayout, QLineEdit
)
from PyQt5.QtCore import Qt, QThread, pyqtSignal
from PyQt5.QtGui import QIcon


# Додано перевірку на наявність Arduino CLI та PyQt5
def check_dependencies():
    if shutil.which("arduino-cli") is None:
        QMessageBox.critical(None, "Помилка", "Не знайдено Arduino CLI. Будь ласка, завантажте Arduino CLI.")
        sys.exit(1)

    try:
        import PyQt5
    except ImportError:
        QMessageBox.critical(None, "Помилка", "PyQt5 не знайдено. Будь ласка, встановіть PyQt5.")
        sys.exit(1)

check_dependencies()

class FlashThread(QThread):
    progress = pyqtSignal(int)
    result = pyqtSignal(str)

    def __init__(self, ino_file, port, board, processor):
        super().__init__()
        self.ino_file = ino_file
        self.port = port
        self.board = board
        self.processor = processor

    def run(self):
        try:
            project_folder = os.path.dirname(self.ino_file)
            os.chdir(project_folder)

            fqbn = self.board
            board_options = ""
            if self.board == "arduino:avr:nano":
                board_options = f"--board-options cpu={self.processor}"

            # Команда компіляції
            compile_command = [
                "arduino-cli", "compile", "--fqbn", fqbn, self.ino_file
            ]
            if board_options:
                compile_command.extend(board_options.split())

            compile_result = subprocess.run(
                compile_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
            )
            if compile_result.returncode != 0:
                self.result.emit(f"Не вдалося скомпілювати:\n{compile_result.stderr}")
                return

            self.progress.emit(50)  # Update progress bar to 50%

            # Команда завантаження
            upload_command = [
                "arduino-cli", "upload", "--fqbn", fqbn, "-p", self.port, self.ino_file
            ]
            if board_options:
                upload_command.extend(board_options.split())

            upload_result = subprocess.run(
                upload_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
            )

            if upload_result.returncode == 0:
                self.result.emit("Arduino успішно прошито!")
            else:
                self.result.emit(f"Не вдалося завантажити:\n{upload_result.stderr}")

            self.progress.emit(100)  # Update progress bar to 100%
        except Exception as e:
            self.result.emit(f"Помилка: {e}")


class ArduinoFlasher(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Програматор Arduino")
        self.setGeometry(100, 100, 600, 500)

        # Використовуємо стиль fusion з Qt для темної теми
        self.setStyle(QStyleFactory.create("Fusion"))  # Темна тема

        self.layout = QVBoxLayout()

        # Тема темного стилю для Apple
        self.setStyleSheet("""
            QWidget {
                background-color: #2e2e2e;
                color: #dcdcdc;
            }
            QLabel {
                font-size: 14px;
                margin-bottom: 10px;
                font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", "Roboto", "Helvetica Neue", Arial, sans-serif;
                color: #dcdcdc;
            }
            QPushButton {
                background-color: #007aff;
                color: white;
                border-radius: 10px;
                font-size: 14px;
                padding: 12px;
            }
            QPushButton:hover {
                background-color: #0061a8;
            }
            QComboBox {
                background-color: #3c3c3c;
                color: #dcdcdc;
                padding: 12px;
                border-radius: 10px;
            }
            QProgressBar {
                background-color: #555555;
                border-radius: 10px;
            }
            QProgressBar::chunk {
                background-color: #007aff;
            }
            QCheckBox {
                color: #007aff;
                font-size: 14px;
            }
            #settings_button {
                background-color: #808080;  /* Сірий колір для кнопки */
                font-size: 10px;  /* Малий розмір кнопки */
                padding: 6px;
                border-radius: 8px;
                color: white;
            }
            #settings_button:hover {
                background-color: #616161;  /* Темніший сірий при наведенні */
            }
        """)

        # Вибір INO файлу
        self.file_label = QLabel("Виберіть INO файл:")
        self.layout.addWidget(self.file_label)
        self.file_button = QPushButton("Огляд")
        self.file_button.clicked.connect(self.select_file)
        self.layout.addWidget(self.file_button)

        # Вибір порту
        self.port_label = QLabel("Виберіть COM порт:")
        self.layout.addWidget(self.port_label)
        self.port_combo = QComboBox()
        self.layout.addWidget(self.port_combo)
        self.refresh_ports()
        self.refresh_button = QPushButton("Оновити порти")
        self.refresh_button.clicked.connect(self.refresh_ports)
        self.layout.addWidget(self.refresh_button)

        # Вибір плати
        self.board_label = QLabel("Виберіть Arduino:")
        self.layout.addWidget(self.board_label)
        self.board_combo = QComboBox()
        self.board_combo.addItems([
            "Arduino: Uno", "Arduino: Nano", "Arduino: Mega 2560", "Arduino: Leonardo",
            "Arduino: Due", "Arduino: Micro", "Arduino: Pro Mini"
        ])
        self.layout.addWidget(self.board_combo)

        # Вибір процесора
        self.processor_label = QLabel("Виберіть процесор:")
        self.layout.addWidget(self.processor_label)
        self.processor_combo = QComboBox()
        self.processor_combo.addItems([
            "atmega328", "atmega328old", "atmega2560", "atmega32u4", "sam3x8e"
        ])
        self.layout.addWidget(self.processor_combo)

        # Кнопка налаштувань
        self.settings_button = QPushButton("Налаштування")
        self.settings_button.setObjectName("settings_button")  # Клас для стилізації
        self.settings_button.clicked.connect(self.open_settings)
        self.layout.addWidget(self.settings_button)

        # Кнопка прошивки
        self.flash_button = QPushButton("Пошити Arduino")
        self.flash_button.clicked.connect(self.flash_arduino)
        self.layout.addWidget(self.flash_button)

        # Прогрес бар
        self.progress_bar = QProgressBar()
        self.progress_bar.setValue(0)
        self.progress_bar.setAlignment(Qt.AlignCenter)
        self.layout.addWidget(self.progress_bar)

        self.setLayout(self.layout)

    def refresh_ports(self):
        try:
            result = subprocess.run(["arduino-cli", "board", "list"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            lines = result.stdout.split("\n")
            ports = [line.split()[0] for line in lines if "/dev" in line or "COM" in line]
            self.port_combo.clear()
            self.port_combo.addItems(ports)
        except Exception as e:
            QMessageBox.critical(self, "Помилка", f"Не вдалося оновити порти:\n{e}")

    def select_file(self):
        file_path, _ = QFileDialog.getOpenFileName(self, "Виберіть INO файл", "", "INO Files (*.ino)")
        if file_path:
            self.file_label.setText(f"Обрано: {file_path}")
            self.ino_file = file_path

    def flash_arduino(self):
        try:
            ino_file = getattr(self, "ino_file", None)
            port = self.port_combo.currentText()

            board_map = {
                "Arduino: Uno": "arduino:avr:uno",
                "Arduino: Nano": "arduino:avr:nano",
                "Arduino: Mega 2560": "arduino:avr:mega",
                "Arduino: Leonardo": "arduino:avr:leonardo",
                "Arduino: Due": "arduino:sam:due",
                "Arduino: Micro": "arduino:avr:micro",
                "Arduino: Pro Mini": "arduino:avr:promini"
            }

            processor_map = {
                "atmega328": "atmega328",
                "atmega328old": "atmega328old",
                "atmega2560": "atmega2560",
                "atmega32u4": "atmega32u4",
                "sam3x8e": "sam3x8e"
            }

            board = board_map.get(self.board_combo.currentText(), "")
            processor = processor_map.get(self.processor_combo.currentText(), "")

            if not ino_file:
                QMessageBox.critical(self, "Помилка", "Будь ласка, виберіть INO файл.")
                return

            if not port:
                QMessageBox.critical(self, "Помилка", "Будь ласка, виберіть COM порт.")
                return

            if not board:
                QMessageBox.critical(self, "Помилка", "Будь ласка, виберіть плату Arduino.")
                return

            if board == "arduino:avr:nano" and processor == "":
                QMessageBox.critical(self, "Помилка", "Для Arduino Nano оберіть процесор.")
                return

            self.progress_bar.setValue(10)
            self.flash_thread = FlashThread(ino_file, port, board, processor)
            self.flash_thread.progress.connect(self.progress_bar.setValue)
            self.flash_thread.result.connect(self.flash_result)
            self.flash_thread.start()

        except Exception as e:
            QMessageBox.critical(self, "Помилка", f"Сталася помилка: {e}")

    def flash_result(self, message):
        QMessageBox.information(self, "Результат", message)
        self.progress_bar.setValue(0)

    def open_settings(self):
        settings_dialog = SettingsDialog(self)
        settings_dialog.exec_()


class SettingsDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Налаштування")
        self.setGeometry(150, 150, 400, 400)

        layout = QFormLayout()

        # Вибір шляху до Arduino IDE
        self.arduino_path_input = QLineEdit(self)
        layout.addRow("Шлях до Arduino IDE:", self.arduino_path_input)

        # Оновлення версії Arduino CLI
        self.update_cli_button = QPushButton("Оновити Arduino CLI")
        layout.addRow(self.update_cli_button)

        # Швидкість передачі (baud rate)
        self.baud_rate_input = QComboBox(self)
        self.baud_rate_input.addItems(["9600", "115200", "250000"])
        layout.addRow("Швидкість передачі:", self.baud_rate_input)

        # Зберегти налаштування
        save_button = QPushButton("Зберегти")
        save_button.clicked.connect(self.save_settings)
        layout.addWidget(save_button)

        self.setLayout(layout)

    def save_settings(self):
        # Логіка для збереження налаштувань
        QMessageBox.information(self, "Налаштування", "Налаштування збережено!")
        self.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    flasher = ArduinoFlasher()
    flasher.show()
    sys.exit(app.exec_())
