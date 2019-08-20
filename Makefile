VIDEO_STREAM := 0

# Build targets
CLIENT := client.out
SERVER := server.out

ifeq ($(VIDEO_STREAM), 1)
VIDEO_CLIENT := video_client.out
VIDEO_SERVER := video_server.out
OPENCV_INCLUDE := `pkg-config --cflags opencv`
OPENCV_LIB := `pkg-config --libs  opencv`
endif

# Directories
SRCDIR := ./src
OBJDIR := ./obj
DEPS := $(wildcard $(SRCDIR)/*.h $(SRCDIR)/*.hpp)

.PHONY: all clean

all: MAKE_OBJ_DIR $(CLIENT) $(SERVER) $(VIDEO_CLIENT) $(VIDEO_SERVER)

$(CLIENT): $(OBJDIR)/socket.o
	g++ -std=c++11 $^ $(SRCDIR)/main_client.cpp -o $@

$(SERVER): $(OBJDIR)/socket.o
	g++ -std=c++11 $^ $(SRCDIR)/main_server.cpp -o $@

$(VIDEO_CLIENT): $(OBJDIR)/socket.o
	g++ -std=c++11 $^ $(SRCDIR)/video_client.cpp -o $@ $(OPENCV_INCLUDE) $(OPENCV_LIB)

$(VIDEO_SERVER): $(OBJDIR)/socket.o
	g++ -std=c++11 $^ $(SRCDIR)/video_server.cpp -o $@ $(OPENCV_INCLUDE) $(OPENCV_LIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	g++ -std=c++11 -c $< -o $@

MAKE_OBJ_DIR:
	mkdir -p obj

clean:
	rm -f $(OBJDIR)/socket.o $(CLIENT) $(SERVER) $(VIDEO_CLIENT) $(VIDEO_SERVER)
