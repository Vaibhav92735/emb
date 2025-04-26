import serial
import time

# === Configuration ===
PORT = '/dev/ttyUSB0'
BAUD_RATE = 115200
OUTPUT_FILE = 'result.raw'
CHUNK_SIZE = 1024
TIMEOUT = 1.0


def main():
    try:
        ser = serial.Serial(PORT, BAUD_RATE, timeout=TIMEOUT)
        time.sleep(2)
        print(f"[*] Connected to {PORT} at {BAUD_RATE} baud")
    except Exception as e:
        print(f"[!] Failed to open serial port: {e}")
        return

    try:
        with open(OUTPUT_FILE, 'wb') as out:
            print(f"[*] Writing raw data to: {OUTPUT_FILE}")
            while True:
                data = ser.read(CHUNK_SIZE)
                if data:
                    out.write(data)
    except KeyboardInterrupt:
        print("\n[!] Capture stopped by user.")
    except Exception as e:
        print(f"[!] I/O error: {e}")
    finally:
        ser.close()
        print("[*] Serial port closed. File saved.")


if __name__ == '__main__':
    main()
