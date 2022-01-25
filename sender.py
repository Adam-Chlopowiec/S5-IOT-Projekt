import subprocess
import time
import sys
import datetime
import re
from datetime import datetime


time_pattern = re.compile("[0-9][0-9]:[0-9][0-9]:[0-9][0-9]")
connect_command = ["sudo", "rfcomm", "connect", 
    "/dev/rfcomm0", "00:12:6f:9e:4c:fe"]
disconnect_command = ["sudo", "rfcomm", "release", "0"]


def clear():
    connect = subprocess.Popen(connect_command, stdout=subprocess.PIPE)
    time.sleep(4)
    with open("/dev/rfcomm0", 'w') as f:
        f.write("\r\nAT+CLS\r\n")
    time.sleep(4)
    disconnect = subprocess.Popen(disconnect_command, stdout=subprocess.PIPE)
    time.sleep(4)


def clearAndWrite(text):
    if len(text) > 18:
        text = text[:18]
    connect_command = ["sudo", "rfcomm", "connect", 
    "/dev/rfcomm0", "00:12:6f:9e:4c:fe"]
    disconnect_command = ["sudo", "rfcomm", "release", "0"]
    connect = subprocess.Popen(connect_command, stdout=subprocess.PIPE)
    time.sleep(4)
    with open("/dev/rfcomm0", 'w') as f:
        f.write("\r\nAT+CLS\r\n")
        time.sleep(4)
        f.write("\r\nAT+STR=" + text + "\r\n")
    time.sleep(2)
    disconnect = subprocess.Popen(disconnect_command, stdout=subprocess.PIPE)
    time.sleep(4)


def main():
    txt = sys.argv[1]
    date1 = sys.argv[2]
    date2 = sys.argv[3]
    if time_pattern.match(date1) and time_pattern.match(date2):
        date_now = datetime.now()
        date1 = date1.split(":")
        date2 = date2.split(":")
        date_start = datetime(date_now.year, date_now.month, date_now.day,
                    int(date1[0]), int(date1[1]), int(date1[2]))
        date_end = datetime(date_now.year, date_now.month, date_now.day,
                    int(date2[0]), int(date2[1]), int(date2[2]))
    else:
        date_start = datetime.fromisoformat(date1)
        date_end = datetime.fromisoformat(date2)

    if date_start < date_end and date_end > datetime.now():
        while True:
            now = datetime.now()
            if date_start < now < date_end:
                clearAndWrite(str(txt))
                while True:
                    if datetime.now() >= date_end:
                        clear()
                        return 0
    else:
        raise Exception("""Start date has to be lower than end date. 
        End date has to be greater than now.""")


if __name__ == "__main__":
    main()