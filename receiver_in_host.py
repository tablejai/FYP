from flask import Flask, request
import pyautogui
import json
import time

app = Flask(__name__)

# Using option as commmand with no mapping
ppt_command_map = [
    "option",  # "STATIC",
    "up",  # "SLIDE_UP",
    "down",  # "SLIDE_DOWN",
    "left",  # "SLIDE_LEFT",
    "right",  # "SLIDE_RIGHT",
    "b",  # "ZOOM_IN" ,
    "w",  # "ZOOM_OUT",
    "option",  # "HIGHLIGHT",
    "option",  # "ON_YES",
    "option",  # "OFF_NO",
    "option",  # "END",
]

ppt_dim = True

def ppt_control_state(command):
    pyautogui.press(command)


@app.route('/', methods=["POST"])
def receive_command():
    ppt_control_state(ppt_command_map[json.loads(request.data)["command"]])
    return 0

app.run(host='0.0.0.0', port=7000, debug=True)
