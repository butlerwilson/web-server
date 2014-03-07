#!/bin/bash
#
# start server and stop server
#

if [ "$1" = "restart" ] || [ "$1" = "start" ]; then
        ./bin/server
elif [ "$1" = "stop" ]; then
        killall ./bin/server
else
        echo "Usage:server.sh estart[start | stop]"
fi

exit 0

