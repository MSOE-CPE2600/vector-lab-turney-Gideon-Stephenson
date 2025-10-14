CC=gcc
CFLAGS=-c -Wall
SOURCES= main.c vector.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=vector

all: $(SOURCES) $(EXECUTABLE)

-include $(OBJECTS:.o=.d)

$(EXECUTABLE): $(OBJECTS)
		$(CC) $(OBJECTS) -o $@

.c.o:
		$(CC) $(CFLAGS) $< -o $@
		$(CC) -MM $<>$*.d

clean:
		rm -rf $(OBJECTS) $(EXECUTABLE) *.d