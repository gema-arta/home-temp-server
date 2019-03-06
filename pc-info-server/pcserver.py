## python pcserver.py <data_output.csv>
## run as:
##    python pcserver.py "../weather-server/data/pc-data-%time%.csv"

import logging
import sys
import os
import os.path
import datetime
import time
import psutil

# Setup logging.
log = logging.getLogger("temp_server")
format_cons = "%(asctime)s %(name)-12s %(levelname)8s\t%(message)s"
logging.basicConfig(level = logging.DEBUG, format = format_cons)

# Note that placeholder %time% is replaced with current
# "year-month" (for example "2018-07") for each request.
time_placeholder = "%time%"
data_file_mask = os.getcwd() + '/' + sys.argv[1]
csv_header = "DateTime,CpuTemp,MemoryPerc"

while True:
  # Create path to correct CSV file.
  dat_now = datetime.datetime.now()
  real_file = data_file_mask.replace(time_placeholder, dat_now.strftime('%Y-%m'))
  
  # If file does not exist, append the header.
  if not os.path.isfile(real_file):
    print("File %r does not exist, appending CSV header." % (real_file))
    with open(real_file, "a") as myfile:
      myfile.write(csv_header + "\r\n")
  
  # Append the data.
  formatted_data = "%s,%s,%s" % (dat_now.strftime('%Y-%m-%dT%H:%M'), str(psutil.sensors_temperatures()["coretemp"][0].current), str(psutil.virtual_memory().percent))
  with open(real_file, "a") as myfile:
    myfile.write(formatted_data + "\r\n")
  
  time.sleep(5)