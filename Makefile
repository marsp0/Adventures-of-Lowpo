CXX           	:= g++
LD            	:= g++

EXECUTABLE    	:= output
CXXFLAGS      	:= -std=c++14 
LDFLAGS       	:= -lGL -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi
SRC_DIRS		:= \
. \
./Systems/Physics \
./Systems/Rendering \
./Systems/Input \
./Systems/Animation \
./Systems/Messaging \
./Components \
./Components/Animation \
./External

# get all files with cpp extension from the src_dirs
SRC           	:= $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
# replace the ./physics/ with nothing
FILENAMES		:= $(notdir $(SRC))
# replace all the cpp extensions with .o
OBJ           	:= $(FILENAMES:%.cpp=out/%.o)

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

# MAIN dir

out/%.o: ./%.cpp
	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@"


# SYSTEM


out/%.o: ./Systems/Physics/%.cpp
	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@"

out/%.o: ./Systems/Animation/%.cpp
	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@"

out/%.o: ./Systems/Input/%.cpp
	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@"

out/%.o: ./Systems/Rendering/%.cpp
	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@"

out/%.o: ./Systems/Messaging/%.cpp
	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@"


# COMPONENT


out/%.o: ./Components/Animation/%.cpp
	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@"

out/%.o: ./Components/%.cpp
	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@"


# EXTERNAL


out/%.o: ./External/%.cpp
	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@"

# TEST

out/%.o: test/%.cpp
	@$(CXX) $(CXXFLAGS_TEST) -c -g $< -o $@ && echo "[OK]  $@"

# --------------------------------------------------------------

.PHONY: clean, clear
clean clear:
	@rm -f out/* && echo "[CL]  out/"