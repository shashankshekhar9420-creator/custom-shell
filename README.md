# Custom Shell v0.1

The first executable version of the shell.

This milestone extends the initial REPL by introducing process creation and program execution.
The shell runs external commands using fork(), exec(), wait() execution model used by UNIX.

---

## Features

- Interactive shell prompt using GNU Readline
- Read-Eval-Print Loop (REPL)
- Execute external programs using `fork()` and `execvp()`
- Parent process synchronization using `waitpid()`
- Display child process exit status
- Built-in `exit` command
- Graceful exit on `Ctrl+D`

---

## Current Limitations

- Executes only one command at a time
- No pipelines (`|`)
- No input/output redirection (`<`, `>`, `>>`)
- No background execution (`&`)
- No command chaining (`;`, `&&`, `||`)
- No built-in `cd`

These features will be implemented in future versions.

---

## Concepts Implemented

- Command-line tokenization using `strtok_r()`
- Construction of `argv` arrays
- Process creation with `fork()`
- Program execution with `execvp()`
- Parent-child synchronization with `waitpid()`
- Exit status handling using `WIFEXITED()` and `WEXITSTATUS()`

---

## Build

```bash
gcc main.c -o mysh -lreadline
```

---

## Run

```bash
./mysh
```

---

## Demo
<img width="708" height="324" alt="Screenshot 2026-06-10 192112" src="https://github.com/user-attachments/assets/023c122c-3825-4d39-9d05-24ce90712d7e" />

---

## Learning Milestone

This version implements the fundamental Unix shell execution cycle:

**Read → Parse → Fork → Exec → Wait → Repeat**

All future features including redirection, pipelines, job control, and command substitution will build upon this execution model.
