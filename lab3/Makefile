.SUFFIXES:
.PHONY: clean
all: lab3

CFLAGS := -Wall -Werror -ggdb -DOPTIMIZE -DDEBUG_PARSE -DDEBUG_GL -DDEBUG_MEM
LDFLAGS := -lglfw -lGLEW -lGL
objs := main.o init.o wavefront.o io.o shader.o

lab3: $(objs)
	@gcc $(LDFLAGS) -o $@ $^
	@echo "LD $@"

%.o: %.c
	@gcc $(CFLAGS) -c -o $@ $<
	@echo "CC $@"

clean:
	-rm -rf $(objs) lab3
