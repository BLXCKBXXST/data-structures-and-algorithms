CC = gcc
CFLAGS = -Wall -Wextra -O2

SRCS_COMMON = sorting.c

all: select bubble shaker insert shell compare heap bsearch quick struct index

select: 1_1_select_sort.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o select 1_1_select_sort.c $(SRCS_COMMON)

bubble: 1_2_bubble_sort.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o bubble 1_2_bubble_sort.c $(SRCS_COMMON)

shaker: 1_3_shaker_sort.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o shaker 1_3_shaker_sort.c $(SRCS_COMMON)

insert: 1_4_insert_sort.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o insert 1_4_insert_sort.c $(SRCS_COMMON)

shell: 1_5_shell_sort.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o shell 1_5_shell_sort.c $(SRCS_COMMON) -lm

compare: 1_4_compare_quadratic.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o compare 1_4_compare_quadratic.c $(SRCS_COMMON)

heap: 1_9_heap_sort.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o heap 1_9_heap_sort.c $(SRCS_COMMON) -lm

bsearch: 1_6_binary_search.c search.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o bsearch 1_6_binary_search.c search.c $(SRCS_COMMON) -lm

quick: 1_10_quick_sort.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o quick 1_10_quick_sort.c $(SRCS_COMMON) -lm

struct: 1_7_struct_sort.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o struct 1_7_struct_sort.c $(SRCS_COMMON) -lm

index: 1_8_index_sort.c $(SRCS_COMMON)
	$(CC) $(CFLAGS) -o index 1_8_index_sort.c $(SRCS_COMMON) -lm

clean:
	rm -f select bubble shaker insert shell compare heap bsearch quick struct index

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
