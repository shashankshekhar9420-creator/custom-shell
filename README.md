# Custom Shell
## Current Status
### Version 0.0

#### Implemented
* Interactive shell prompt using GNU Readline
* Read-Eval-Print Loop (REPL)
* Built-in `exit` command
* Graceful exit on `Ctrl+D`

---

### Build
```bash
gcc main.c -o mysh -lreadline
```
### Run
```bash
./mysh
```
