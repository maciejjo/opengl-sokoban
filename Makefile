TARGET		= sokoban
CC				= clang -c
LINKER		= clang
CFLAGS		= -Wall -Wextra -Werror -pedantic -O2 -g -std=gnu99
LFLAGS		= -lkazmath -lglut -lGLEW -lGL -lm -lglfw -lassimp


SRCDIR		= src
OBJDIR		= obj
BINDIR		= bin

SOURCES 	:= $(wildcard $(SRCDIR)/*.c)
SRC_OBJECTS		:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

REMOVE := rm -f


$(BINDIR)/$(TARGET): $(SRC_OBJECTS)
	$(LINKER) $(LFLAGS) -o $@ $(SRC_OBJECTS)
	@echo "Linking complete"

$(SRC_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled $<"

.PHONY: clean
clean:
	$(REMOVE) $(SRC_OBJECTS)
	@echo "Deleted $<"
