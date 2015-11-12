export MACOSX_DEPLOYMENT_TARGET=10.9
gcc -Wall  -I/Library/Frameworks/Tcl.framework/Headers/ -I/opt/X11/include/ -ltcl -ltk hello2.c -o hello2; chmod +x hello2
