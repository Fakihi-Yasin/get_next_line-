/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yafakihi <yafakihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 15:44:54 by yafakihi          #+#    #+#             */
/*   Updated: 2026/01/01 04:55:32 by yafakihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char *read_buffer(int fd, char *buffer)
{
    char    *temp;
    int     bytes;

    temp = malloc(BUFFER_SIZE + 1);
    if (!temp)
        return (NULL);
    bytes = 1;
    while (bytes > 0 && !ft_strchr(buffer, '\n'))
    {
        bytes = read(fd, temp, BUFFER_SIZE);
        if (bytes < 0)
        {
            free(temp);
            return (NULL);
        }
        temp[bytes] = '\0';
        buffer = ft_strjoin(buffer, temp);
    }
    free(temp);
    if (!buffer)
        return (NULL);
    if (buffer[0] == '\0')
    {
        free(buffer);
        return (NULL);
    }
    return (buffer);
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
    static char *buffer;
    char        *line;

    if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
        return (NULL);

    buffer = read_buffer(fd, buffer);
    if (!buffer)
        return (NULL);

    line = get_line(buffer);
    buffer = get_rest(buffer);

    return (line);
}
