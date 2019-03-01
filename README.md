## consume-data-server
Mini Python-based server which collects temp/humidity data fromy my sensors and stores them into file.

See `udpserver.py` for info.

## weather-server
Simple one-page website which reads temp/humidity data and serves it for easy display via [Dygraphs](http://dygraphs.com).

Run with `python3 -m http.server <port>`