import os
import hashlib
import subprocess
import random
import string
from PyQt5 import QtWidgets, QtGui
from PyQt5.QtWidgets import QFileDialog, QMessageBox, QLineEdit, QSpinBox
from PyQt5.QtGui import QColor

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
        self.select_file_button.setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;")
        self.select_file_button.clicked.connect(self.select_file)
        self.layout.addWidget(self.select_file_button)

        self.password_label = QtWidgets.QLabel("Пароль для шифрування:")
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
        self.generate_password_button.setStyleSheet("background-color: #2196F3; color: white; font-weight: bold;")
        self.generate_password_button.clicked.connect(self.generate_password)
        self.layout.addWidget(self.generate_password_button)

        self.standard_password_button = QtWidgets.QPushButton("Вставити стандартний пароль")
        self.standard_password_button.setStyleSheet("background-color: #FF5722; color: white; font-weight: bold;")
        self.standard_password_button.clicked.connect(self.insert_standard_password)
        self.layout.addWidget(self.standard_password_button)

        self.encrypt_button = QtWidgets.QPushButton("Зашифрувати файл")
        self.encrypt_button.setStyleSheet("background-color: #9C27B0; color: white; font-weight: bold;")
        self.encrypt_button.clicked.connect(self.encrypt_file)
        self.encrypt_button.setEnabled(False)
        self.layout.addWidget(self.encrypt_button)

        self.output_text = QtWidgets.QTextEdit()
        self.output_text.setReadOnly(True)
        self.layout.addWidget(self.output_text)

        # Central Widget
        central_widget = QtWidgets.QWidget()
        central_widget.setLayout(self.layout)
        self.setCentralWidget(central_widget)

        # File path
        self.file_path = None

    def select_file(self):
        file_dialog = QFileDialog()
        file_path, _ = file_dialog.getOpenFileName(self, "Вибрати INO файл", "", "Arduino Files (*.ino)")
        if file_path:
            self.file_path = file_path
            self.file_label.setText(f"Вибраний файл: {os.path.basename(file_path)}")
            self.encrypt_button.setEnabled(True)

    def generate_password(self):
        # Get password length from the spinbox
        password_length = self.password_length_spinbox.value()
        
        # Generate random password with the specified length
        password = ''.join(random.choices(string.ascii_letters + string.digits, k=password_length))
        
        # Set the generated password to the password input field
        self.password_input.setText(password)

    def insert_standard_password(self):
        standard_password = "aXG2E6eLxzEE"
        self.password_input.setText(standard_password)

    def encrypt_file(self):
        if not self.file_path:
            QMessageBox.critical(self, "Помилка", "Будь ласка, виберіть файл перед шифруванням.")
            return

        password = self.password_input.text()
        if not password:
            QMessageBox.critical(self, "Помилка", "Будь ласка, введіть пароль для шифрування.")
            return

        try:
            with open(self.file_path, "rb") as file:
                data = file.read()

            hashed_password = hashlib.sha256(password.encode()).digest()
            encrypted_data = bytes(b ^ hashed_password[i % len(hashed_password)] for i, b in enumerate(data))

            # Save encrypted file
            encrypted_file_path, _ = QFileDialog.getSaveFileName(self, "Зберегти зашифрований файл", os.path.splitext(self.file_path)[0] + ".hash", "Hash Files (*.hash)")
            if not encrypted_file_path:
                return

            with open(encrypted_file_path, "wb") as encrypted_file:
                encrypted_file.write(encrypted_data)

            self.output_text.append(f"Файл успішно зашифровано та збережено у: {encrypted_file_path}")
            self.output_text.append(f"Пароль для розшифрування: {password}\n")

        except Exception as e:
            QMessageBox.critical(self, "Помилка", f"Помилка під час шифрування: {e}")

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = ArduinoHasherApp()
    window.show()
    sys.exit(app.exec_())
