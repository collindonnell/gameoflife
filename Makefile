LDLIBS = -lncurses

gameoflife:
	$(CC) gameoflife.c $(LDLIBS) -o gol.out

clean:
	rm *.out
