## Project Overview

A simple Unix shell built in C aligned to `bash` features (command parsing, piping, redirections, and built-in commands)
---

## Features
- Prompt with working directory and last status
- Command execution with `$PATH` lookup
- Pipes `|` and redirections `<`, `>`, `>>`, `<<` (heredoc)
- Quotes: `'` literal, `"` expandable; backslash escapes outside single quotes
- Environment expansion: `$VAR`, `$?`
- Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Signals: interactive `SIGINT` handling; `SIGQUIT` Behaviour per subject
- Exit codes compatible with bash where applicable

---

## Usage

```bash
./minishell
minishell$ echo "Hello" | tr a-z A-Z > out
minishell$ cat < out
minishell$ cd $PATH
```

Heredoc:

```bash
minishell$ cat << EOF | grep Hello
Hello
World
EOF
```

---

## Builtins

| Name   | Synopsis                   | Notes                               |
| ------ | -------------------------- | ----------------------------------- |
| echo   | `echo [-n] [arg ...]`      | Only `-n`                           |
| cd     | `cd [dir]`                 | Updates `PWD`/`OLDPWD`              |
| pwd    | `pwd`                      | Prints cwd                          |
| export | `export [NAME[=VALUE]]...` | No options; prints env when no args |
| unset  | `unset NAME...`            | Remove variables                    |
| env    | `env`                      | No options                          |
| exit   | `exit [n]`                 | Exit shell with status `n`          |

> Run in parent when state must persist; in pipelines, forked children may run copies.

---

## Expansion

- `$VAR` → value or empty if undefined
- `$?` → last exit status
- Heredoc: quoted delimiter → no expansion; unquoted → expand in body

---

## Execution Model
- Build pipeline; for each node:
  - Set up FDs from redirs/pipes with `dup2`
  - If built-in and not forced to fork, execute in parent; else `fork` + `execve`
  - Resolve the exec path via `$PATH` unless the command contains `/`
- Close unused FDs; wait; set `$?` to last process status

---

## Signals
- Interactive: `SIGINT` cancels current line and sets status 130; `SIGQUIT` ignored
- Children: default handling; shell updates status on signal termination

---

## Author

Mohamed El hammouchi | *G1iTch*

42 Login: mel-hamm

