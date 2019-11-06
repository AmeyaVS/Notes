from flask import Flask
from flask import render_template


app = Flask(__name__)


@app.route("/<name>")
def home(name):
    return render_template("index.html", content=name, r=2)


if __name__ == "__main__":
    app.run()
