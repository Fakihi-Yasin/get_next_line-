
*This project has been created as part of the 42 curriculum by yafakihi.*
 
### Description
- **Goal:** Implement the `get_next_line` function in C: a function that returns the next line read from a file descriptor on each call. The project focuses on correct memory management, handling partial reads (using a fixed `BUFFER_SIZE`), and supporting multiple file descriptors for the bonus version.
- **Overview:** The implementation reads data in chunks of `BUFFER_SIZE` from a file descriptor, accumulates leftover bytes between calls in a persistent buffer, extracts a single line (including the terminating `\n` when present), and returns that line as a newly-allocated string that the caller must free.


### Instructions

- The project is compiled using `cc` with the following flags:

```bash
cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c -D BUFFER_SIZE=32
```

- To compile with a custom buffer size:

```bash
cc -D BUFFER_SIZE=42 -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c
```

- Bonus version (multiple file descriptors support):

```bash
cc -Wall -Wextra -Werror get_next_line_bonus.c get_next_line_utils_bonus.c -D BUFFER_SIZE=32
```

- Quick Valgrind example (small buffer case):

```bash
cc -D BUFFER_SIZE=1 gnl_reader.c get_next_line_impl.c get_next_line_utils.c -o gnl_reader_bs1
valgrind --leak-check=full ./gnl_reader_bs1 test_files/large.txt
```
**Usage examples**
- Simple usage from a C program:

```bash
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd;
    char *line;

    fd = open("test.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)))
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```bash
---Bonus
int main(){
	int fd1 = open("file1.txt", O_RDONLY);
	int fd2 = open("file2.txt", O_RDONLY);
	int fd3 = open("file3.txt", O_RDONLY);
	char *line;
	int round = 0;
	while (round <= 2){
		line = get_next_line(fd1);
		printf("F1 : %s\n", line);
		free(line);

		line = get_next_line(fd2);
		printf("F2 : %s\n", line);
		free(line);

		line = get_next_line(fd3);
		printf("F3 : %s\n", line);
		free(line);

		round++;
	}
	close(fd1);
	close(fd2);
	close(fd3);
    return (0);
}

```

### Resources
```bash 
Linux read() manual:
https://man7.org/linux/man-pages/man2/read.2.html

File descriptors overview:
https://man7.org/linux/man-pages/man2/open.2.html

Static variables in C:
https://en.cppreference.com/w/c/language/storage_duration

BUFFER and memory management concepts:
https://man7.org/linux/man-pages/man3/malloc.3.html

```
**Algorithm explanation & justification**
- The implementation uses a persistent buffer to store leftover bytes between calls. For the bonus, a separate buffer is kept per file descriptor.

- On each `get_next_line(fd)` call:
	1. Validate parameters (`fd`, `BUFFER_SIZE`) and probe the descriptor.
 2. Repeatedly `read(fd, tmp, BUFFER_SIZE)` into a temporary buffer and append the data to the persistent buffer until a newline character is present in the buffer or `read` returns 0/EOF.
 3. Extract the next line from the buffer: copy up to and including the first `\n` (if present) into a new allocation and return it.
 4. Keep the remainder (bytes after the returned line) in the persistent buffer for the next call; free the buffer and set to `NULL` when empty.

- Justification:
	- Reading in fixed-size chunks balances system-call cost and memory usage.
	- Persisting leftovers between calls allows returning lines exactly as required (including cases where a line spans multiple reads).
	- Per-FD buffers (bonus) enable interleaved reads on multiple open descriptors.

**Complexity**
- Time: O(n) total for n bytes read (each byte is appended/processed a constant number of times).
- Space: temporary read buffer `BUFFER_SIZE + 1` plus the persistent remainder buffer. Each returned line is separately allocated and must be freed by the caller.

**Testing & validation**
- This repository includes several test programs and sample files in `test_files/` for edge cases (empty input, no trailing newline, very large line, many small lines). Use Valgrind to check memory safety across `BUFFER_SIZE` values.

**Notes & implementation choices**
- Utility functions (`ft_strlen`, `ft_strdup`, `ft_strjoin`, `ft_strchr`, `ft_substr`) are implemented in `get_next_line_utils.c`. The `ft_strjoin` in this project frees its first argument (`s1`) to simplify ownership in the read loop.

