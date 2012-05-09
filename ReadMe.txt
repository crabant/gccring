1.how to install
	1)get the gccring source code to target linux machine
	2)make
	3)sudo make install
2.show help usage
	gccring help
3.collect the source files information.
  assume use arm-hismall-linux-g++ as compiler
  1)create the compiler gccring alias
    sudo gccring compiler add arm-hismall-linux-g++
  2)enable gccring
    gccring enable
    running the export PATH=... as 'gccring enable' output
  3)rebuild the target project
    make clean
    make
  4)check the file list
    check /tmp/gccring/orilist.txt
  5)check the new file list
    gccring will copy all built files to the new location which is specified in /usr/local/gccring/cfg/gccring.cfg
    the new location is /tmp/gccring,all files is put there
    the new files list is /tmp/gccring/orilist.txt
4.gccring configuration
    /usr/local/gccring/cfg/gccring.cfg