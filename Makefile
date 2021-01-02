awk: awk.o var.o split.o string.o

.PHONY: clean

clean:
	rm -f *.o awk
