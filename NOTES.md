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

