CC = gcc

objects = bank.o

banker: $(objects)
	$(CC) -o banker $(objects)


.PHONY: clean
clean:
	rm $(objects) banker result.txt
