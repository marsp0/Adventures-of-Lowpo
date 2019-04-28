CXX           	:= g++
LD            	:= g++

EXECUTABLE    	:= output
CXXFLAGS      	:= -std=c++14 
LDFLAGS       	:= -lGL -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi
SRC           	:= $(wildcard ./*.cpp)
OBJ           	:= $(SRC:./%.cpp=out/%.o)

EXECUTABLE_TEST := test
CXXFLAGS_TEST 	:= $(CXXFLAGS)
LDFLAGS_TEST  	:= $(LDFLAGS)
SRC_TEST      	:= $(wildcard test/*.cpp)
OBJ_TEST      	:= $(filter-out out/main.o, $(OBJ)) $(SRC_TEST:test/%.cpp=out/%.o)

.SUFFIXES:

# --------------------------------------------------------------

.PHONY: all
all: out/$(EXECUTABLE)

out/$(EXECUTABLE): $(OBJ)
	@$(LD) $^ -o $@ $(LDFLAGS) && echo "[OK]  $@"

# --------------------------------------------------------------

.PHONY: test
test: out/$(EXECUTABLE_TEST)

out/$(EXECUTABLE_TEST): $(OBJ_TEST)
	@$(LD) $^ -o $@ $(LDFLAGS_TEST) && echo "[OK]  $@"

# --------------------------------------------------------------

out/%.o: ./%.cpp
	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@ .o"

out/%.o: test/%.cpp
	@$(CXX) $(CXXFLAGS_TEST) -c -g $< -o $@ && echo "[OK]  $@"

# --------------------------------------------------------------

.PHONY: clean, clear
clean clear:
	@rm -f out/* && echo "[CL]  out/"