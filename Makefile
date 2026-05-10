CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -Ilib
LDLIBS  = -lm

LIB_DIR = lib
ARR_DIR = src/arrays
LST_DIR = src/lists
BIN     = build/bin

# Общие источники
SORTING := $(LIB_DIR)/sorting.c
LIST    := $(LIB_DIR)/list.c
SEARCH  := $(LIB_DIR)/search.c
HASH    := $(LIB_DIR)/hash.c

# Цели (имя цели = имя бинарника = имя исходника без .c)
ARRAY_TARGETS := \
    1_1_select_sort   1_2_bubble_sort   1_3_shaker_sort \
    1_4_insert_sort   1_5_shell_sort    1_6_binary_search \
    1_7_struct_sort   1_8_index_sort    1_9_heap_sort \
    1_10_quick_sort

LIST_TARGETS := \
    2_1_list          2_2_merge_sort    2_3_digital_sort \
    2_4_hash_chain    2_5_hash_open

ALL_TARGETS := $(ARRAY_TARGETS) $(LIST_TARGETS)
ALL_BINS    := $(addprefix $(BIN)/,$(ALL_TARGETS))

# Короткие алиасы (для удобства)
ALIASES := select bubble shaker insert shell heap bsearch quick struct index \
           list merge digital hash_chain hash_open

.PHONY: all clean pack unpack help $(ALL_TARGETS) $(ALIASES)

all: $(ALL_BINS)

# `make <full-name>` собирает соответствующий бинарник
$(ALL_TARGETS): %: $(BIN)/% ;

# Каталог вывода
$(BIN):
	@mkdir -p $@

# ── Массивные задачи (1.1–1.10) ─────────────────────────────────────────────
$(BIN)/1_1_select_sort:   $(ARR_DIR)/1_1_select_sort.c   $(SORTING)             | $(BIN)
$(BIN)/1_2_bubble_sort:   $(ARR_DIR)/1_2_bubble_sort.c   $(SORTING)             | $(BIN)
$(BIN)/1_3_shaker_sort:   $(ARR_DIR)/1_3_shaker_sort.c   $(SORTING)             | $(BIN)
$(BIN)/1_4_insert_sort:   $(ARR_DIR)/1_4_insert_sort.c   $(SORTING)             | $(BIN)
$(BIN)/1_5_shell_sort:    $(ARR_DIR)/1_5_shell_sort.c    $(SORTING)             | $(BIN)
$(BIN)/1_6_binary_search: $(ARR_DIR)/1_6_binary_search.c $(SEARCH) $(SORTING)   | $(BIN)
$(BIN)/1_7_struct_sort:   $(ARR_DIR)/1_7_struct_sort.c   $(SORTING)             | $(BIN)
$(BIN)/1_8_index_sort:    $(ARR_DIR)/1_8_index_sort.c    $(SORTING)             | $(BIN)
$(BIN)/1_9_heap_sort:     $(ARR_DIR)/1_9_heap_sort.c     $(SORTING)             | $(BIN)
$(BIN)/1_10_quick_sort:   $(ARR_DIR)/1_10_quick_sort.c   $(SORTING)             | $(BIN)

# ── Списочные задачи (2.1–2.5) ──────────────────────────────────────────────
$(BIN)/2_1_list:          $(LST_DIR)/2_1_list.c          $(LIST)               | $(BIN)
$(BIN)/2_2_merge_sort:    $(LST_DIR)/2_2_merge_sort.c    $(LIST)               | $(BIN)
$(BIN)/2_3_digital_sort:  $(LST_DIR)/2_3_digital_sort.c  $(LIST) $(SORTING)    | $(BIN)
$(BIN)/2_4_hash_chain:    $(LST_DIR)/2_4_hash_chain.c    $(HASH) $(LIST)       | $(BIN)
$(BIN)/2_5_hash_open:     $(LST_DIR)/2_5_hash_open.c     $(HASH) $(LIST)       | $(BIN)

# ── Универсальное правило линковки ──────────────────────────────────────────
$(BIN)/%:
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^) $(LDLIBS)

# ── Короткие алиасы ─────────────────────────────────────────────────────────
select:     1_1_select_sort
bubble:     1_2_bubble_sort
shaker:     1_3_shaker_sort
insert:     1_4_insert_sort
shell:      1_5_shell_sort
bsearch:    1_6_binary_search
struct:     1_7_struct_sort
index:      1_8_index_sort
heap:       1_9_heap_sort
quick:      1_10_quick_sort
list:       2_1_list
merge:      2_2_merge_sort
digital:    2_3_digital_sort
hash_chain: 2_4_hash_chain
hash_open:  2_5_hash_open

clean:
	rm -rf build project.pack

# ── pack/unpack: сериализация исходников в один файл ────────────────────────
PACK_FILE  = project.pack
PACK_FILES = $(sort $(wildcard $(LIB_DIR)/*.c $(LIB_DIR)/*.h \
                               $(ARR_DIR)/*.c $(LST_DIR)/*.c \
                               Makefile README.md \
                               $(ARR_DIR)/README.md $(LST_DIR)/README.md))

pack:
	@rm -f $(PACK_FILE)
	@for f in $(PACK_FILES); do \
		echo "###FILE: $$f" >> $(PACK_FILE); \
		cat "$$f" >> $(PACK_FILE); \
	done
	@echo "Packed $(words $(PACK_FILES)) files -> $(PACK_FILE)"

unpack:
	@awk '/^###FILE:/{ \
		if (f) close(f); \
		f = substr($$0, 10); \
		dir = f; sub(/\/[^\/]*$$/, "", dir); \
		if (dir != f) system("mkdir -p \"" dir "\""); \
		next \
	} { if (f) print > f }' $(PACK_FILE)
	@echo "Unpacked from $(PACK_FILE)"

help:
	@echo "Цели:"
	@echo "  make                       — собрать всё"
	@echo "  make <full-name>           — собрать одну программу по полному имени"
	@echo "                                 (например: make 1_1_select_sort, make 2_2_merge_sort)"
	@echo "  make <alias>               — короткий алиас (select, bubble, …, hash_open)"
	@echo "  make clean                 — удалить build/ и project.pack"
	@echo "  make pack / make unpack    — сериализация исходников"
	@echo ""
	@echo "Бинарники складываются в $(BIN)/"
