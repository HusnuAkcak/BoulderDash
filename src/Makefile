CC = gcc
FLAGS = -pedantic -g -Wall `pkg-config --cflags --libs allegro-5 allegro_dialog-5 allegro_ttf-5 allegro_image-5 allegro_dialog-5 allegro_audio-5 allegro_acodec-5 `
BIN = bd
SOURCE = main.c base.c cave.c overhead.c play.c test.c
HEADER = base.h cave.h overhead.h play.h test.h

target : all

all: $(HEADER) $(SOURCE)
	$(CC) $(SOURCE) $(FLAGS) -o $(BIN)

clean :
	rm all
