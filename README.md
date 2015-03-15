web-server
==========
<<<<<<< HEAD
=======

C/S Modle

This is a simple introduce to the C/S modle, which can conmunicate with
each clients and servers. This is directroy contains two directeoies and
two files.

There are client directroy and server directroy and Makefile and readme file.

The client directroy contains client codes, which manage the client. And
make the client safety and quuickly to get information from servers.

The server directroy contains server codes, and manage the servers. Every
clients can visit the server to get information, and the servers can quickly
make response to clients.

The Makefile can use command make to compile the whole codes and get binary
codes to run the system.

About this file is a introduce of currrent directroy.

You should use command to make start this server.

When you compile the sources codes, you'd better run <$make clean> to
clean the rest unuseful files.

Compiler:
	$ make
start|stop:
	$ server.sh start[|restart|stop]
browser:
	input host name 192.168.1.111:32766[ip:port]
    or
	192.168.1.111:32766/www/filename
All the web pages can run correctly, except the php web pages.
>>>>>>> origin/master
