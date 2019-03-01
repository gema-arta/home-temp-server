## python udpserver.py <data_output.csv> <udp_port>
## run as:
##    python udpserver.py "../weather-server/data/sample-data.csv" 12345
##    or with year placeholder which will create file-per-year
##    python udpserver.py "../weather-server/data/sample-data-%year%.csv" 12345
##
## send sample UDP packet.
##
## echo -n "hello" > /dev/udp/127.0.0.1/12345

import logging
import socket
import sys
import os
import datetime

# Setup logging.
log = logging.getLogger("temp_server")
format_cons = "%(asctime)s %(name)-12s %(levelname)8s\t%(message)s"
logging.basicConfig(level = logging.DEBUG, format = format_cons)

# Global variables.
server_port = int(sys.argv[2])

# Note that placeholder %year% is replaced with current
# year for each request if contained in argument.
year_placeholder = "%year%"
data_file_mask = os.getcwd() + '/' + sys.argv[1]

def udp_server(host = "127.0.0.1", port = server_port):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    log.info("Listening on 'udp/%s:%s'..." % (host, port))
    log.info("Will append data to %r file mask.\n" % (data_file_mask))
    
    s.bind((host, port))
    while True:
        # 16-byte buffer is enough for my needs.
        (data, addr) = s.recvfrom(16)
        yield data

for data in udp_server():   
    log.debug("Received data %r." % (data))
    
    # Format received data and append to correct CSV file.
    dat_now = datetime.datetime.now()
    real_file = data_file_mask.replace(year_placeholder, str(dat_now.year))
    formatted_data = "%s,%s" % (dat_now.strftime('%Y-%m-%dT%H:%M'), data)
    
    log.debug("Formatted data is %r." % (formatted_data))
    log.debug("Appending to %r.\n" % (real_file))
    
    with open(real_file, "a") as myfile:
        myfile.write(formatted_data + "\r\n")