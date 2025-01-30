import os
import sys
import subprocess
import shutil
import hashlib
from colorama import Fore, Style, init
from termcolor import colored, cprint
from tkinter import filedialog, Tk
from pyfiglet import figlet_format

# Ініціалізація colorama
init(autoreset=True)


# Фіксований пароль для розхешовування
HASH_PASSWORD = "aXG2E6eLxzEE"

# Перевірка залежностей
def check_dependencies():
    if shutil.which("arduino-cli") is None:
        print(Fore.RED + "[Помилка] Arduino CLI не знайдено. Будь ласка, встановіть Arduino CLI.")
        sys.exit(1)

check_dependencies()

# Список портів
def list_ports():
    try:
        result = subprocess.run(["arduino-cli", "board", "list"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if result.returncode != 0:
            print(Fore.RED + f"[Помилка] Не вдалося отримати список портів: {result.stderr}")
            return []

        lines = result.stdout.split("\n")
        ports = [line.split()[0] for line in lines if "/dev" in line or "COM" in line]
        return ports
    except Exception as e:
        print(Fore.RED + f"[Помилка] Сталася помилка під час отримання портів: {e}")
        return []

# Вибір порту
def select_port():
    while True:
        ports = list_ports()
        os.system('cls' if os.name == 'nt' else 'clear')
        print(Fore.CYAN + figlet_format("Strob"))
        if not ports:
            print(Fore.YELLOW + "[Увага] Порти не знайдено.")
        else:
            print(Fore.CYAN + "Доступні порти:")
            for i, port in enumerate(ports, start=1):
                print(f"  {i}. {port}")

        print(Fore.BLUE + "  0. Оновити порти")
        print(Fore.MAGENTA + "  9. Повернутися назад")
        choice = input(Fore.CYAN + "Виберіть порт (номер): ")
        if choice == "0":
            continue
        if choice == "9":
            return None
        if choice.isdigit() and 1 <= int(choice) <= len(ports):
            return ports[int(choice) - 1]
        else:
            print(Fore.RED + "[Помилка] Некоректний вибір. Спробуйте ще раз.")

# Вибір плати
def select_board():
    boards = {
        "1": "Arduino: Nano",
        "2": "Arduino: Nano (Old Bootloader)",
        "3": "Arduino: Uno",
        "4": "Arduino: Mini",
    }

    while True:
        os.system('cls' if os.name == 'nt' else 'clear')
        print(Fore.CYAN + figlet_format("Strob"))
        print(Fore.CYAN + "Доступні плати:")
        for key, name in boards.items():
            print(f"  {key}. {name}")

        print(Fore.MAGENTA + "  9. Повернутися назад")
        choice = input(Fore.CYAN + "Виберіть плату (номер): ")
        if choice == "9":
            return None
        if choice in boards:
            return boards[choice]
        else:
            print(Fore.RED + "[Помилка] Некоректний вибір. Спробуйте ще раз.")

# Вибір процесора
def select_processor():
    processors = {
        "1": "atmega328",
        "2": "atmega328old",
        "3": "atmega168",
    }

    while True:
        os.system('cls' if os.name == 'nt' else 'clear')
        print(Fore.CYAN + figlet_format("Strob"))
        print(Fore.CYAN + "Доступні процесори:")
        for key, name in processors.items():
            print(f"  {key}. {name}")

        print(Fore.MAGENTA + "  9. Повернутися назад")
        choice = input(Fore.CYAN + "Виберіть процесор (номер): ")
        if choice == "9":
            return None
        if choice in processors:
            return processors[choice]
        else:
            print(Fore.RED + "[Помилка] Некоректний вибір. Спробуйте ще раз.")

# Вибір файлу INO
def select_file():
    os.system('cls' if os.name == 'nt' else 'clear')
    print(Fore.CYAN + figlet_format("Strob"))
    print(Fore.CYAN + "Будь ласка, виберіть INO файл.")
    root = Tk()
    root.withdraw()
    file_path = filedialog.askopenfilename(title="Виберіть INO файл", filetypes=[("INO файли", "*.ino")])
    if not file_path or not os.path.isfile(file_path):
        print(Fore.RED + "[Помилка] Файл не знайдено або не вибрано. Спробуйте ще раз.")
        return None
    return file_path

# Вибір хешованого файлу
def select_hashed_file():
    os.system('cls' if os.name == 'nt' else 'clear')
    print(Fore.CYAN + figlet_format("Strob"))
    print(Fore.CYAN + "Будь ласка, виберіть хешований файл.")
    root = Tk()
    root.withdraw()
    file_path = filedialog.askopenfilename(title="Виберіть хешований файл", filetypes=[("Хешовані файли", "*.hash")])
    if not file_path or not os.path.isfile(file_path):
        print(Fore.RED + "[Помилка] Файл не знайдено або не вибрано. Спробуйте ще раз.")
        return None
    return file_path

# Розхешовування файлу
def decrypt_file(file_path, password):
    try:
        with open(file_path, "rb") as f:
            data = f.read()

        hashed_password = hashlib.sha256(password.encode()).digest()
        decrypted_data = bytes(b ^ hashed_password[i % len(hashed_password)] for i, b in enumerate(data))

        output_file = file_path.replace(".hash", ".ino")
        with open(output_file, "wb") as f:
            f.write(decrypted_data)

        print(Fore.GREEN + f"[Успіх] Файл успішно розхешовано: {output_file}")
        return output_file
    except Exception as e:
        print(Fore.RED + f"[Помилка] Сталася помилка під час розхешовування: {e}")
        return None

# Прошивка Arduino
# Прошивка Arduino
def flash_arduino(ino_file, port, board_name, processor):
    try:
        # Перехід до папки проекту
        project_folder = os.path.dirname(ino_file)
        os.chdir(project_folder)

        fqbn_map = {
            "Arduino: Nano": "arduino:avr:nano",
            "Arduino: Nano (Old Bootloader)": "arduino:avr:nano:cpu=atmega328old",
            "Arduino: Uno": "arduino:avr:uno",
            "Arduino: Mini": "arduino:avr:mini",
        }

        fqbn = fqbn_map.get(board_name)
        if not fqbn:
            print(Fore.RED + "[Помилка] Невідома плата.")
            return

        # Підтвердження перед прошивкою
        print(Fore.YELLOW + "[Увага] Ви впевнені, що хочете прошити Arduino з такими параметрами?")
        print(Fore.CYAN + f"  Файл: {ino_file}")
        print(Fore.CYAN + f"  Порт: {port}")
        print(Fore.CYAN + f"  Плата: {board_name}")
        print(Fore.CYAN + f"  Процесор: {processor}")
        confirm = input(Fore.YELLOW + "Введіть 'y' або 'yes' для підтвердження: ").strip().lower()
        if confirm not in ["y", "yes"]:
            print(Fore.RED + "[Відміна] Прошивка скасована.")
            return

        # Компіляція
        print(Fore.GREEN + "[Інформація] Початок компіляції...")
        compile_command = ["arduino-cli", "compile", "--fqbn", fqbn, ino_file]
        compile_result = subprocess.run(compile_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        

        if compile_result.returncode != 0:
            print(Fore.RED + "[Помилка] Не вдалося скомпілювати файл.")
            print(Fore.RED + f"Деталі помилки: {compile_result.stderr}")
            input()
            return

        print(Fore.GREEN + "[Успіх] Компіляція завершена успішно.")

        # Завантаження прошивки
        print(Fore.GREEN + "[Інформація] Початок завантаження...")
        upload_command = ["arduino-cli", "upload", "--fqbn", fqbn, "-p", port, ino_file]
        upload_result = subprocess.run(upload_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        if upload_result.returncode != 0:
            print(Fore.RED + "[Помилка] Не вдалося завантажити прошивку.")
            print(Fore.RED + f"Деталі помилки: {upload_result.stderr}")
            input()
        else:
            print(Fore.GREEN + "[Успіх] Arduino успішно прошито!")
            input(Fore.BLUE + "Нажміть на любу кнопку...")
    except Exception as e:
        print(Fore.RED + f"[Помилка] Сталася помилка: {e}")
        input()


# Головне меню
def main():
    while True:
        os.system('cls' if os.name == 'nt' else 'clear')
        # print(Fore.CYAN + figlet_format("Strob"))
        print((colored(figlet_format("Strob"), color="blue")))
        print(Fore.CYAN + "1. Вибрати INO файл")
        print(Fore.CYAN + "2. Розхешувати файл")
        print(Fore.CYAN + "0. Вийти")
        choice = input(Fore.CYAN + "Ваш вибір: ")
        if choice == "1":
            ino_file = None
            while not ino_file:
                ino_file = select_file()
                if ino_file is None:
                    return

            port = None
            while not port:
                port = select_port()
                if port is None:
                    break

            if port is None:
                continue

            board = select_board()
            if board is None:
                continue

            processor = select_processor()
            if processor is None:
                continue

            flash_arduino(ino_file, port, board, processor)

        elif choice == "2":
            hashed_file = select_hashed_file()
            if not hashed_file:
                continue

            decrypted_file = decrypt_file(hashed_file, HASH_PASSWORD)
            if decrypted_file:
                print(Fore.GREEN + f"[Інформація] Файл {decrypted_file} готовий для прошивки.")
                port = None
                while not port:
                    port = select_port()
                    if port is None:
                        break

                if port is None:
                    continue

                board = select_board()
                if board is None:
                    continue

                processor = select_processor()
                if processor is None:
                    continue

                # Прошивка після компіляції
                flash_arduino(decrypted_file, port, board, processor)

        elif choice == "0":
            print(Fore.CYAN + "[Інформація] Вихід...")
            sys.exit(0)
        else:
            print(Fore.RED + "[Помилка] Некоректний вибір. Спробуйте ще раз.")

if __name__ == "__main__":
    main()
