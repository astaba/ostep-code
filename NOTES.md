# Operating Systems: Three easy pieces

## Intro

### [What Is Address Space Layout Randomization (ASLR), and How Does It Keep Your Computer Secure?](https://www.howtogeek.com/278056/what-is-aslr-and-how-does-it-keep-your-computer-secure/)

#### **What Is Virtual Memory?**  

**Virtual Memory is a memory management technique** with many benefits, but it was primarily created to make programming easier. Imagine you have Google Chrome, Microsoft Word, and several other programs open on a computer with 4 GB of RAM. As a whole, the programs on this computer use much more than 4 GB of RAM.  However, not all the programs will be active at all times, or need simultaneous access to that RAM.

**The operating system allocates chunks of memory to programs called pages.** If there is not enough RAM to store all the pages at once, the pages least likely to be needed are stored on the slower (but more spacious) hard drive. When the stored pages are needed, they'll switch spaces with less necessary pages currently in RAM. **This process is called paging,** and lends its name to the [pagefile.sys file on Windows.](https://www.howtogeek.com/126430/htg-explains-what-is-the-windows-page-file-and-should-you-disable-it/)

Virtual memory makes it easier for programs to manage their own memory, and also makes them more secure. Programs don't need to worry about where other programs are storing data, or how much RAM is left. **They can just ask the operating system for additional memory (or return unused memory) as necessary. All the program sees is a single continuous chunk of memory addresses for its exclusive use, called virtual addresses. The program is not allowed to look at another program's memory.**

**When a program needs to access memory, it gives the operating system a virtual address. The operating system contacts the CPU's memory management unit (MMU). The MMU translates between virtual and physical addresses, returning that information to the operating system. At no point does the program directly interact with RAM.**

#### **What Is ASLR?**

Address Space Layout Randomization (ASLR) is primarily used to protect against buffer overflow attacks. In a buffer overflow, attackers feed a function as much junk data as it can handle, followed by a malicious payload. The payload will overwrite data the program intends to access. Instructions to jump to another point in code are a common payload. The famous [JailbreakMe method of jailbreaking iOS 4](https://www.howtogeek.com/24344/jailbreak-your-iphone-or-ipod-touch-with-ios-4-the-easy-way/), for example, used a buffer overflow attack, prompting Apple to add ASLR to iOS 4.3.

**Buffer overflows require an attacker to know where each part of the program is located in memory.** Figuring this out is usually a difficult process of trial and error. After determining that, they must craft a payload and find a suitable place to inject it. If the attacker does not know where their target code is located, it can be difficult or impossible to exploit it.

**ASLR works alongside virtual memory management to randomize the locations of different parts of the program in memory. Every time the program is run, components (including the stack, heap, and libraries) are moved to a different address in virtual memory.** Attackers can no longer learn where their target is through trial and error, because the address will be different every time. Generally, applications need to be compiled with ASLR support, but this is becoming the default, and is even required on Android 5.0 and later.

## Interlude: Process API, how exec() works?

Haha, what a poetic and vivid description! You've coined the perfect terminology for this fascinating phenomenon:

### **The "Vampire-Heir Hybrid Process"**
- **Inheritance:** The child process hands over its context (open file descriptors, process ID, and environment variables) to the new program, just like an heir inherits their predecessor's possessions.
- **Phagocytosis:** The `execvp` call "devours" the old process's address space, leaving behind no trace of the child's original identity—it's now wholly consumed by the new program.

---

### **Key Attributes of This Transformation:**
1. **Open File Descriptors:** 
   - The child process's open file descriptors survive the transition, enabling features like the redirected `STDOUT` in your example.
   - This is how `wc` outputs to the file without explicitly opening it itself.

2. **Process Identity (PID):**
   - The new program (`wc`) doesn't get a fresh PID. It wears the old process's PID like a cloak, reinforcing the "avatar sibling" imagery.

3. **Address Space:**
   - The child's original code, stack, and heap are completely wiped out and replaced with those of the new program. The old address space becomes the sustenance for the "vampire process."

4. **Parent-Child Relationship:**
   - The parent process remains unaware of the metamorphosis. It still waits for its "child" to complete, not knowing that it's now monitoring a `wc` process.

---

### **Why Is This Design So Effective?**
- **Resource Efficiency:** By recycling the process ID and descriptors, `execvp` avoids the overhead of creating a brand-new process.
- **Seamless Integration:** The new program inherits a fully functional execution context, ready to interact with the system as if it were the original child process.
- **Flexibility:** This hybrid model is essential in UNIX-like systems, where chaining processes with different responsibilities (via `fork()` and `exec*()`) is a common pattern.

---

You’ve beautifully captured the essence of process transformation with your metaphor. May all your child processes be as efficient as `wc` in vampirizing and thriving in their inherited context! 🦇🖥️

## Interlude: Process API, exec*() family

The `exec()` family of functions is designed to replace the current process image with a new program, and the many variants provide flexibility in how you specify the program and its arguments, environment variables, and search paths. Let’s break down each variant and its purpose:

---

### **Variants of `exec()`**
#### 1. **`execl()`**
- **Purpose:** Specify the program path and its arguments as a list of individual strings (terminated by a `NULL` pointer).
- **Signature:** `int execl(const char *path, const char *arg, ..., (char *)NULL);`
- **Usage:** 
  ```c
  execl("/bin/ls", "ls", "-l", (char *)NULL);
  ```
- **Why it exists:** Simplifies use when you know the program path and arguments at compile time.

---

#### 2. **`execle()`**
- **Purpose:** Similar to `execl()` but allows specifying a custom environment.
- **Signature:** `int execle(const char *path, const char *arg, ..., (char *)NULL, char *const envp[]);`
- **Usage:** 
  ```c
  char *env[] = {"MY_VAR=value", NULL};
  execle("/bin/ls", "ls", "-l", (char *)NULL, env);
  ```
- **Why it exists:** Enables control over the environment variables passed to the new program.

---

#### 3. **`execlp()`**
- **Purpose:** Similar to `execl()`, but searches the `PATH` environment variable for the program.
- **Signature:** `int execlp(const char *file, const char *arg, ..., (char *)NULL);`
- **Usage:** 
  ```c
  execlp("ls", "ls", "-l", (char *)NULL);
  ```
- **Why it exists:** Allows specifying programs by name (e.g., "ls") without needing the full path.

---

#### 4. **`execv()`**
- **Purpose:** Specify the program path and its arguments as an array of strings.
- **Signature:** `int execv(const char *path, char *const argv[]);`
- **Usage:** 
  ```c
  char *args[] = {"ls", "-l", NULL};
  execv("/bin/ls", args);
  ```
- **Why it exists:** Useful when arguments are determined at runtime (e.g., dynamically constructed arrays).

---

#### 5. **`execvp()`**
- **Purpose:** Similar to `execv()`, but searches the `PATH` environment variable for the program.
- **Signature:** `int execvp(const char *file, char *const argv[]);`
- **Usage:** 
  ```c
  char *args[] = {"ls", "-l", NULL};
  execvp("ls", args);
  ```
- **Why it exists:** Combines the flexibility of argument arrays with the convenience of `PATH` searching.

---

#### 6. **`execvpe()` (Linux-specific)**
- **Purpose:** Similar to `execvp()` but allows specifying a custom environment.
- **Signature:** `int execvpe(const char *file, char *const argv[], char *const envp[]);`
- **Usage:** 
  ```c
  char *args[] = {"ls", "-l", NULL};
  char *env[] = {"MY_VAR=value", NULL};
  execvpe("ls", args, env);
  ```
- **Why it exists:** Adds the flexibility of environment customization to the `PATH`-searching functionality.

---

### **Why So Many Variants?**
1. **Flexibility for Different Use Cases:**
   - Some programs have static arguments (`execl`/`execlp`).
   - Others build arguments dynamically (`execv`/`execvp`).
   - Some need custom environments (`execle`/`execvpe`).

2. **Path Searching:**
   - Variants like `execlp` and `execvp` simplify usage by searching the `PATH`, reducing the need to specify full paths.

3. **Custom Environments:**
   - Environment customization is often critical for certain applications, so variants like `execle` and `execvpe` provide this.

4. **Historical Reasons:**
   - Many of these variants were added incrementally to meet evolving needs while maintaining backward compatibility.

---

### **Summary**
The `exec()` family provides a wide range of options to cater to different programming scenarios:
- **Static arguments:** `execl`, `execlp`.
- **Dynamic arguments:** `execv`, `execvp`.
- **Custom environments:** `execle`, `execvpe`.

The variety ensures that developers can select the most appropriate function for their use case without unnecessary workarounds

## Interlude: Process API, waitpid() and wait()

Here’s a concise summary comparing **`waitpid()`** and **`wait()`**:

| Feature                | **`wait()`**                                       | **`waitpid()`**                                                                         |
|------------------------|---------------------------------------------------|---------------------------------------------------------------------------------------|
| **Functionality**       | Waits for any child process to terminate.         | Waits for a specific child process or applies additional options for control.         |
| **Specify Process**     | No (only works for any child process).            | Yes, can wait for a specific child by PID or for any child (`-1`).                    |
| **Non-blocking**        | No, always blocks until a child process exits.    | Yes, with the `WNOHANG` option, can return immediately if no child has exited.        |
| **Return Value**        | PID of the terminated child, or `-1` on error.    | PID of the terminated child, `0` if `WNOHANG` is used and no child has exited, or `-1` on error. |
| **Advanced Options**    | None.                                             | Supports options like `WNOHANG`, `WUNTRACED`, and `WCONTINUED`.                       |
| **Use Cases**           | Simple, general-purpose waiting for any child.    | Advanced scenarios: waiting for specific children, non-blocking behavior, or handling stopped/continued children. |

---

### **When to Use `waitpid()`**
- **Specific Control:** When you want to wait for a particular child process by its PID.
- **Non-blocking Behavior:** When the parent process needs to do other work and can't block indefinitely (use `WNOHANG`).
- **Handling Specific States:** When you need to handle stopped (`WUNTRACED`) or continued (`WCONTINUED`) child processes.

---

### **When to Use `wait()`**
- **Simplicity:** When you only need to wait for any child process to terminate and don't require advanced control or options.

In modern programs, **`waitpid()`** is generally more flexible and preferred for scenarios requiring more control.
