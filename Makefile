CFLAGS := -O2

all: ips ips-make
ips: ips.o libips.o common.o
	gcc $(CFLAGS) common.o libips.o ips.o -o ips

ips.o: ips.c
	gcc $(CFLAGS) ips.c -c

ips-make: ips-make.o libips.o common.o
	gcc $(CFLAGS) libips.o common.o ips-make.o -o ips-make

ips-make.o: ips-make.c
	gcc $(CFLAGS) ips-make.c -c

libips.o: libips.c libips.h
	gcc $(CFLAGS) -c libips.c

common.o: common.c common.h
	gcc $(CFLAGS) -c common.c
