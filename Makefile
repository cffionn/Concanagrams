CXX = g++
#O3 for max optimization (go to 0 for debug)
CXXFLAGS = -Wall -Werror -O3 -Wextra -Wno-unused-local-typedefs -Wno-deprecated-declarations -std=c++11 -g
ifeq "$(GCCVERSION)" "1"
  CXXFLAGS += -Wno-error=misleading-indentation
endif

INCLUDE=-I$(PWD)
LIB=-L$(PWD)/lib

MKDIR_BIN=mkdir -p $(PWD)/bin

all: mkdirBin bin/runConcanagrams.exe

mkdirBin:
	$(MKDIR_BIN)

bin/runConcanagrams.exe: src/runConcanagrams.C
	$(CXX) $(CXXFLAGS) src/runConcanagrams.C -o bin/runConcanagrams.exe $(INCLUDE)

clean:
	rm -f ./*~
	rm -f ./#*#
	rm -f bin/*.exe
	rm -rf bin
	rm -f include/*~
	rm -f include/#*#
	rm -f input/*~
	rm -f input/#*#
	rm -f src/*~
	rm -f src/#*#
