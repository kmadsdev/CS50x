import os
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session['user_id']
    cash = db.execute('SELECT cash FROM users WHERE id = ?', user_id)[0]['cash']
    rows = db.execute('''
        SELECT symbol, SUM(shares) AS shares
            FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING shares > 0
    ''', user_id)

    portfolio = []
    total = cash

    for row in rows:
        quote = lookup(row['symbol'])
        price = quote['price']
        value = price * row['shares']
        portfolio.append({
            'symbol': row['symbol'],
            'name': quote['name'],
            'shares': row['shares'],
            'price': price,
            'total': value
        })
        total += value

    return render_template('index.html', portfolio=portfolio, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == 'POST':
        symbol = request.form.get('symbol').upper()
        shares = request.form.get('shares')
        if not symbol:
            return apology('must provide symbol', 400)
        if not shares.isdigit() or int(shares) <= 0:
            return apology('invalid share count', 400)
        shares = int(shares)
        quote = lookup(symbol)
        if quote is None:
            return apology('invalid symbol', 400)
        price = quote['price']
        cost = price * shares
        cash = db.execute('SELECT cash FROM users WHERE id = ?', session['user_id'])[0]['cash']
        if cost > cash:
            return apology('can\'t afford it! :(', 400)
        db.execute(
            'INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)',
            session['user_id'], symbol, shares, price
        )
        db.execute(
            'UPDATE users SET cash = cash - ? WHERE id = ?',
            cost, session['user_id']
        )
        return redirect('/')
    return render_template('buy.html')



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session['user_id']
    return render_template('history.html', transactions=db.execute('''
        SELECT symbol, shares, price, time
            FROM transactions
        WHERE user_id = ?
        ORDER BY time DESC
        ''', user_id)
    )


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == 'POST':
        symbol = request.form.get('symbol')
        if not symbol:
            return apology('must provide symbol')
        if not lookup(symbol):
            return apology('Invalid symbol')

        return render_template('quoted.html', quote=quote)

    return render_template('quote.html')


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == 'POST':

        username = request.form.get('username')
        password = request.form.get('password')
        confirmation = request.form.get('confirmation')

        if not username or not password or not confirmation:
            return apology('must provide username and password', 400)
        if password != confirmation:
            return apology('passwords do not match', 400)

        try:
            new_id = db.execute('INSERT INTO users (username, hash) VALUES (?, ?)', username, generate_password_hash(password),)
        except:
            return apology('username already exists', 400)

        session['user_id'] = new_id
        return redirect('/')
    return render_template('register.html')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session['user_id']
    symbols = db.execute('''
        SELECT symbol
            FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING SUM(shares) > 0
    ''', user_id)

    if request.method == "POST":
        symbol = request.form.get('symbol')
        shares = request.form.get('shares')

        if not symbol or not shares or not shares.isdigit() or int(shares) <= 0:
            return apology('invalid input')

        shares = int(shares)
        owned = db.execute('''
            SELECT SUM(shares) as shares
            FROM transactions
            WHERE user_id = ? AND symbol = ?
        ''', user_id, symbol)[0]['shares']

        if shares > owned:
            return apology('Your don\'t own that many shares! :(')

        quote = lookup(symbol)
        revenue = shares * quote['price']

        db.execute('UPDATE users SET cash = cash + ? WHERE id = ?', revenue, user_id)
        db.execute('''
            INSERT INTO transactions (user_id, symbol, shares, price)
            VALUES (?, ?, ?, ?)
        ''', user_id, symbol, -shares, quote['price'])

        return redirect('/')

    return render_template('sell.html', symbols=[row['symbol'] for row in symbols])
