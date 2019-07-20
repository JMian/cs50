import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["userid"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # Confirm the password
        elif not request.form.get("confirmation") or request.form.get("confirmation") != request.form.get("password"):
            return apology("passwords do not match")

        # Query database for username
        newuser = db.execute("INSERT INTO users (username, hash) VALUES(:username, :pwhash)",
                             username=request.form.get("username"), pwhash=generate_password_hash(request.form.get("password")))

        # If not successfully inserted into the database after username checking
        if not newuser:
            return apology("username taken, try others")

        newusercheck = db.execute("SELECT userid FROM users WHERE username = :username",
                                  username=request.form.get("username"))

        # Set the current session for this new registered user
        session["user_id"] = newusercheck[0]["userid"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""

    # Find the same username from the table
    username = request.args.get("username")
    if len(username) > 0:
        usernamecheck = db.execute("SELECT username FROM users WHERE username = :username",
                                   username=request.args.get("username"))

        # If the username does not exist in the table
        if not usernamecheck:
            return jsonify(True)

        return jsonify(False)


@app.route("/changepw", methods=["GET", "POST"])
@login_required
def changepw():
    """Change Password"""

    if request.method == "POST":

        # Ensure old password was submitted
        if not request.form.get("pwold"):
            return apology("must provide old password")

        # Ensure new password was submitted
        elif not request.form.get("pwnew"):
            return apology("must provide new password")

        # Confirm old password
        oldpwhash = db.execute("SELECT hash FROM users WHERE userid = :userid", userid=session["user_id"])

        if not check_password_hash(oldpwhash[0]["hash"], request.form.get("pwold")):
            return apology("old password incorrect")

        # Update user's password
        updatepw = db.execute("UPDATE users SET hash = :newpw WHERE userid = :userid",
                              newpw=generate_password_hash(request.form.get("pwnew")), userid=session["user_id"])
        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("changepw.html")


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""

    # Obtain all the shares' symbols bought and sold by the user
    allshareshold = db.execute("SELECT symbol, SUM(shares) sumshares FROM transactions WHERE userid = :userid GROUP BY symbol HAVING sumshares > 0",
                               userid=session["user_id"])

    # Access the individual dictionary of all shares currently held
    allsharesinfo = []

    grandtotal = 0

    for d in allshareshold:
        symbol = d["symbol"]
        quote_result = lookup(symbol)
        sharesinfo = {"symbol": symbol, "name": quote_result["name"], "shares": d["sumshares"],
                      "price": quote_result["price"], "totalvalue": d["sumshares"] * quote_result["price"]}
        allsharesinfo.append(sharesinfo)
        grandtotal += d["sumshares"] * quote_result["price"]

    cashcheck = db.execute("SELECT cash FROM users WHERE userid = :userid", userid=session["user_id"])

    currentcash = cashcheck[0]["cash"]

    grandtotal += currentcash

    if request.method == "POST":
        transtype = request.form.get("transtype")
        shares = int(request.form.get("shares"))
        symbol = request.form.get("symbol")
        if not transtype or not shares or not symbol:
            return apology("must provide transaction information")

        quote_result = lookup(symbol)

        # Obtain the current price per share
        price = quote_result["price"]

        usercashcheck = db.execute("SELECT cash FROM users WHERE userid = :userid", userid=session["user_id"])

        usercash = usercashcheck[0]["cash"]

        totalprice = shares * price

        if transtype == "BUY":

            if totalprice > usercash:
                return apology("Insufficient cash")

            # Insert purchase information into database
            rows = db.execute("INSERT INTO transactions (userid, transtype, symbol, shares, perprice, totalprice) VALUES(:userid, 'BUY', :symbol, :shares, :perprice, :totalprice)",
                              userid=session["user_id"], symbol=symbol, shares=shares, perprice=price, totalprice=totalprice)

            # Deduct money from user's cash
            updatecash = db.execute("UPDATE users SET cash = :newcash WHERE userid = :userid",
                                    newcash=usercash-totalprice, userid=session["user_id"])

            flash(f"Successfully bought {shares} shares of {symbol}")

        if transtype == "SELL":

            # Check the total shares for this stock currently owned by the user
            sharesholdcheck = db.execute("SELECT SUM(shares) FROM transactions WHERE userid = :userid AND symbol = :symbol",
                                         userid=session["user_id"], symbol=symbol)

            shareshold = sharesholdcheck[0]["SUM(shares)"]

            # If shares held is less than the intended selling quantity
            if shareshold < shares:
                return apology("Not enough shares to sell")

            # Insert sell information into database
            rows = db.execute("INSERT INTO transactions (userid, transtype, symbol, shares, perprice, totalprice) VALUES(:userid, 'SELL', :symbol, :shares, :perprice, :totalprice)",
                              userid=session["user_id"], symbol=symbol, shares=-shares, perprice=price, totalprice=totalprice)

            # Add money to user's cash
            updatecash = db.execute("UPDATE users SET cash = :newcash WHERE userid = :userid",
                                    newcash=usercash+totalprice, userid=session["user_id"])

            flash(f"Successfully sold {shares} shares of {symbol}")

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("index.html", allshareshold=allshareshold, allsharesinfo=allsharesinfo, currentcash=currentcash, grandtotal=grandtotal)


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    allshareshold = db.execute("SELECT symbol, SUM(shares) allshares FROM transactions WHERE userid = :userid GROUP BY symbol",
                               userid=session["user_id"])

    for d in allshareshold:
        for x in d.values():
            if x == 0:
                allshareshold.remove(d)

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")

        # Ensure username was submitted
        if not symbol:
            return apology("must provide stock's symbol")

        quote_result = lookup(symbol)

        if not quote_result:
            return apology("invalid symbol")

        # Redirect user to home page
        return render_template("/quoted.html", quote_result=quote_result)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol").upper()
        quote_result = lookup(symbol)

        # Ensure username was submitted
        if not symbol or not quote_result:
            return apology("Invalid stock's symbol")

        sharestobuy = request.form.get("shares")

        # Ensure shares was submitted
        if not sharestobuy.isdigit():
            return apology("Invalid shares")

        sharestobuy = int(sharestobuy)

        if not sharestobuy > 0:
            return apology("Invalid shares")

        # Obtain the current price per share
        price = quote_result["price"]

        usercashcheck = db.execute("SELECT cash FROM users WHERE userid = :userid", userid=session["user_id"])

        usercash = usercashcheck[0]["cash"]

        totalprice = sharestobuy * price

        if totalprice > usercash:
            return apology("Insufficient cash")

        # Insert purchase information into database
        rows = db.execute("INSERT INTO transactions (userid, transtype, symbol, shares, perprice, totalprice) VALUES(:userid, 'BUY', :symbol, :shares, :perprice, :totalprice)",
                          userid=session["user_id"], symbol=symbol, shares=sharestobuy, perprice=price, totalprice=totalprice)

        # Deduct money from user's cash
        updatecash = db.execute("UPDATE users SET cash = :newcash WHERE userid = :userid",
                                newcash=usercash-totalprice, userid=session["user_id"])

        flash(f"Successfully bought {sharestobuy} shares of {symbol}")

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Obtain all the shares' symbols currently owned the user i.e. shares != 0 after all buying and selling
    allshareshold = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE userid = :userid GROUP BY symbol HAVING SUM(shares) > 0",
                               userid=session["user_id"])

    if request.method == "POST":

        symbol = request.form.get("symbol").upper()

        # If no symbol is chosen
        if not symbol:
            return apology("No symbol chosen")

        sharestosell = request.form.get("shares")

        # If no share is entered
        if not sharestosell or not sharestosell.isdigit():
            return apology("Invalid shares")

        sharestosell = int(sharestosell)

        # Check the total shares for this stock currently owned by the user
        sharesholdcheck = db.execute("SELECT SUM(shares) FROM transactions WHERE userid = :userid AND symbol = :symbol",
                                     userid=session["user_id"], symbol=symbol)

        shareshold = sharesholdcheck[0]["SUM(shares)"]

        # If shares held is less than the intended selling quantity
        if shareshold < sharestosell:
            return apology("Not enough shares to sell")

        quote_result = lookup(symbol)

        # Obtain the current price per share
        price = quote_result["price"]

        # Obtain the user's current cash
        usercashcheck = db.execute("SELECT cash FROM users WHERE userid = :userid", userid=session["user_id"])

        usercash = usercashcheck[0]["cash"]

        totalprice = sharestosell * price

        # Insert sell information into database
        rows = db.execute("INSERT INTO transactions (userid, transtype, symbol, shares, perprice, totalprice) VALUES(:userid, 'SELL', :symbol, :shares, :perprice, :totalprice)",
                          userid=session["user_id"], symbol=symbol, shares=-sharestosell, perprice=price, totalprice=totalprice)

        # Add money to user's cash
        updatecash = db.execute("UPDATE users SET cash = :newcash WHERE userid = :userid",
                                newcash=usercash+totalprice, userid=session["user_id"])

        flash(f"Successfully sold {sharestosell} shares of {symbol}")

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:

        return render_template("sell.html", allshareshold=allshareshold)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    rows = db.execute("SELECT symbol, shares, perprice, timestamp FROM transactions WHERE userid = :userid",
                      userid=session["user_id"])

    return render_template("history.html", rows=rows)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
