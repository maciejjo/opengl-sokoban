TARGET		= sokoban
CC				= gcc -c
LINKER		= gcc
CFLAGS		= -Wall -Wextra -pedantic -O2 -g -std=gnu99
LFLAGS		= -lglut -lGLEW -lGL -lm


SRCDIR		= src
OBJDIR		= obj

SOURCES 	:= $(wildcard $(SRCDIR)/*.c)
SRC_OBJECTS		:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

REMOVE := rm -f


$(TARGET): $(SRC_OBJECTS)
	$(LINKER) $(LFLAGS) -o $@ $(SRC_OBJECTS)
	@echo "Linking complete"

$(SRC_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled $<"

.PHONY: clean
clean:
	$(REMOVE) $(SRC_OBJECTS)
	@echo "Deleted $<"
