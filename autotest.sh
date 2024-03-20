#!/usr/bin/env bash

pkill tiny
pkill nop-server.py

port=$(./port-for-user.pl | awk '{print $2}')

if [[ "$?" -ne 0 ]]; then
  echo 'no free port available'
  exit
fi

echo "free port: $port"

./nop-server.py $((port+1)) &

cd tiny || exit
./tiny "$port" &

curl --proxy localhost:4500 localhost:$((port+1))
# (curl --proxy localhost:4500 localhost:"$port")