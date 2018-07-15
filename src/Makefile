CC = gcc
FLAGS = -Wall `pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_dialog-5 allegro_audio-5 allegro_acodec-5`
BIN = bd
SOURCE = main.c base.c play.c test.c
HEADER = base.h play.h test.h

target : all

all: $(HEADER) $(SOURCE)
	$(CC) $(SOURCE) $(FLAGS) -o $(BIN)

clean :
	rm all
