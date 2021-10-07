SRC_DIR  := src
OBJ_DIR  := obj
BIN_DIR  := bin
CPPFLAGS := -Iinclude
CFLAGS   := -Wall -Wextra -pedantic -Wshadow
CXXFLAGS := -Wall -Wextra -pedantic -Wshadow -std=c++20
LDLIBS   := -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglfw

.PHONY: all clean

all: $(BIN_DIR)/graphics

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

$(BIN_DIR)/graphics: $(OBJ_DIR)/main.o $(OBJ_DIR)/glad.o | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/glad.o: $(SRC_DIR)/glad.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CCFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@
