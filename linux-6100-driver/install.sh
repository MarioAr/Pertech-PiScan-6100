#!/bin/bash
# Install script 

echo "Install Updates"
sudo apt-get update
echo "Install libusb"
sudo apt-get install libusb-1.0-0-dev
echo "Install the libraries"
sudo cp -r ./installfiles/* /usr/local/lib/
sudo chmod 755 /usr/local/lib
sudo chmod 755 /etc/ld.so.conf
sudo chmod 755 /usr/share/pixmaps
sudo chmod 755 /usr/share/applications
sudo cp -r ./6100-C-Test/pertechlogo.xpm /usr/share/pixmaps
sudo ldconfig /usr/local/lib
sudo cp -r ./6100-C-Test/gtktest /usr/bin
sudo cp -r ./6100-C-Test/pertechlogo.desktop /usr/share/applications

echo "Ready to go!"
echo "to run the example in C just go inside 6100-C-Test and execute sudo ./gtktest"
echo "Make sure gtktest has the correct permissions" 

