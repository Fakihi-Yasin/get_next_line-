/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yafakihi <yafakihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 15:44:54 by yafakihi          #+#    #+#             */
/*   Updated: 2026/01/02 10:40:13 by yafakihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
 * read_buffer:
 * - Purpose: ensure `buffer` contains at least one full line (ending with '\n')
 *   or all remaining data from `fd` (EOF reached).
 * - Parameters:
 *   - fd: file descriptor to read from.
 *   - buffer: existing leftover string from previous calls. May be NULL.
 * - Behavior & ownership:
 *   - Allocates a temporary read buffer `temp` of size `BUFFER_SIZE + 1`.
 *   - Repeatedly reads up to `BUFFER_SIZE` bytes into `temp` and appends
 *     it to `buffer` using `ft_strjoin` until a '\n' is present or read() returns 0.
 *   - `ft_strjoin` (in this project) takes ownership of its first argument
 *     and frees it; it returns a newly allocated string which becomes the new `buffer`.
 *   - On read error, frees `temp` and returns NULL.
 *   - After the loop, frees `temp` and:
 *     - if `buffer` is NULL or empty (""), free it and return NULL.
 *     - otherwise return `buffer` (heap-allocated, caller owns it).
 * - Notes:
 *   - `bytes` is the number of bytes read each iteration; when 0 we reached EOF.
 *   - We check for a '\n' in `buffer` with `ft_strchr` to stop early.
 */
static char *read_buffer(int fd, char *buffer)
{
    char    *temp;
    int     bytes;

    /* Temporary buffer for each read; +1 for null-terminator. */
    temp = malloc(BUFFER_SIZE + 1);
    if (!temp)
        return (NULL);
    bytes = 1; /* initialize to enter loop */

    /* Keep reading until we get a newline in `buffer` or EOF/error */
    while (bytes > 0 && !ft_strchr(buffer, '\n'))
    {
        bytes = read(fd, temp, BUFFER_SIZE);
        if (bytes < 0)
        {
            free(temp);
            return (NULL);
        }
        temp[bytes] = '\0';

        /* Append the newly read chunk to `buffer`.
         * ft_strjoin frees the old `buffer` and returns a new allocation.
         */
        buffer = ft_strjoin(buffer, temp);
    }
    free(temp);

    /* If buffer is NULL or an empty string, return NULL to indicate EOF/no-data. */
    if (!buffer)
        return (NULL);
    if (buffer[0] == '\0')
    {
        free(buffer);
        return (NULL);
    }
    return (buffer);
}


/*
 * get_line:
 * - Purpose: extract the first line (including the trailing '\n' if present)
 *   from `buffer` and return it as a newly allocated string.
 * - Parameters:
 *   - buffer: the combined data which may contain one or more lines.
 * - Behavior & ownership:
 *   - Finds the length of the first line (up to and including '\n').
 *   - Calls `ft_substr` to allocate and return that slice.
 *   - Returned string is heap-allocated and must be freed by the caller
 *     (in this project, `get_next_line` returns it to the user).
 */
static char *get_line(char *buffer)
{
    int i;

    if (!buffer)
        return (NULL);
    i = 0;
    /* Walk until end or newline */
    while (buffer[i] && buffer[i] != '\n')
        i++;
    /* If there is a newline, include it in the returned line */
    if (buffer[i] == '\n')
        i++;
    return (ft_substr(buffer, 0, i));
}


/*
 * get_rest:
 * - Purpose: remove the returned line from `buffer` and keep the remaining
 *   part (the 'rest') for future calls.
 * - Parameters:
 *   - buffer: heap-allocated string containing at least one line.
 * - Behavior & ownership:
 *   - Finds the first '\n'. If there is no '\n', there is no remainder:
 *     free `buffer` and return NULL.
 *   - Otherwise, allocate a new string containing everything after the '\n'
 *     (using `ft_substr`), free the old `buffer`, and return the new remainder.
 *   - The returned remainder is heap-allocated and will be stored in the
 *     static `buffer` inside `get_next_line` for the next call.
 */
static char *get_rest(char *buffer)
{
    int     i;
    char    *rest;

    if (!buffer)
        return (NULL);
    i = 0;
    while (buffer[i] && buffer[i] != '\n')
        i++;
    /* If we reached the end with no '\n', there's no remainder. */
    if (!buffer[i])
    {
        free(buffer);
        return (NULL);
    }
    /* Skip the '\n' and take the rest of the string */
    rest = ft_substr(buffer, i + 1, ft_strlen(buffer) - i - 1);
    free(buffer);
    return (rest);
}


/*
 * get_next_line:
 * - Public API: returns the next line from file descriptor `fd` or NULL.
 * - Important notes on behavior and ownership:
 *   - Uses a `static char *buffer` to keep leftover data between calls.
 *     This means the function preserves state across calls for the same
 *     process; in this implementation there is a single buffer (no per-fd
 *     storage), so switching between multiple fds may mix data.
 *   - On success, returns a heap-allocated string containing the next line
 *     (including the terminating '\n' when present). The caller must free it.
 *   - Returns NULL on error or when there is no more data to read.
 * - Steps:
 *   1) Validate `fd` and `BUFFER_SIZE` and use `read(fd, NULL, 0)` to check fd validity.
 *   2) Call `read_buffer` to ensure `buffer` contains data up to a '\n' or EOF.
 *   3) Use `get_line` to extract the line to return.
 *   4) Use `get_rest` to update the static `buffer` with leftover data.
 */
char *get_next_line(int fd)
{
    static char *buffer; /* persistent leftover between calls */
    char        *line;

    /* Validate input: negative fd, non-positive BUFFER_SIZE, or invalid fd */
    if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
        return (NULL);

    /* Fill `buffer` until it contains a newline or EOF. `buffer` may be NULL. */
    buffer = read_buffer(fd, buffer);
    if (!buffer)
        return (NULL);

    /* Extract the first line to return, then update `buffer` to the remainder. */
    line = get_line(buffer);
    buffer = get_rest(buffer);

    return (line);
}
