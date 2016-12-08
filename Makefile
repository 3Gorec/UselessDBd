BUILD_DIR=build/
EXT_INCLUDES=
DIRS=./ useless_nl_core
SRC_DIR=$(foreach d, $(DIRS), $d/)
CPP_SOURCES=$(foreach dir,$(SRC_DIR),$(wildcard $(dir)*.cpp))
CPP_SOURCE_FILES=$(foreach dir,$(SRC_DIR),$(patsubst $(dir)%,%,$(wildcard $(dir)*.cpp)))
CPP_OBJECTS=$(CPP_SOURCE_FILES:.cpp=.o)
INCLUDES=$(foreach d, $(DIRS), -I$d) $(foreach d, $(EXT_INCLUDES), -I$d) 
SYMBOLS=
DSYMBOLS=$(foreach sym, $(SYMBOLS), -D$(sym))
LDFLAGS=-lsqlite3
TARGET=uselessdbd
vpath %.cpp $(DIRS)

all: debug

release: $(TARGET)

debug: CXXFLAGS+=-g3 -std=c++11
debug: $(TARGET)	

$(TARGET): $(CPP_OBJECTS) prepare	
	$(CXX) -o $(BUILD_DIR)$@ $(patsubst %,$(BUILD_DIR)%,$(CPP_OBJECTS)) $(LDFLAGS)

prepare:	
	@echo $(CPP_SOURCE_FILES)
	@if [ ! -d $(BUILD_DIR) ] ; then mkdir $(BUILD_DIR) ;fi	

%.o: %.cpp prepare
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)$@ -c $< $(INCLUDES) $(DSYMBOLS)

clean:
	rm -Rf $(BUILD_DIR)
