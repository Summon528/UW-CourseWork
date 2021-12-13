all: client server libmfs.so

server: server.c udp.c udp.h proto.h
	gcc server.c udp.c -o server -O2

client: libmfs.so client.c
	gcc client.c -o client -O2 -L. -lmfs

libmfs.so: mfs.c udp.c udp.h proto.h
	gcc -shared -o libmfs.so -fPIC mfs.c udp.c

print: print.c
	gcc print.c -o print -O2

clean:
	rm server client libmfs.so