main: main.c
	gcc -Wall -Wextra main.c -o main ./raylib/lib/libraylib.a -I./raylib/include -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

.PHONY: clean
clean:
	rm -f main
