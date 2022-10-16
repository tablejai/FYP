from flask import Flask, redirect, url_for, request
app = Flask(__name__)


@app.route("/hello", methods=['POST'])
def hello_world():
    return "What lun the fuck"


if __name__ == '__main__':
    app.run(debug=True)
