# -*- coding: utf-8 -*-
"""
Created on Wed May 30 16:33:23 2018

@author: Chian-Yi Huang
"""

import socket
from argparse import ArgumentParser

def main():

    parser = ArgumentParser(description='TCP/IP socket')
    parser.add_argument("--ip", dest="ip", default="127.0.0.1", help="IP address, (default: %(default)s)")
    parser.add_argument("--port", dest="port", default=8700, type=int, help="port numer (default: %(default)d)")
    
    args = parser.parse_args()
    print('server-ip: %s' % args.ip)
    print('port numer: %i' % args.port)

    # Create socket
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((args.ip, args.port))
    s.listen(10)

    print ('Waiting for connection...')
    conn, addr = s.accept()

    print ('Connected by client', addr)
    while True:
        try:
            # Receive data
            data = conn.recv(200)        
            print ('Received data from client: %s' % data.decode())
            
            # Send data
            data = 'Hi, client!!'.encode()
            conn.send(data)

        except  socket.error as err_msg:
            print(err_msg)
            break

    conn.close()
    print ('Server closed.')

if __name__ == "__main__":
    main()