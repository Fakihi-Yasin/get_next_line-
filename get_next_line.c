/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yafakihi <yafakihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 15:44:54 by yafakihi          #+#    #+#             */
/*   Updated: 2025/12/27 15:44:55 by yafakihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

char	*get_next_line(int fd)
{
    static char	*buffer;
    char		*line;

    if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
        return (NULL);
    line = NULL;
    if (buffer)
    {
        line = ft_strjoin(line, buffer);
        if (ft_strchr(buffer, '\n'))
            return (line);
    }
}