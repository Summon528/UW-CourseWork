all: client server

server: server.c udp.c
	gcc server.c udp.c -o server -O2

client: mfs client.c
	gcc client.c -o client -O2 -L. -lmfs

mfs: mfs.c udp.c
	gcc -shared -o libmfs.so -fPIC mfs.c udp.c

print: print.c
	gcc print.c -o print -O2

clean:
	rm server client libmfs.so