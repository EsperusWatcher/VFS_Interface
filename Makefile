CC = g++
APPNAME = ivfs
FILES := $(wildcard source/*.cpp)
FLAGS := -std=c++17 
DEBUG_FLAGS := -DDEBUG -g

$(APPNAME).exe: $(patsubst source/%.cpp, ./%.o, $(FILES))
	$(CC) $(FLAGS) $^ -o $@ -lstdc++fs 

debug: $(patsubst source/%.cpp, ./%.o, $(FILES))
	$(CC) $(FLAGS) $^ -o $(APPNAME).exe

%.o: source/%.cpp
	$(CC) $(FLAGS) -c -MD $<

clean:
	rm *.o *.exe *.d *.out
