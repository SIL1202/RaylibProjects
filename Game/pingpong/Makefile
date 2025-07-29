CC = gcc -I/opt/homebrew/include 
LDFLAGS = -L/opt/homebrew/lib -lraylib \
  -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL

SRC1_1 = main.c
SRC1_2 = menu.c
OUT1 = pp

$(OUT1): $(SRC1_1) $(SRC1_2)
	$(CC) $(SRC1_1) $(SRC1_2) -o $(OUT1) $(LDFLAGS) && ./$(OUT1) 

clean:
	rm -f  $(OUT1)
