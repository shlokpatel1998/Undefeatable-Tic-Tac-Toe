CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: alphatoe 

alphatoe.o: alphatoe.c ttt.h
	$(CC) $(CFLAGS) -c alphatoe.c

ttt1.o: ttt1.c ttt.h
	$(CC) $(CFLAGS) -c ttt1.c

ttt2.o: ttt2.c ttt.h
	$(CC) $(CFLAGS) -c ttt2.c

alphatoe: alphatoe.o ttt1.o ttt2.o
	$(CC) $(CFLAGS) alphatoe.o ttt1.o ttt2.o -o alphatoe

show_node.o: show_node.c ttt.h
	$(CC) $(CFLAGS) -c show_node.c

show_node: show_node.o ttt1.o ttt2.o
	$(CC) $(CFLAGS) show_node.o ttt1.o ttt2.o -o show_node

count_nodes.o: count_nodes.c ttt.h
	$(CC) $(CFLAGS) -c count_nodes.c

count_nodes: count_nodes.o ttt1.o ttt2.o
	$(CC) $(CFLAGS) count_nodes.o ttt1.o ttt2.o -o count_nodes

clean:
	rm *.o

cleanall:
	rm *.o alphatoe show_node count_nodes

run1: alphatoe
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --dsymutil=yes ./alphatoe
	./alphatoe

run2: show_node
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --dsymutil=yes ./show_node 0
	./show_node 0 2 83 89 116

run3: count_nodes
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --dsymutil=yes ./count_nodes
	./count_nodes