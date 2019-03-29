## consume-data-server
Mini Python-based server which collects temp/humidity data fromy my sensors and stores them into InfluxDB.

See `udpserver.py` for info.

## esp12_sensor
Simple C++/Arduino firmware which can be flashed into ESP8266-powered machines. This firmware reads sensor data in regular intervals and sends it to server via UDP protocol and deep-sleeps between measurements.
