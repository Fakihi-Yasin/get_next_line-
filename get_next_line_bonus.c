/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yafakihi <yafakihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:00:00 by yafakihi          #+#    #+#             */
/*   Updated: 2026/01/07 02:38:53 by yafakihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char *read_buffer(int fd, char *buffer)
{
    char *temp = malloc(BUFFER_SIZE + 1);
    int bytes = 1;

    if (!temp) return NULL;
    while (bytes > 0 && !ft_strchr(buffer, '\n'))
    {
        bytes = read(fd, temp, BUFFER_SIZE);
        if (bytes < 0) { free(temp); return NULL; }
        temp[bytes] = '\0';
        buffer = ft_strjoin(buffer, temp);
        if (!buffer) { free(temp); return NULL; }
    }
    free(temp);
    if (!buffer || buffer[0] == '\0') { free(buffer); return NULL; }
    return buffer;
}

static char *get_line(char *buffer)
{
    int i;

    if (!buffer)
        return (NULL);
    i = 0;
    while (buffer[i] && buffer[i] != '\n')
        i++;
    if (buffer[i] == '\n')
        i++;
    return (ft_substr(buffer, 0, i));
}

static char *get_rest(char *buffer)
{
    int     i;
    char    *rest;

    if (!buffer)
        return (NULL);
    i = 0;
    while (buffer[i] && buffer[i] != '\n')
        i++;
    if (!buffer[i])
    {
        free(buffer);
        return (NULL);
    }
    rest = ft_substr(buffer, i + 1, ft_strlen(buffer) - i - 1);
    free(buffer);
    return (rest);
}

char *get_next_line(int fd)
{
    static char *buffers[OPEN_MAx];
    char *line;

    if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
        return NULL;
    buffers[fd] = read_buffer(fd, buffers[fd]);
    if (!buffers[fd])
        return NULL;
    line = get_line(buffers[fd]);
    buffers[fd] = get_rest(buffers[fd]);
    return line;
}
