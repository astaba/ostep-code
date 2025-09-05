# Overview

Code from OSTEP chapter
[Introduction](http://pages.cs.wisc.edu/~remzi/OSTEP/intro.pdf).

To compile, just type:

```bash
prompt> make
```

See the highly primitive `Makefile` for details.

Then, run them! Examples:

```bash
prompt> ./cpu A
prompt> ./mem 1
prompt> ./threads 10000
prompt> ./io
```

## Details

One issue with mem.c is that address space randomization is usually on by
default. To turn it off:

> **ASLR** stands for **Address Space Layout Randomization**.
>
> It's a security feature in modern operating systems that randomly arranges
the memory addresses of a program's executable, stack, heap, and shared
libraries. The goal is to make it much harder for an attacker to predict
the location of code they want to exploit, such as in a buffer overflow attack.

### macOS
From
[stackoverflow](http://stackoverflow.com/questions/23897963/documented-way-to-disable-aslr-on-os-x)

Just compile/link as follows: `gcc -o mem mem.c -Wall -Wl,-no_pie`

### Linux

From Giovanni Lagorio:

Under Linux you can disable ASLR, without using a debugger, in (at least)
two ways:

* Use the command setarch to run a process with ASLR disabled; I typically
run bash, with which I can execute examples, like this: `setarch $(uname
--machine) --addr-no-randomize /bin/bash` where the active option can be
shortened to `-R`.
* Writing 0 into `/proc/sys/kernel/randomize_va_space`; you need to be root
to do this and this change has (a non-permanent) effect on the whole system,
which is something you probably don't want. I use this one only inside VMs.

---

## cpu.c

This code is a classic example of a **busy-wait loop**, a technique used in
operating systems and concurrent programming. Your confusion is completely
understandable because it behaves differently from what you might expect.

Let's break down each part of your query.

### What is `gettimeofday()`?

`gettimeofday()` is a Unix system call that retrieves the system's current
time. It fills a `struct timeval` with the time elapsed since the Unix Epoch
(January 1, 1970). The key thing is its precision: it can return the time
with **microsecond accuracy**.

  * `t.tv_sec`: The number of seconds.
  * `t.tv_usec`: The number of microseconds within the current second.

The function `GetTime()` combines these two values into a single `double`
for easier calculation: `(double)t.tv_sec + (double)t.tv_usec / 1e6`. This
gives you a highly precise floating-point number representing the current
time in seconds, with up to six decimal places of precision.

### How does `Spin()` work?

You are right that the time between two calls to `GetTime()` is very short,
but that's exactly the point.

The `Spin()` function implements a **busy-wait** or **polling** loop. Instead
of telling the CPU to "sleep" and do nothing, it keeps the CPU busy with an
empty loop, constantly checking a condition.

Here's the step-by-step process:

1.  It gets the starting time with microsecond precision: `t = GetTime();`.
2.  It enters a `while` loop.
3.  Inside the loop, the CPU **repeats the following two actions as fast as
it can**:
      * Call `GetTime()` to get the current time.
      * Subtract the starting time `t` to see how much time has passed.
4.  The loop continues until the time elapsed is greater than or equal to
`howlong` (which is 1 second in your `main()` function).

The CPU is incredibly fast. In a single second, a modern CPU can perform
hundreds of millions or even billions of operations. The `while` loop is a
single instruction, and the `GetTime()` call is a quick system call. The loop
will run continuously, using 100% of a CPU core's resources, until exactly
the specified time has elapsed. It's a very inefficient way to wait, as it
wastes CPU cycles, but it's a simple way to implement a precise delay.

### Is this an ODR violation?

This is a great observation. In standard C, if you define a function in a
header file and include that header in more than one `.c` file, you will
indeed get a **One Definition Rule (ODR)** violation from the linker.

However, your code is likely to work for one of two reasons:

1.  **Single Compilation Unit:** You might only be compiling a single `.c`
file that includes the header. If there's only one source file, there's no
linking issue.

2.  **`inline` or `static` keywords:** A proper, portable solution would
use the `static` or `inline` keywords in the header file.

    ```c
    // Using inline to prevent ODR violation
    static double GetTime() { /* ... */ }
    static void Spin(int howlong) { /* ... */ }
    ```

The `inline` keyword tells the compiler that it's okay for the same function
to be defined in multiple files, and the linker will resolve it correctly. The
`static` keyword would make a separate, private copy of the function in each
file that includes the header.

### Running many programs at once

The reason `Figure 2.2` command doesn't work as expected is the semicolon
(`;`) used in conjunction with the ampersand (`&`). While a semicolon is a
command separator, the ampersand already detaches the process and runs it
in the background, making the semicolon redundant.

To run multiple instances of a program in the background, you just need to
separate each command with an ampersand (`&`).

**Correct Command:** The simplest way is to put all the commands on a single
line, each followed by an `&`.

```bash
./cpu.out A & ./cpu.out B & ./cpu.out C & ./cpu.out D &
```

**Better Alternative: Using a Loop:** For a more scalable and cleaner solution,
you can use a `for` loop, especially if you have many instances to run.

```bash
for arg in A B C D; do
    ./cpu.out "$arg" &
done
```

This loop iterates through a list of arguments and starts a new background
process for each one. This is a best practice as it is less prone to syntax
errors and is easier to read and maintain.

### Kill all instances

You can kill all instances of `cpu.out` with a single command. The easiest
and most common way is to use a combination of `pkill` or `pgrep` and `xargs`.

**Recommended Command:** The simplest way is with `pkill`.

```bash
pkill cpu.out
```

This command searches for processes by name and sends a termination signal
to all of them. It's a single, clean command.

**Alternative Method:** A more explicit way to do the same thing is to use
`pgrep` and `xargs`.

```bash
pgrep cpu.out | xargs kill
```

Here's how that command works:

  * **`pgrep cpu.out`**: This command finds the Process IDs (PIDs) of all
  running processes whose name matches `cpu.out`. It then prints these PIDs
  to standard output.
  * **`|`**: The pipe operator takes the output of `pgrep` and uses it as
  the input for the next command.
  * **`xargs kill`**: This command reads the PIDs from the standard input
  and passes each PID as an argument to the `kill` command.

If the processes are not responding to the normal `kill` command, you can
use the more forceful signal `SIGKILL` (signal number 9), which cannot be
ignored by the process.

```bash
pkill -9 cpu.out
```

## open() file permission flags

### The `open()` Flags

The `open()` system call uses bitwise flags to control how a file is
opened. Your code uses a combination of three flags, which are combined
using the bitwise OR operator (`|`).

1.  **`O_WRONLY`**: This is a mandatory flag that specifies the file access
mode. `O_WRONLY` indicates that you are opening the file for **write-only**
access. This means you can write data to the file, but you cannot read from
it using the returned file descriptor.

2.  **`O_CREAT`**: This is a file creation flag. It instructs the operating
system to **create the file** if it does not already exist. If you did not
include this flag and the file wasn't present, the `open()` call would fail
with an error.

3.  **`O_TRUNC`**: This stands for "truncate." If the file already exists
and is successfully opened for writing, `O_TRUNC` **truncates the file's
length to zero**. This effectively erases the file's previous contents, so
you are starting with a clean slate. Without this flag, new data would be
written starting at the beginning of the file, overwriting the old content
but potentially leaving old data at the end of the file.

### The `S_I` Prefix and the `S_` Prefix

The `S_IXXX` family of flags are part of the `mode` argument to `open()` and
other system calls that create files, such as `creat()` and `mkdir()`. They
are used to specify the **file permissions** for the newly created file.

The `S_IXXX` flags correspond to the standard Unix file permissions: read,
write, and execute for three categories of users: the file's owner, the
group owner, and all other users. They are bitwise flags, which is why you
can combine them with the bitwise OR (`|`) operator.

The `S_` prefix stands for **"Status"** or **"Stat"**, as these flags
are also used with the `stat()` system call which retrieves file status
information. The `I` stands for **"Inode"**, as permissions are stored as
part of the file's inode metadata. So, `S_I` simply refers to the status
bits related to a file's inode.

### Owner Permissions (`S_I`**`R`**`USR`, `S_I`**`W`**`USR`, `S_I`**`X`**`USR`)

* **`S_IRUSR`**: (Read User) This grants **read** permission to the file's
owner. This is the `r` in the `rwx` permission set for the owner.
* **`S_IWUSR`**: (Write User) This grants **write** permission to the file's
owner. This is the `w` in the `rwx` permission set for the owner.
* **`S_IXUSR`**: (Execute User) This grants **execute** permission to the
file's owner. This is the `x` in the `rwx` permission set for the owner.

In your code, `S_IRUSR | S_IWUSR` sets permissions so that the user who owns
the file can both read from and write to it.

### Group Permissions (`S_I`**`R`**`GRP`, `S_I`**`W`**`GRP`, `S_I`**`X`**`GRP`)

* **`S_IRGRP`**: (Read Group) Grants **read** permission to users in the
file's group.
* **`S_IWGRP`**: (Write Group) Grants **write** permission to users in the
file's group.
* **`S_IXGRP`**: (Execute Group) Grants **execute** permission to users in
the file's group.

### Other Permissions (`S_I`**`R`**`OTH`, `S_I`**`W`**`OTH`, `S_I`**`X`**`OTH`)

* **`S_IROTH`**: (Read Other) Grants **read** permission to all other users.
* **`S_IWOTH`**: (Write Other) Grants **write** permission to all other users.
* **`S_IXOTH`**: (Execute Other) Grants **execute** permission to all
other users.

### Combining Flags and Octal Equivalents

When you use a combination of these flags, it's equivalent to the more
familiar octal permission notation. For example:

* `S_IRUSR | S_IWUSR` is equivalent to `0600` (read and write for owner only).
* `S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP` is equivalent to `0660` (read/write
for owner and group).
* `S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH` is
equivalent to `0755` (read/write/execute for owner, read/execute for group
and others).

### The `S_I` Family of Shorthand Flags

These flags are convenient macros that combine the individual read, write,
and execute permissions for a specific user category.

1.  **`S_IRWXU`**: **(Read, Write, and Execute for User)**. This is a shorthand
for `S_IRUSR | S_IWUSR | S_IXUSR`. It's the octal equivalent of `0700`.

2.  **`S_IRWXG`**: **(Read, Write, and Execute for Group)**. This is a
shorthand for `S_IRGRP | S_IWGRP | S_IXGRP`. It's the octal equivalent of
`0070`.

3.  **`S_IRWXO`**: **(Read, Write, and Execute for Other)**. This is a
shorthand for `S_IROTH | S_IWOTH | S_IXOTH`. It's the octal equivalent of
`0007`.

### The Complete Set of `S_I` Flags

So, when you see a line like this:

```c
int fd = open("./tmp/file", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
```

You could rewrite it using the shorthand flag to be more concise:

```c
// Same as S_IRUSR | S_IWUSR
int fd = open("./tmp/file", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
```

Wait, that's not quite right. `S_IRWXU` includes execute permission, which
you didn't have. You would need to use `S_IRUSR | S_IWUSR`. Let's correct that.

The shorthand flags are useful when you want all three permissions. If you
only want a subset, you use the individual flags.

### The `S_IFMT` Family

There's another family of `S_` flags, `S_IFMT`, which are used to describe
the **type** of file, not its permissions. They are not used in `open()`'s
`mode` argument, but they are returned by `stat()` to tell you if a file is
a regular file, a directory, a symbolic link, etc.

  * **`S_IFREG`**: Regular file
  * **`S_IFDIR`**: Directory
  * **`S_IFLNK`**: Symbolic link
  * **`S_IFCHR`**: Character device
  * **`S_IFBLK`**: Block device

These flags are typically used with the `S_IS...()` macros to check the
file type, for example, `S_ISDIR(mode)` checks if the mode bits indicate
a directory.

## The Purpose of `fsync()`

The `fsync()` system call is crucial for ensuring data durability and
integrity. When a program writes data to a file using `write()`, the operating
system often caches the data in memory (in a location called the "buffer
cache" or "page cache") before writing it to the physical storage device
(like an SSD or hard drive). This caching is done for performance reasons.

`fsync()`'s purpose is to **force all of the cached, modified data for a
specific file to be written from the buffer cache to the physical storage
device**. It blocks (pauses) the calling program until the write operation
has been completed by the disk and confirmed by the operating system.

Without `fsync()`, if your program were to crash or the system were to lose
power after the `write()` call but before the data was actually written to the
disk, the data would be lost. By calling `fsync()`, you are explicitly asking
the kernel to flush the data and guarantee its persistence on non-volatile
storage.

In your program, `fsync()` is used to ensure that the "hello world\n"
string has been completely written and saved to the `/tmp/file` before the
program finishes.
