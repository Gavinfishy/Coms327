########################################
# Adhere statement in COPYWRITE.txt   #
########################################

CC = gcc
CXX = g++
ECHO = echo
RM = rm -f

TERM = "S2019"

CFLAGS = -Wall -ggdb -funroll-loops -DTERM=$(TERM)
CXXFLAGS = -Wall -ggdb -funroll-loops -DTERM=$(TERM)

LDFLAGS = -lncurses

BIN = PokemonGame
OBJS = main.o db.o encounters.o display.o generate.o dijkstra.o heap.o move.o windows.o

all: $(BIN) etags

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MF $*.d -c $<

.PHONY: all clean clobber etags

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o $(BIN) *.d TAGS core vgcore.* gmon.out

clobber: clean
	@$(ECHO) Removing backup files
	@$(RM) *~ \#* *pgm

