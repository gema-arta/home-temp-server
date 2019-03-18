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

while True:
  sens_temp = psutil.sensors_temperatures()["coretemp"][0].current
  sens_mem = psutil.virtual_memory().percent
 
  # Submit the data.
  formatted_data = "influx -database 'home_data' -execute 'INSERT server temp={},mem_perc={}'".format(sens_temp, sens_mem)

  print(formatted_data)

  os.system(formatted_data)
  
  time.sleep(30)