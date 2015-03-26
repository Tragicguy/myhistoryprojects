DLL = libMWGateway.so.3.0.0.1
OBJ = libMWGateway.o
JDKPATH =/usr/java/j2sdk1.4.2_15/include/linux

$(DLL) : $(OBJ)
	gcc -g -I$(JDKPATH) -shared -o $(DLL) $(OBJ)

$(OBJ) : MWGateway.c MWGateway.h montnets_MWGateway.h
	gcc -fPIC -I$(JDKPATH) -g -c MWGateway.c -o $(OBJ) -lc
