# Custom Shell v0.3

This milestone introduces Unix pipelines.

Commands can now communicate through a pipe, allowing the output of one process to become the input of another.

## Added

* Single pipe (`|`) support
* Creation of anonymous pipes using `pipe()`
* Two-child process execution model
* Inter-process communication through file descriptors
* Input/output stream rewiring using `dup2()`

## Examples

### List files and filter output

```text
mysh> ls | grep main
main
main.c
```

### Count files

```text
mysh> ls | wc -l
```

## Concepts Implemented

* `pipe()`
* `fork()`
* `execvp()`
* `dup2()`
* `close()`
* Parent-child process coordination
* File descriptor inheritance
* Inter-process communication (IPC)

## Current Limitations

* Supports only one pipe
* No multiple pipelines (`cmd1 | cmd2 | cmd3`)
* No redirection combined with pipelines
* No background jobs (`&`)
* No signal handling
* No job control

## Demo

