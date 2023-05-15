import mysql.connector
from datetime import datetime, timedelta
import random


# Connect to the database
db = mysql.connector.connect(
    host="localhost",
    user="luthfipratamaps",
    password="Alsin12354!",
    database="loraiot"
)
cursor = db.cursor()

# Generate the data
start_time = datetime.strptime('00:00:00', '%H:%M:%S')
end_time = datetime.strptime('23:59:59', '%H:%M:%S')
delta_time = timedelta(minutes=1)
current_time = start_time

today = datetime.now()
days = []
for i in range(7):
    date = today - timedelta(days=i)
    date_string = date.strftime('%Y-%m-%d')
    days.append(date_string)

Lats = [-6.9174639, -6.9174640, -6.9174641, -6.9174642]
Longs = [107.6191228, 107.6191217, 107.6191206, 107.6191195]

for day in days:
    while current_time <= end_time:
        # Generate random values for Suhu, RH, and IC
        Suhu1 = round(random.uniform(20, 30), 2)
        Suhu2 = round(random.uniform(20, 30), 2)
        Suhu3 = round(random.uniform(20, 30), 2)
        Suhu4 = round(random.uniform(20, 30), 2)
        RH1 = round(random.uniform(40, 60), 2)
        RH2 = round(random.uniform(40, 60), 2)
        RH3 = round(random.uniform(40, 60), 2)
        RH4 = round(random.uniform(40, 60), 2)
        SH1 = round(random.uniform(40, 60), 2)
        SH2 = round(random.uniform(40, 60), 2)
        SH3 = round(random.uniform(40, 60), 2)
        SH4 = round(random.uniform(40, 60), 2)
        IC1 = round(random.uniform(100, 1000), 2)
        IC2 = round(random.uniform(100, 1000), 2)
        IC3 = round(random.uniform(100, 1000), 2)
        IC4 = round(random.uniform(100, 1000), 2)
        Latitude1 = Lats[0]
        Latitude2 = Lats[1]
        Latitude3 = Lats[2]
        Latitude4 = Lats[3]
        Longitude1 = Longs[0]
        Longitude2 = Longs[1]
        Longitude3 = Longs[2]
        Longitude4 = Longs[3]
        
        # Format the date and time strings
        date_string = day
        time_string = current_time.strftime('%H:%M:%S')

        # Insert the data into the table
        sql = "INSERT INTO monitoring_data (Tanggal, Waktu, Suhu1, Suhu2, Suhu3, Suhu4, RH1, RH2, RH3, RH4, SH1, SH2, SH3, SH4, IC1, IC2, IC3, IC4, Latitude1, Latitude2, Latitude3, Latitude4, Longitude1, Longitude2, Longitude3, Longitude4) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"
        val = (date_string, time_string, Suhu1, Suhu2, Suhu3, Suhu4, RH1, RH2, RH3, RH4, SH1, SH2, SH3, SH4, IC1, IC2, IC3, IC4, Latitude1, Latitude2, Latitude3, Latitude4, Longitude1, Longitude2, Longitude3, Longitude4)
        print(val)
        cursor.execute(sql, val)
        
        # Increment the time by delta_time
        current_time += delta_time

    current_time = datetime.strptime('00:00:00', '%H:%M:%S')

# Commit the changes and close the connection
db.commit()
db.close()
