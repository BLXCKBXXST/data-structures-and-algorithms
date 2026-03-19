CC = gcc
CFLAGS = -Wall -Wextra -O2

SRCS_COMMON = sorting.c

all: select bubble shaker insert shell compare heap

select: main_select.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o select main_select.c $(SRCS_COMMON)

bubble: main_bubble.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o bubble main_bubble.c $(SRCS_COMMON)

shaker: main_shaker.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o shaker main_shaker.c $(SRCS_COMMON)

insert: main_insert.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o insert main_insert.c $(SRCS_COMMON)

shell: main_shell.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o shell main_shell.c $(SRCS_COMMON) -lm

compare: main_compare.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o compare main_compare.c $(SRCS_COMMON)

heap: main_heap.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o heap main_heap.c $(SRCS_COMMON) -lm

clean:
	rm -f select bubble shaker insert shell compare heap

PACK_FILE = project.pack
PACK_FILES = $(sort $(wildcard *.h *.c) Makefile)

pack:
	@rm -f $(PACK_FILE)
	@for f in $(PACK_FILES); do \
		echo "###FILE: $$f" >> $(PACK_FILE); \
		cat "$$f" >> $(PACK_FILE); \
	done
	@echo "Packed $(words $(PACK_FILES)) files -> $(PACK_FILE)"

unpack:
	@awk '/^###FILE:/{if(f)close(f);f=substr($$0,10);next}{if(f)print>f}' $(PACK_FILE)
	@echo "Unpacked from $(PACK_FILE)"

.PHONY: all clean pack unpack
