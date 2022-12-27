from flask import Flask, render_template
app = Flask(__name__)

@app.route('/')
def home():
    return render_template('home.html')

@app.route('/my-link/')
def my_link():
    print('I got clicked')

    return 'Click'

if __name__ == '__main__':
    app.run(debug=True)