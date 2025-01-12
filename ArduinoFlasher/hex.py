import os
import hashlib
import subprocess
import random
import string
from PyQt5 import QtWidgets, QtGui
from PyQt5.QtWidgets import QFileDialog, QMessageBox, QLineEdit, QSpinBox

class ArduinoHasherApp(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Arduino Hasher and Compiler")
        self.setGeometry(200, 200, 600, 450)

        # Layout and Widgets
        self.layout = QtWidgets.QVBoxLayout()

        self.file_label = QtWidgets.QLabel("Вибраний файл: Немає")
        self.layout.addWidget(self.file_label)

        self.select_file_button = QtWidgets.QPushButton("Вибрати INO файл")
        self.select_file_button.clicked.connect(self.select_file)
        self.layout.addWidget(self.select_file_button)

        self.password_label = QtWidgets.QLabel("Пароль для хешу:")
        self.layout.addWidget(self.password_label)

        self.password_input = QLineEdit()
        self.password_input.setEchoMode(QLineEdit.Normal)  # Пароль видно
        self.layout.addWidget(self.password_input)

        self.password_length_label = QtWidgets.QLabel("Довжина пароля:")
        self.layout.addWidget(self.password_length_label)

        # Spinbox to set password length
        self.password_length_spinbox = QSpinBox()
        self.password_length_spinbox.setRange(6, 64)  # Range from 6 to 64 characters
        self.password_length_spinbox.setValue(12)  # Default length is 12
        self.layout.addWidget(self.password_length_spinbox)

        self.generate_password_button = QtWidgets.QPushButton("Згенерувати пароль")
        self.generate_password_button.clicked.connect(self.generate_password)
        self.layout.addWidget(self.generate_password_button)

        self.hash_button = QtWidgets.QPushButton("Згенерувати хеш та скомпілювати")
        self.hash_button.clicked.connect(self.generate_hash_and_compile)
        self.hash_button.setEnabled(False)
        self.layout.addWidget(self.hash_button)

        self.output_text = QtWidgets.QTextEdit()
        self.output_text.setReadOnly(True)
        self.layout.addWidget(self.output_text)

        # Central Widget
        central_widget = QtWidgets.QWidget()
        central_widget.setLayout(self.layout)
        self.setCentralWidget(central_widget)

        # File path and Arduino CLI
        self.file_path = None
        self.arduino_cli_path = "arduino-cli"  # Ensure arduino-cli is installed and in PATH

    def select_file(self):
        file_dialog = QFileDialog()
        file_path, _ = file_dialog.getOpenFileName(self, "Вибрати INO файл", "", "Arduino Files (*.ino)")
        if file_path:
            self.file_path = file_path
            self.file_label.setText(f"Вибраний файл: {os.path.basename(file_path)}")
            self.hash_button.setEnabled(True)

    def generate_password(self):
        # Get password length from the spinbox
        password_length = self.password_length_spinbox.value()
        
        # Generate random password with the specified length
        password = ''.join(random.choices(string.ascii_letters + string.digits, k=password_length))
        
        # Set the generated password to the password input field
        self.password_input.setText(password)

    def generate_hash_and_compile(self):
        if not self.file_path:
            QMessageBox.critical(self, "Помилка", "Будь ласка, виберіть файл перед хешуванням.")
            return

        # Get the password for the hash (if any)
        password = self.password_input.text()

        try:
            hasher = hashlib.sha256()
            with open(self.file_path, "rb") as file:
                while chunk := file.read(8192):
                    hasher.update(chunk)

            # Append password to the hash if provided
            if password:
                hasher.update(password.encode('utf-8'))

            hash_value = hasher.hexdigest()

            # Let the user select where to save the hash file
            hash_file_path, _ = QFileDialog.getSaveFileName(self, "Зберегти хеш файл", os.path.splitext(self.file_path)[0] + ".sha256", "SHA256 Files (*.sha256)")
            if not hash_file_path:
                return

            with open(hash_file_path, "w") as hash_file:
                hash_file.write(hash_value)

            self.output_text.append(f"Хеш згенеровано: {hash_value}")
            self.output_text.append(f"Збережено у файл: {hash_file_path}\n")

            # Automatically compile the file after generating the hash
            self.compile_file()

        except Exception as e:
            QMessageBox.critical(self, "Помилка", f"Помилка під час хешування: {e}")

    def compile_file(self):
        if not self.file_path:
            QMessageBox.critical(self, "Помилка", "Будь ласка, виберіть файл перед компіляцією.")
            return

        try:
            project_folder = os.path.dirname(self.file_path)
            os.chdir(project_folder)

            # Run Arduino CLI compile command
            fqbn = "arduino:avr:uno"  # Default board, customizable
            compile_command = [self.arduino_cli_path, "compile", "--fqbn", fqbn, self.file_path]
            result = subprocess.run(compile_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

            if result.returncode == 0:
                self.output_text.append("Файл успішно скомпільовано!")
                self.output_text.append(result.stdout)
            else:
                self.output_text.append("Помилка компіляції:")
                self.output_text.append(result.stderr)

        except Exception as e:
            QMessageBox.critical(self, "Помилка", f"Помилка під час компіляції: {e}")

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = ArduinoHasherApp()
    window.show()
    sys.exit(app.exec_())
