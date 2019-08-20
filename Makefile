# Build targets
CLIENT := client.out
SERVER := server.out

SRCDIR := ./src
OBJDIR := ./obj
DEPS := $(wildcard $(SRCDIR)/*.h $(SRCDIR)/*.hpp)

.PHONY: all clean

all: MAKE_OBJ_DIR $(CLIENT) $(SERVER)

$(CLIENT): $(OBJDIR)/socket.o
	g++ $^ $(SRCDIR)/main_client.cpp -o $@

$(SERVER): $(OBJDIR)/socket.o
	g++ $^ $(SRCDIR)/main_server.cpp -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	g++ -c $< -o $@

MAKE_OBJ_DIR:
	mkdir -p obj

clean:
	rm -f $(OBJDIR)/socket.o $(CLIENT) $(SERVER)
