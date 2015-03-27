IS_DEBUG	= y

# compiler and linker flags -std=c++0x
CXXFLAGS	= -Wall -Wextra -pedantic -Wno-unused-function -fmessage-length=0 -std=gnu++11 
LDFLAGS		= -lpthread -lm

# enable/disable debug
ifeq ($(IS_DEBUG),y)
	CXXFLAGS += -O0 -ggdb
else
	CXXFLAGS += -O3
endif

# folders and files locations
BUILD_DIR	= build
SOURCE_DIR	= src
TARGET		= $(BUILD_DIR)/Delaunay
OBJS		= $(BUILD_DIR)/Delaunay.o $(BUILD_DIR)/Vertex.o $(BUILD_DIR)/Triangle.o $(BUILD_DIR)/Helper.o

# targets and rules
.PHONY: all clean

folder: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(SOURCE_DIR)/%.h
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(LDFLAGS)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) 

all:	folder $(TARGET)

clean:
	rm -rf $(BUILD_DIR)/*
