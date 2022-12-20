CC = g++
APPNAME = ivfs
FILES := $(wildcard source/*.cpp)
FLAGS := -g
DEBUG_FLAGS := -DDEBUG -g

$(APPNAME).exe: $(patsubst src/%.cpp, ./%.o, $(FILES))
	$(CC) $(FLAGS) $^ -o $@

debug: $(patsubst src/%.cpp, ./%.o, $(FILES))
	$(CC) $(FLAGS) $^ -o $(APPNAME).exe

%.o: src/%.cpp
	$(CC) $(FLAGS) -c -MD $<

clean:
	rm *.o *.exe *.d *.out
