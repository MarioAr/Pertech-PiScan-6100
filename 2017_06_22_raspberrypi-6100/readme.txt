Perterch Industries inc
6100 RaspberryPi driver and API
Short instruction for installing the demo

The raspberrypi-6100 directory should contain the following files:
libMICR_API.so
libPIImage.so
libPISCAN.so
gtktest
images (a directory)
readme.txt

Step 1
copy the entire directory into your PC
e.g. 
cp raspberrypi-6100 ~\Documents

Step 2
copy the .so files into /usr/local/lib
you must have admin rights to do so
e.g.
sudo cp *.so /usr/local/lib

Step 3
make sure your .so files in /usr/local/lib have the executeable flag
in case run the command
sudo chmod +x *.so

Step 4
run the command 
sudo ldconfig
this command will creates the necessary links and cache to the most recent shared libraries 
you must have admin rights to do so

Step 5 
make sure you have installed libjpeg and libtiff
run the following commands:

sudo apt-get update
sudo apt-get install libtiff5
sudo apt-get install libjpeg

Step 6
run the command inside your raspberrypi-6100 directory  (may need to add the executeable flag to gtktest to run)
sudo ./gtktest

gtktest is a sample program that you can also use a a reference for developing




