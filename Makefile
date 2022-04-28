default: c

c: gameClient gameServer # gameServer gameClient

gameClient: gameClient.c
	gcc gameClient.c -o gameClient1
	gcc gameClient.c -o gameClient2

gameServer: gameServer.c
	gcc gameServer.c -o gameServer

clean:
	rm -f gameClient1 gameClient2 gameServer
