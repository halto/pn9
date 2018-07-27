#
BIN := pn9
INCDIR := -I./inc
SRCDIR := ./src
OBJDIR := ./build
BINDIR := ./bin
#
TARGET := $(BINDIR)/$(BIN)
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
#
$(warning $(OBJS))
#
CC := gcc
CFLAGS := -Wall -O2 -I$(INCDIR)


.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	-mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) -fr $(OBJDIR) $(TARGET)

