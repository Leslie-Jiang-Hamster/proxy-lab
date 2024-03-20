#!/usr/bin/env bash

pkill tiny

port=$(./port-for-user.pl | awk '{print $2}')

if [[ "$?" -ne 0 ]]; then
  echo 'no free port available'
  exit
fi

echo "free port: $port $((port+1))"

cd tiny || exit
./tiny "$((port+1))" &

curl --proxy localhost:4500 localhost:"$((port+1))"