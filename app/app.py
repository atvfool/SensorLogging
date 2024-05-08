import mysql.connector
import os
from flask import Flask, request

app = Flask(__name__)

def get_connection_creds():
    return dict(host=os.environ["SENSOR_SERVER"],
                user=os.environ["SENSOR_USERNAME"],
                password=os.environ["SENSOR_PASSWORD"],
                database=os.environ["SENSOR_DATABASE"]);

def save_log(sensor_id, message, token):
    try:
        creds = get_connection_creds()
        with mysql.connector.MySQLConnection(host=creds['host'], 
                                             user=creds['user'], 
                                             password=creds['password'], 
                                             database=creds['database']) as mydb:
            with mydb.cursor() as mycursor: 
                args = (sensor_id, message, token)
                resultargs = mycursor.callproc('spSaveLog', args)
                mydb.commit()
                print(resultargs)
                mycursor.close()
            mydb.close()

    except mysql.connector.Error as error:
        print("failed to execute stored procedure: {}".format(error))

@app.route('/')
def index():
    return 'Hello World!'

@app.route('/savelog', methods=['POST'])
def savelogpost():
    sensor_id=request.form['sensor_id']
    message=request.form['message']
    token=request.form['token']
    save_log(sensor_id, message, token)
    return 'Saved'

if __name__ == '__main__':
    try:
        app.run(host="0.0.0.0", port=8000)
    except mysql.connector.Error as error:
        print("mysql main error {}".format(error))