/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yafakihi <yafakihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 12:00:00 by yafakihi          #+#    #+#             */
/*   Updated: 2025/12/30 12:00:00 by yafakihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Test 1: Basic multiple lines
void test_basic()
{
    printf("Test 1: Basic multiple lines\n");
    int fd = open("test.txt", O_RDONLY);
    char *line;
    int i = 1;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line %d: %s", i++, line);
        free(line);
    }
    close(fd);
    printf("\n");
}

// Test 2: Empty file
void test_empty_file()
{
    printf("Test 2: Empty file\n");
    int fd = open("empty.txt", O_RDONLY | O_CREAT, 0644);
    char *line = get_next_line(fd);
    if (line == NULL)
        printf("Correctly returned NULL for empty file\n");
    else
    {
        printf("Error: returned %s\n", line);
        free(line);
    }
    close(fd);
    printf("\n");
}

// Test 3: File without newline at end
void test_no_newline()
{
    printf("Test 3: File without newline at end\n");
    int fd = open("no_nl.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    write(fd, "Hello world", 11);
    close(fd);
    fd = open("no_nl.txt", O_RDONLY);
    char *line = get_next_line(fd);
    if (line)
    {
        printf("Line: %s", line);
        free(line);
    }
    line = get_next_line(fd);
    if (line == NULL)
        printf("Correctly returned NULL after last line\n");
    close(fd);
    printf("\n");
}

// Test 4: File with only newline
void test_only_newline()
{
    printf("Test 4: File with only newline\n");
    int fd = open("only_nl.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    write(fd, "\n", 1);
    close(fd);
    fd = open("only_nl.txt", O_RDONLY);
    char *line = get_next_line(fd);
    if (line)
    {
        printf("Line: %s", line);
        free(line);
    }
    line = get_next_line(fd);
    if (line == NULL)
        printf("Correctly returned NULL\n");
    close(fd);
    printf("\n");
}

// Test 5: Large line (larger than BUFFER_SIZE)
void test_large_line()
{
    printf("Test 5: Large line (> BUFFER_SIZE)\n");
    int fd = open("large.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    char large[1000];
    memset(large, 'A', 999);
    large[999] = '\n';
    write(fd, large, 1000);
    close(fd);
    fd = open("large.txt", O_RDONLY);
    char *line = get_next_line(fd);
    if (line && strlen(line) == 1000)
        printf("Correctly read large line\n");
    else
        printf("Error reading large line\n");
    free(line);
    close(fd);
    printf("\n");
}

// Test 6: Invalid fd
void test_invalid_fd()
{
    printf("Test 6: Invalid fd\n");
    char *line = get_next_line(-1);
    if (line == NULL)
        printf("Correctly returned NULL for invalid fd\n");
    else
    {
        printf("Error: returned %s\n", line);
        free(line);
    }
    printf("\n");
}

// Test 7: Read from stdin (simulate)
void test_stdin()
{
    printf("Test 7: Read from stdin (enter a line)\n");
    char *line = get_next_line(0);
    if (line)
    {
        printf("Read from stdin: %s", line);
        free(line);
    }
    printf("\n");
}

// Test 8: Multiple fds (note: static buffer may cause issues)
void test_multiple_fds()
{
    printf("Test 8: Multiple fds\n");
    int fd1 = open("test.txt", O_RDONLY);
    int fd2 = open("no_nl.txt", O_RDONLY);
    char *line1 = get_next_line(fd1);
    char *line2 = get_next_line(fd2);
    if (line1) printf("Fd1: %s", line1);
    if (line2) printf("Fd2: %s", line2);
    free(line1);
    free(line2);
    close(fd1);
    close(fd2);
    printf("\n");
}

// Test 9: BUFFER_SIZE edge cases (if changeable)
void test_buffer_size()
{
    printf("Test 9: BUFFER_SIZE = %d\n", BUFFER_SIZE);
    // Already tested implicitly
    printf("Current BUFFER_SIZE is %d\n", BUFFER_SIZE);
    printf("\n");
}

int main()
{
    test_basic();
    test_empty_file();
    test_no_newline();
    test_only_newline();
    test_large_line();
    test_invalid_fd();
    printf("For stdin test, run separately if needed.\n");
    // test_stdin(); // Uncomment to test stdin
    test_multiple_fds();
    test_buffer_size();
    return 0;
}