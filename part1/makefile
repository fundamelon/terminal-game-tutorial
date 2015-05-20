
CXX = g++ -MMD -g
CXXFLAGS = --std=c++11 -Wall -Werror -pedantic 
BOOSTFLAGS = -lboost_regex

MKDIR_P = mkdir -p
ODIR = bin/
SDIR = src/

_CPP = $(wildcard src/*.cpp)

_MAIN_OBJ = main.o
RSHELL_OBJ = $(addprefix $(ODIR), $(_RSHELL_OBJ) )

.PHONY: all
all: main

main: $(RSHELL_OBJ)
	$(CXX) -o $(ODIR)main $(RSHELL_OBJ) $(BOOSTFLAGS) $(CXXFLAGS) 

$(ODIR)%.o: $(SDIR)%.cpp
	$(MKDIR_P) $(ODIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY: clean
clean: 
	rm -f *.o *.d *~
	rm -rf $(ODIR)

.PHONY: test
test:
	make main 
	bin/main
