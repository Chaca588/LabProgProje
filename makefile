static:
	gcc -Wall -c player.c
	ar -rc libplayer.a player.o
	gcc  -Wall bn.c -o bn -L. -lplayer
	gcc  -Wall bn2.c -o bn2 -L. -lplayer
	gcc  -Wall bn3.c -o bn3 -L. -lplayer

clean:
	rm -f *.o *.a