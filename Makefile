CC = g++
APPNAME = ivfs
FILES := $(filter-out source/test.cpp, $(wildcard source/*.cpp))
TESTFILES := $(filter-out source/main.cpp, $(wildcard source/*.cpp))
FLAGS := -std=c++17 
DEBUG_FLAGS := -Wall -std=c++17

$(APPNAME).exe: $(patsubst source/%.cpp, ./%.o, $(FILES))
	$(CC) $(FLAGS) $^ -o $@ -lstdc++fs 

debug: $(patsubst source/%.cpp, ./%.o, $(TESTFILES))
	$(CC) $(FLAGS) $^ -o $(APPNAME)_test.exe -lstdc++fs

%.o: source/%.cpp
	$(CC) $(FLAGS) -c -MD $<

%.o: test/%.cpp
	$(CC) $(FLAGS) -c -MD $<

clean:
	rm *.o *.exe *.d *.out
