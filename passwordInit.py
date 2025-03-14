import serial
import time

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=1)
time.sleep(2)

def send_number(number):
    if 0 <= number <= 9999:
        arduino.write(f"{number}\n".encode())
        print(f"Sent: {number}")
    else:
        print("Invalid number")

