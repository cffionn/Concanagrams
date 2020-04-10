CXX = g++
#O3 for max optimization (go to 0 for debug)
CXXFLAGS = -Wall -Werror -O3 -Wextra -Wno-unused-local-typedefs -Wno-deprecated-declarations -std=c++11 -g
ifeq "$(GCCVERSION)" "1"
  CXXFLAGS += -Wno-error=misleading-indentation
endif

INCLUDE=-I$(PWD)
LIB=-L$(PWD)/lib

MKDIR_BIN=mkdir -p $(PWD)/bin
MKDIR_LIB=mkdir -p $(PWD)/lib
MKDIR_OBJ=mkdir -p $(PWD)/obj
MKDIR_OUTPUT=mkdir -p $(PWD)/output

all: mkdirBin mkdirLib mkdirObj mkdirOutput obj/checkMakeDir.o lib/libConcanagrams.so bin/runConcanagrams.exe

mkdirBin:
	$(MKDIR_BIN)

mkdirLib:
	$(MKDIR_LIB)

mkdirObj:
	$(MKDIR_OBJ)

mkdirOutput:
	$(MKDIR_OUTPUT)

obj/checkMakeDir.o: src/checkMakeDir.C
	$(CXX) $(CXXFLAGS) -fPIC -c src/checkMakeDir.C -o obj/checkMakeDir.o $(INCLUDE) 

lib/libConcanagrams.so:
	$(CXX) $(CXXFLAGS) -fPIC -shared -o lib/libConcanagrams.so obj/checkMakeDir.o $(INCLUDE)


bin/runConcanagrams.exe: src/runConcanagrams.C
	$(CXX) $(CXXFLAGS) src/runConcanagrams.C -o bin/runConcanagrams.exe $(INCLUDE)  $(LIB) -lConcanagrams

clean:
	rm -f ./*~
	rm -f ./#*#
	rm -f bin/*.exe
	rm -rf bin
	rm -f include/*~
	rm -f include/#*#
	rm -f input/*~
	rm -f input/#*#
	rm -rf lib
	rm -rf obj
	rm -f src/*~
	rm -f src/#*#
