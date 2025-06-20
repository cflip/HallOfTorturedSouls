CFLAGS += -Wall -Wextra
LDFLAGS += -mwindows -lgdi32

OBJS =	data_credits.o \
		data_map.o \
		data_textures.o \
		extra_main.o \
		floating.o \
		game.o \
		init.o \
		map.o \
		render.o \
		update.o \
		wall.o \
		window.o \
		xl_util.o

.PHONY: all
all: torturedsouls

torturedsouls: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f torturedsouls.exe $(OBJS)
