import time

import telebot

# 1. Execute create_mtf.bat
# 2. Put you token into 'TOKEN' constant.
import mytoken

bot = telebot.TeleBot(mytoken.TOKEN)

import serial

COM_PORT = "COM3"
BAUD_RATE = 9600
serial_port = serial.Serial(COM_PORT, BAUD_RATE)

import cv2
from io import BytesIO

cap = cv2.VideoCapture(0)

@bot.message_handler(commands=['start'])
def start(message):
    bot.send_message(message.chat.id, "Привіт, я бот, який дозволяє керувати світлодіодною матрицею дистанційно через телеграм.")

@bot.message_handler(commands=['help'])
def help_command(message):
    bot.send_message(message.chat.id, "Використовуйте команду /set, якщо ви хочете перемкнути стан світлодіод у вказаному положенні.\nСинтаксис ціїє команти досить простий: /set 'X' 'Y' 'Status'.\nЯк ви бачете, ця команда приймає 3 параметри.\n\nПараметер 'X' позначає позицію світлодіоду по вісі x.\nПараметр 'Y' позначає позицію світлодіоду по вісі y\nПараметер 'Status' приймає 0 або 1. 0 - вимкнути, 1 - увімкнути.\n\n⚠️ Важливо ⚠️\n- Матриця має розмір 8x8\n- Координати починаються з 0.\n- X: 0 та Y: 0 - верхній лівий кут.\n\n Приклади:\nУвімкнути світлодіод (x: 1, y: 1):\n/set 1 1 1\nУвімкнути світлодіод (x: 5, y: 7):\n/set 5 7 1\nВимкнути світлодіод (x: 0, y: 0):\n/set 0 0 0\n\nCopyright © 2023 Denis Cooper. All rights reserved.")

@bot.message_handler(func=lambda message: True)
def handle_message(message):
    text = message.text
    if text.startswith("/set "):
        values_str = text[5:]
        values = values_str.split()

        if len(values) == 3:
            try:
                x = int(values[0])
                y = int(values[1])
                s = values[2]

                print(f"x: {x}, y: {y}, s: {s}")

                data_to_send = f'x{x}y{y}s{s}'
                serial_port.write(data_to_send.encode('utf-8'))
                
                time.sleep(2)

                frame = cap.read()[1]
                _, image_buffer = cv2.imencode('.jpg', frame)
                image_stream = BytesIO(image_buffer)
                bot.send_photo(message.chat.id, photo=image_stream)
            except ValueError:
                print("Invalid number format")
    elif text == "/clear":
        serial_port.write("/clear".encode('utf-8'))

bot.polling()

serial_port.close()
cap.release()
