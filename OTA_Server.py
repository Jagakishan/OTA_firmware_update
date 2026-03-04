from http.server import HTTPServer, BaseHTTPRequestHandler
import os

FILE_PATH=r"C:\Users\Jagakishan\STM32CubeIDE\workspace_1.18.0\Bootloader project\App1ForBL1\Debug\App1ForBL1.bin"
HOST_IP="0.0.0.0"  #To allow any device on the network to access the server
PORT_NUMBER=8000

class ManualRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path== "/firmware":
            try:
                file_size=os.path.getsize(FILE_PATH)

                with open(FILE_PATH, "rb") as f:
                    self.send_response(200)
                    self.send_header("Content-Type", "application/octet-stream")
                    self.send_header("Content-Length", str(file_size))
                    self.end_headers()

                    while True:
                        chunk=f.read(4096)
                        if not chunk:
                            break
                        self.wfile.write(chunk)

                    print(f"Firmware sent\n")

            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                print("File opening failed\n")

            except Exception as e:
                self.send_response(500)
                self.end_headers()
                print(f"File transmission failed: {e}\n")

        else:
            self.send_response(404)
            self.end_headers()


if __name__=="__main__":
    try:
        server=HTTPServer((HOST_IP, PORT_NUMBER), ManualRequestHandler)
        print(f"Listening on Port Number: {PORT_NUMBER}\n")
        server.serve_forever()
    except KeyboardInterrupt:
        print(f"Server down\n")