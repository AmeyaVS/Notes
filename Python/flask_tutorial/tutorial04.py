from flask import Flask
from flask import render_template
from flask import redirect
from flask import url_for
from flask import request


app = Flask(__name__)


@app.route("/")
def home():
    return render_template("derived.html")


@app.route("/login", methods=["POST", "GET"])
def login():
    if request.method == "POST":
        user = request.form['nm']
        return redirect(url_for("user", usr=user))
    else:
        return render_template("login.html")


@app.route("/<usr>")
def user(usr):
    return f"<h1>{usr}</h1>"


if __name__ == "__main__":
    app.run(debug=True)
