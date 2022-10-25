from flask import Flask, redirect, url_for, request, jsonify
app = Flask(__name__)


@app.route("/hello", methods=['POST'])
def hello_world():
    data = request.json
    print(data)
    return "Request Received"


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5001, debug=False)
