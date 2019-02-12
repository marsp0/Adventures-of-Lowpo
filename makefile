SRC_DIR := .
OBJ_DIR := ./pobj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/physics/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
# OBJ_FILES += $(patsubst $(SRC_DIR)/physics/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := -lGL -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi
# CPPFLAGS := -std=c++14 -Wall -pedantic
CPPFLAGS := -std=c++14
CXXFLAGS := ...

output.out: $(OBJ_FILES)
	g++ -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	# g++ $(CPPFLAGS) -c -o $@ $<
	g++ $(CPPFLAGS) -c -g -o $@ $<

$(OBJ_DIR)/physics/%.o: $(SRC_DIR)/physics/%.cpp
	# g++ $(CPPFLAGS) -c -o $@ $<
	g++ $(CPPFLAGS) -c -g -o $@ $<
	
clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(OBJ_DIR)/physics/*.o
	rm -rf output.out

.PHONY: all
	$(SRC_DIR)/%.cpp