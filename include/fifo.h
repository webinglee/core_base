/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * a very simple circular buffer FIFO implementation
 */

#ifndef _FIFO_H
#define _FIFO_H

#include <stdint.h>
#include <sys/types.h>

typedef struct fifo_buffer {
    uint8_t *buffer;
    uint8_t *rptr, *wptr, *end;
    uint32_t rndx, wndx;
} fifo_buffer;

/**
 * Initialize an fifo_buffer.
 * @param size of FIFO
 * @return fifo_buffer or NULL in case of memory allocation failure
 */
fifo_buffer *fifo_alloc(unsigned int size);

/**
 * Initialize an fifo_buffer.
 * @param nmemb number of elements
 * @param size  size of the single element
 * @return fifo_buffer or NULL in case of memory allocation failure
 */
fifo_buffer *fifo_alloc_array(size_t nmemb, size_t size);

/**
 * Free an fifo_buffer.
 * @param f fifo_buffer to free
 */
void fifo_free(fifo_buffer *f);

/**
 * Free an fifo_buffer and reset pointer to NULL.
 * @param f fifo_buffer to free
 */
void fifo_freep(fifo_buffer **f);

/**
 * Reset the fifo_buffer to the state right after fifo_alloc, in particular it is emptied.
 * @param f fifo_buffer to reset
 */
void fifo_reset(fifo_buffer *f);

/**
 * Return the amount of data in bytes in the fifo_buffer, that is the
 * amount of data you can read from it.
 * @param f fifo_buffer to read from
 * @return size
 */
int fifo_size(const fifo_buffer *f);

/**
 * Return the amount of space in bytes in the fifo_buffer, that is the
 * amount of data you can write into it.
 * @param f fifo_buffer to write into
 * @return size
 */
int fifo_space(const fifo_buffer *f);

/**
 * Feed data at specific position from an fifo_buffer to a user-supplied callback.
 * Similar as fifo_gereric_read but without discarding data.
 * @param f fifo_buffer to read from
 * @param offset offset from current read position
 * @param buf_size number of bytes to read
 * @param func generic read function
 * @param dest data destination
 */
int fifo_generic_peek_at(fifo_buffer *f, void *dest, int offset, int buf_size, void (*func)(void*, void*, int));

/**
 * Feed data from an fifo_buffer to a user-supplied callback.
 * Similar as fifo_gereric_read but without discarding data.
 * @param f fifo_buffer to read from
 * @param buf_size number of bytes to read
 * @param func generic read function
 * @param dest data destination
 */
int fifo_generic_peek(fifo_buffer *f, void *dest, int buf_size, void (*func)(void*, void*, int));

/**
 * Feed data from an fifo_buffer to a user-supplied callback.
 * @param f fifo_buffer to read from
 * @param buf_size number of bytes to read
 * @param func generic read function
 * @param dest data destination
 */
int fifo_generic_read(fifo_buffer *f, void *dest, int buf_size, void (*func)(void*, void*, int));

/**
 * Feed data from a user-supplied callback to an fifo_buffer.
 * @param f fifo_buffer to write to
 * @param src data source; non-const since it may be used as a
 * modifiable context by the function defined in func
 * @param size number of bytes to write
 * @param func generic write function; the first parameter is src,
 * the second is dest_buf, the third is dest_buf_size.
 * func must return the number of bytes written to dest_buf, or <= 0 to
 * indicate no more data available to write.
 * If func is NULL, src is interpreted as a simple byte array for source data.
 * @return the number of bytes written to the FIFO
 */
int fifo_generic_write(fifo_buffer *f, void *src, int size, int (*func)(void*, void*, int));

/**
 * Resize an fifo_buffer.
 * In case of reallocation failure, the old FIFO is kept unchanged.
 *
 * @param f fifo_buffer to resize
 * @param size new fifo_buffer size in bytes
 * @return <0 for failure, >=0 otherwise
 */
int fifo_realloc2(fifo_buffer *f, unsigned int size);

/**
 * Enlarge an fifo_buffer.
 * In case of reallocation failure, the old FIFO is kept unchanged.
 * The new fifo size may be larger than the requested size.
 *
 * @param f fifo_buffer to resize
 * @param additional_space the amount of space in bytes to allocate in addition to fifo_size()
 * @return <0 for failure, >=0 otherwise
 */
int fifo_grow(fifo_buffer *f, unsigned int additional_space);

/**
 * Read and discard the specified amount of data from an fifo_buffer.
 * @param f fifo_buffer to read from
 * @param size amount of data to read in bytes
 */
void fifo_drain(fifo_buffer *f, int size);

/**
 * Return a pointer to the data stored in a FIFO buffer at a certain offset.
 * The FIFO buffer is not modified.
 *
 * @param f    fifo_buffer to peek at, f must be non-NULL
 * @param offs an offset in bytes, its absolute value must be less
 *             than the used buffer size or the returned pointer will
 *             point outside to the buffer data.
 *             The used buffer size can be checked with fifo_size().
 */
static inline uint8_t* fifo_peek2(const fifo_buffer *f, int offs)
{
    uint8_t *ptr = f->rptr + offs;
    if (ptr >= f->end)
        ptr = f->buffer + (ptr - f->end);
    else if (ptr < f->buffer)
        ptr = f->end - (f->buffer - ptr);
    return ptr;
}

#endif /* _FIFO_H */
