.SUFFIXES:
.PHONY: clean
all: lab3

CC := clang
CFLAGS := -Wmost -Werror -ggdb -DOPTIMIZE -DDEBUG_PARSE -DDEBUG_GL -DDEBUG_MEM -ftree-vectorize
LDFLAGS := -lglfw -lGLEW -lGL -lm
objs := main.o init.o wavefront.o io.o shader.o
objdir := obj

real_objs := $(addprefix $(objdir)/,$(objs))

lab3: $(real_objs)
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "LD $@"

$(objdir):
	@mkdir -p "${objdir}"
	@echo "MKDIR ${objdir}"

$(objdir)/%.o: %.c | $(objdir)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo "CC $@"

clean:
	-rm -rf $(real_objs) lab3
