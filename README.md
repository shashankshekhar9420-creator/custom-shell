# Custom Shell v0.2

This milestone introduces process creation, program execution, and I/O redirection.

## Added

- Command tokenization using `strtok_r()`
- Process creation with `fork()`
- Program execution with `execvp()`
- Parent-child synchronization using `waitpid()`
- Input redirection (`<`)
- Output redirection (`>`)
- Output append redirection (`>>`)
- Child exit status reporting

## Examples
### Redirect output

```text
mysh> ls > files.txt
```

### Append output

```text
mysh> ls >> files.txt
```

### Redirect input

```text
mysh> wc < files.txt
```

## Concepts Implemented

- Argument vector construction
- File descriptor manipulation
- Command parsing with `strtok_r()`
- `open()`
- `dup2()`
- `close()`
- `fork()`
- `execvp()`
- `waitpid()`

## Demo
<img width="465" height="668" alt="Screenshot 2026-06-11 001701" src="https://github.com/user-attachments/assets/bd2b6647-887f-4b66-852a-ccf4e9187709" />


