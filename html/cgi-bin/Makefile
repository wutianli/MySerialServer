CC   = powerpc-linux-gnu-gcc

LIBS = -L./ -lcgic
PATH = -Wl,-rpath,/home/ubuntu/ltib-mpc8313erdb-20081222/rootfs/usr/lib/
CFLAG    = -O3 -Wall

.c.o:
	$(CC) $(CFLAG) -c $<

all:	
	$(CC) -o port1.cgi			serial_port.c     $(LIBS) $(PATH)
	$(CC) -o port2.cgi			serial_port2.c    $(LIBS) $(PATH)
	$(CC) -o port3.cgi			serial_port3.c    $(LIBS) $(PATH)
	$(CC) -o port4.cgi			serial_port4.c    $(LIBS) $(PATH)
	$(CC) -o port5.cgi			serial_port5.c    $(LIBS) $(PATH)
	$(CC) -o port6.cgi			serial_port6.c    $(LIBS) $(PATH)
	$(CC) -o port7.cgi			serial_port7.c    $(LIBS) $(PATH)
	$(CC) -o port8.cgi			serial_port8.c    $(LIBS) $(PATH)
	$(CC) -o VersionAnalysis.cgi  VersionAnalysis.c $(LIBS) $(PATH)
	$(CC) -o safe.cgi				safe.c            $(LIBS) $(PATH)
	$(CC) -o user_list.cgi		user_list.c       $(LIBS) $(PATH)
	$(CC) -o landing.cgi			landing.c 		  $(LIBS) $(PATH)
	$(CC) -o mod_landing.cgi		mod_landing.c     $(LIBS) $(PATH)
	$(CC) -o SysReboot.cgi		SysReboot.c		  $(LIBS) $(PATH)
	$(CC) -o client.cgi  			client.c          $(LIBS) $(PATH)
	$(CC) -o recover.cgi			recover.c		  $(LIBS) $(PATH)
clean:
	rm -rf *.o
	rm -rf *.cgi
	rm -rf *.c

