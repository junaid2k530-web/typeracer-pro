CXX     = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET  = typeracer

SRCS = main.cpp \
       Typist.cpp \
       Player.cpp \
       AIBot.cpp \
       GameSession.cpp \
       WordBank.cpp \
       Coach.cpp \
       PerformanceTracker.cpp \
       Display.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET)
