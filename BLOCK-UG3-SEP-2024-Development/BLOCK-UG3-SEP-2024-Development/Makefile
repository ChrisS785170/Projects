CC = g++

CFLAGS = -Wall -g -static

# Specify all source files
SOURCES = main.cpp streamProcessor/streamProcessor.cpp compressionAlgorithms/noCompression.cpp compressionAlgorithms/RLECompression.cpp

# Specify the output executable name
OUTPUT = blockModelCompression.exe

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCES)

clean:
	rm -f *.exe
	rm -f *.o
