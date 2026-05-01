var:=$(patsubst %.c,%.o,$(wildcard *.c))
mp3.exe:$(var)
	gcc -o $@ $^
clean:
	rm *.o *.exe