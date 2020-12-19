# MyLinuxLearning
learning of Linux-0.11
Environment:Ubuntu 16.04 LTS,bochs 2.6.8
#How to run
download and cd FOLDER
make
#For run
bochs -q -f Qinix.bxrc
#For debug(needs two terminals)
#terminal 1
bochs -q -f QDebug.bxrc
#second terminal
cd init
gdb system
...
target remote localhost:1234

#I'm on the interrupt and Task 0 now. next step will be tasks switch and tasks shedule, then mm mudule and fork...
