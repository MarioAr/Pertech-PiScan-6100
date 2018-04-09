#!/bin/bash
# Install script 

echo "Install java"
sudo apt-get update
sudo apt-get install openjdk-7-jre
echo "Install the libraries"
sudo cp *.so /usr/local/lib
sudo ldconfig /usr/local/lib
echo "Install libusb"
sudo apt-get install libusb-1.0-0-dev

echo "Ready to go!"
echo "to run the example just execute sudo ./example.sh"

