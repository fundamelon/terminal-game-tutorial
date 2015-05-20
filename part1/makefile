
CXX = g++ -MMD -g
CXXFLAGS = --std=c++11 -Wall -Werror -pedantic 
BOOSTFLAGS = -lboost_regex

MKDIR_P = mkdir -p
ODIR = bin/
SDIR = src/

_CPP = $(wildcard src/*.cpp)

_RSHELL_OBJ = main.o rshell.o
RSHELL_OBJ = $(addprefix $(ODIR), $(_RSHELL_OBJ) )

_LS_OBJ = ls.o
LS_OBJ = $(addprefix $(ODIR), $(_LS_OBJ) )

_RM_OBJ = rm.o
RM_OBJ = $(addprefix $(ODIR), $(_RM_OBJ) )

_MV_OBJ = mv.o
MV_OBJ = $(addprefix $(ODIR), $(_MV_OBJ) )


.PHONY: all
all: rshell ls rm mv

rshell: $(RSHELL_OBJ)
	$(CXX) -o $(ODIR)rshell $(RSHELL_OBJ) $(BOOSTFLAGS) $(CXXFLAGS) 

ls: $(LS_OBJ)
	$(CXX) -o $(ODIR)ls $(LS_OBJ) $(BOOSTFLAGS) $(CXXFLAGS) 

rm: $(RM_OBJ)
	$(CXX) -o $(ODIR)rm $(RM_OBJ) $(CXXFLAGS)

mv: $(MV_OBJ)
	$(CXX) -o $(ODIR)mv $(MV_OBJ) $(CXXFLAGS)

$(ODIR)%.o: $(SDIR)%.cpp
	$(MKDIR_P) $(ODIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)


.PHONY: clean
clean: 
	rm -f *.o *.d rshell *~
	rm -rf $(ODIR)

.PHONY: test
test:
	make rshell
	bin/rshell
