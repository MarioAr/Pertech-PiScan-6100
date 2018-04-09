Perterch Industries inc
6100 Linux driver and API
Short instruction for installing the demo

The linux-6100-xxbit directory should contain the following files:
libMICR_API.so
libPIImage.so
libPISCAN.so
gtktest
images (a directory)
readme.txt

Step 1
copy the entire directory into your PC
e.g. 
copy linux-6100-32bit ~\Documents

Step 2
copy the .so files into /usr/local/lib
you must have admin rights to do so
e.g.
sudo copy *.so /usr/local/lib

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
they should be already installed in a normal ubuntu 16.04 release
but in case you miss them 
run the following commands:

sudo apt-get update
sudo apt-get install libtiff5
sudo apt-get install libjpeg

Step 6
run the command inside your linux-6100-xxbit directory 
sudo ./gtktest

gtktest is a sample program that you can also use a a reference for developping


