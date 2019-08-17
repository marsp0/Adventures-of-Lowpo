# https://en.wikipedia.org/wiki/Makefile

CXX           	:= g++
SRCDIR 			:= ./src
OBJDIR 			:= ./out
EXECUTABLE    	:= game
EXECUTABLE_TEST := test
EXECUTABLE_GCOV := gcov
CXXFLAGS      	:= -std=c++14
STRUCTURE 		:= $(shell find $(SRC) -name "*.cpp")
FILENAMES		:= $(notdir $(SRC))
OBJ 	     	:= $(FILENAMES:%.cpp=out/%.o)
LDFLAGS       	:= -lGL -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi

# all:$(OBJ)
# 	$(info $(OBJ))
# 	g++ $^ -o $@ $(LDFLAGS) && echo "[OK]  $@"

# out/%.o: $(SRC)
# 	$(info $(SRC))
# 	@$(CXX) $(CXXFLAGS) -c -g $< -o $@ $(LDFLAGS) && echo "[OK]  $@"

# clean:
# 	@rm -f out/* && echo "[CL]  out/"


CC      := g++
CCFLAGS := 
LDFLAGS := -lGL -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi
TARGETS	:= edit
MAINS  	:= $(addsuffix .o, $(TARGETS) )
OBJ    	:= kbd.o command.o display.o $(MAINS)
DEPS   	:= defs.h command.h

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJ)

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -o $@ $(LIBS) $^ $(CCFLAGS) $(LDFLAGS)
