CC = gcc -I/opt/homebrew/include 
CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib \
  -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL

SRC1 = game.cpp
OUT1 = game

SRC2 = bouncing.cpp
OUT2 = bouncing

SRC3 = destroy.c
OUT3 = destroy

$(OUT1): $(SRC1)
	$(CXX) $(CXXFLAGS) $(SRC1) -o $(OUT1) $(LDFLAGS) && ./$(OUT1) && rm $(OUT1)

$(OUT2): $(SRC2)
	$(CXX) $(CXXFLAGS) $(SRC2) -o $(OUT2) $(LDFLAGS) && ./$(OUT2) && rm $(OUT2)

$(OUT3): $(SRC3)
	$(CC) $(SRC3) -o $(OUT3) $(LDFLAGS) && ./$(OUT3) && rm $(OUT3)

clean:
	rm -f $(OUT1) $(OUT2) $(OUT3)
