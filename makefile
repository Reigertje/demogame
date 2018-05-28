OUT = game
CXX = g++
LIBS = -lGL -lGLU -lGLEW -lSDL2 -lm -fopenmp
CFLAGS = -c -I src -I . -std=c++11 -g

SRC_DIR = src

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SOURCES := $(call rwildcard, $(SRC_DIR)/,*.cpp)
OBJECTS := $(SOURCES:%.cpp=%.o)

GAME_SOURCES := $(call rwildcard, $(SRC_DIR)/game/,*.cpp)
GAME_OBJECTS := $(GAME_SOURCES:%.cpp=%.o)

all: $(OUT)

$(OUT) : $(OBJECTS)
	$(CXX) -o $(OUT) $(OBJECTS) $(LIBS)

%.o : %.cpp
	$(CXX) $(CFLAGS) $< -o $@ $(LIBS) 

purge: 
	rm -rf $(OBJECTS) game

clean:
	rm -rf $(GAME_OBJECTS) $(GENERATION_OBJECTS) game

test:
	echo "\n$(OBJECTS)"