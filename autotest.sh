#!/usr/bin/env bash

port=$(./free-port.sh)

if [[ "$?" -ne 0 ]]; then
  echo 'no free port available'
  exit
fi

echo "free port: $port"

cd tiny || exit
./tiny "$port" &

curl localhost:"$port"

pkill tiny