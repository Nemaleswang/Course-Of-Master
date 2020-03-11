object = dofifochild.o dofifoparent.o parentchildfifo.o \
		 parentwritepipe.o pipeclient.o pipeserver.o \
		 seqserverbad.o simpleredirect.o synchronizefan.o restart.o


target = parentwritepipe synchronizefan simpleredirect \
	parentchildfifo pipeserver pipeclient seqserverbad

all: ${target}

.PHONY:
	all

parentwritepipe: parentwritepipe.o
	$(CC) $(CFLAGS) $^ -o $@

synchronizefan: synchronizefan.o restart.o
	$(CC) $(CFLAGS) $^ -o $@

simpleredirect: simpleredirect.o
	$(CC) $(CFLAGS) $^ -o $@

parentchildfifo: parentchildfifo.o dofifochild.o dofifoparent.o restart.o
	$(CC) $(CFLAGS) $^ -o $@

pipeserver: pipeserver.o restart.o
	$(CC) $(CFLAGS) $^ -o $@

pipeclient: pipeclient.o restart.o
	$(CC) $(CFLAGS) $^ -o $@

seqserverbad: seqserverbad.o restart.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm ${object}
	-rm ${target}
