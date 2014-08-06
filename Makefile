TARGET		= sokoban
CFLAGS		= -Wall -Wextra -Werror -pedantic -O2 -g -std=gnu99
LFLAGS		= -lkazmath -lglut -lGLEW -lGL -lm -lglfw -lassimp


SRCDIR		= src
OBJDIR		= obj
BINDIR		= bin

SOURCES 	:= $(wildcard $(SRCDIR)/*.c)
SRC_OBJECTS		:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

REMOVE := rm -rf


$(BINDIR)/$(TARGET): $(SRC_OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) $(LFLAGS) -o $@ $(SRC_OBJECTS)
	@echo "Linking complete"

$(SRC_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) $< -o $@
	@echo "Compiled $<"

.PHONY: clean
clean:
	$(REMOVE) $(SRC_OBJECTS) $(OBJDIR) $(BINDIR)
	@echo "Deleted $<"
