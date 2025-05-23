CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib \
  -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL

SRC = main.cpp
OUT = queen

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS) && ./queen && rm queen

clean:
	rm -f $(OUT)
