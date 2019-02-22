## send sample UDP packet.
##
## echo -n "hello" >/dev/udp/127.0.0.1/12345

import logging
import socket

log = logging.getLogger('udp_server')


def udp_server(host='127.0.0.1', port=12345):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    log.info("Listening on udp %s:%s" % (host, port))
    s.bind((host, port))
    while True:
        # 16-byte buffer is enough for my needs.
        (data, addr) = s.recvfrom(16)
        yield data


FORMAT_CONS = '%(asctime)s %(name)-12s %(levelname)8s\t%(message)s'
logging.basicConfig(level=logging.DEBUG, format=FORMAT_CONS)

for data in udp_server():
    # TODO: Append data to "temperature" file/DB/etc.
    log.debug("%r" % (data,))