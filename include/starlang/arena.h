#pragma once

#include <stddef.h>
#include <stdint.h>

#define ARENA_ALIGN_SIZE sizeof(void *)

#define ARENA_ALIGN_BLOCK(size)                                                \
    size % ARENA_ALIGN_SIZE != 0                                               \
        ? size + (ARENA_ALIGN_SIZE - (size % ARENA_ALIGN_SIZE))                \
        : size

/*
 * struct for keeping information about an arena instance
 */
typedef struct {
    size_t capacity;

    uintptr_t start;
    uintptr_t cursor;
    uintptr_t end;
} arena_t;

/*
 * initializes an arena with the given capacity.
 */
arena_t *arena_init(size_t capacity);

/*
 * 'allocates' `size` amount of spaces then returns the pointer to the
 * beginning of that space.
 */
void *arena_alloc(arena_t *arena, size_t size);

/*
 * resizes a given arena `arena` to a given size `size`.
 */
void arena_resize(arena_t *arena, size_t size);

/*
 * frees an arena instance.
 */
void arena_free(arena_t *arena);

/*
 * resets an arena instance.
 */
void arena_reset(arena_t *arena);
