#!/bin/sh

for i in 0 1 2 3 4 5 6 7 8 9; do
  sudo usbip detach -p $i
done

for i in 1 2 3 4 5 6 7 8 9 10; do
  sudo usbip attach -r jgem-bbb -b 1-1.$i
done
