CFLAGS=`pkg-config --cflags sdl2`
LIBS=`pkg-config --libs sdl2`
TARGET=rule110

all:
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(LIBS)

clean:
	rm -rf $(TARGET)
