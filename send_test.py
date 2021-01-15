import zmq
import time

url = 'tcp://127.0.0.1:6600'

context = zmq.Context()
socket = context.socket(zmq.REQ)
# socket.connect('tcp://192.168.1.223:6600')
socket.connect(url)

print("Connect to ", url)
while 1:

    ####LEFT####
    socket.send(b'{"cmd": "LEFT", "arg": 240}')
    # msg = b"hello"
    # socket.send(msg)
    json_str = socket.recv()
    print(json_str)
    time.sleep(1)

    socket.send(b'{"cmd": "LEFT", "arg": 0}')
    json_str = socket.recv()
    print(json_str)
    time.sleep(1)

    ####RIGHT####
    socket.send(b'{"cmd": "RIGHT", "arg": 240}')
    json_str = socket.recv()
    print(json_str)
    time.sleep(1)

    socket.send(b'{"cmd": "RIGHT", "arg": 0}')
    json_str = socket.recv()
    print(json_str)
    time.sleep(1)

    ####UP####
    socket.send(b'{"cmd": "UP", "arg": 240}')
    json_str = socket.recv()
    print(json_str)
    time.sleep(1)

    socket.send(b'{"cmd": "UP", "arg": 0}')
    json_str = socket.recv()
    print(json_str)
    time.sleep(1)

    ####DOWN####
    socket.send(b'{"cmd": "DOWN", "arg": 240}')
    json_str = socket.recv()
    print(json_str)
    time.sleep(1)
    socket.send(b'{"cmd": "DOWN", "arg": 0}')
    json_str = socket.recv()
    print(json_str)
    time.sleep(1)
