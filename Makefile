CC = g++
CFLAGS = -Wall -Wextra -Wold-style-cast
BDIR = bin
SDIR = src

$(shell mkdir -p $(BDIR))
SOURCES := $(wildcard $(SDIR)/*.cpp)
OBJECTS := $(patsubst $(SDIR)/%.cpp,$(BDIR)/%.o,$(SOURCES))

all: fano-archiver

fano-archiver: $(OBJECTS)
	$(CC) -o $@ $^

$(BDIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -rf $(BDIR)
	rm -r fano-archiver