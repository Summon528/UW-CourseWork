all: client server

server: server.c
	gcc server.c udp.c -o server -O2

client: mfs client.c
	gcc client.c -o client -O2 -L. -lmfs

mfs: mfs.c
	gcc -shared -o libmfs.so -fPIC mfs.c

print: print.c
	gcc print.c udp.c -o print -O2

clean:
	rm server client libmfs.so