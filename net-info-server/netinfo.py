import subprocess
import os

while True:
  result = subprocess.run(['speedtest-cli', '--csv'], stdout=subprocess.PIPE, text=True)
  
  if result.returncode == 0:
    print("Obtained speedtest data is: '%s'." % (result.stdout))

    # Parse the data.
    parsed_result = result.stdout.replace("\n", "")
    parsed_list = parsed_result.split(",")

    ip_addr = parsed_list[9]
    download = parsed_list[6]
    upload = parsed_list[7]
    ping = parsed_list[5]

    print("IP address: %s\nDownload: %s\nUpload: %s\nPing: %s" % (ip_addr, download, upload, ping))

    # Send data to database.
    db_command = "influx -database 'home_data' -execute 'INSERT netspeed,ipaddr=\"{}\" download={},upload={},ping={}'".format(ip_addr, download, upload, ping) 

    os.system(db_command)
  else:
    print("Error running 'speedtest-cli'.")