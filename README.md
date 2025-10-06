REA# MINI\_SHELL

A small, POSIX‑like shell in C. Reads a line, parses it, expands env vars and quotes, and executes via forks, pipes, and redirections. No bonus features.

---

## Project Overview

Goal: reproduce a minimal interactive shell with behaviour aligned to `bash` the covered features. Focus on lexer → parser → executor pipeline, file descriptors, signals, and environment.

**Key Topics**: lexical analysis, parsing, heredoc, processes, pipes, redirections, env management, signals, exit codes, and strict memory/error handling under 42 Norm.

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

> Run in parent when state must persist; in pipelines forked children may run copies.

---

## Parsing Rules

- Tokenise words, pipes, redirs, and heredoc delimiters
- Remove quotes after tokenisation; apply quote semantics to expansion
- Unclosed quotes or bad redirection targets → syntax error
- Whitespace separates tokens except inside quotes

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

## Exit Codes

- Command not found: 127
- Permission denied / exec format error: 126
- `exit` non‑numeric arg: print error, return 2
- Interrupted by `SIGINT`: 130; by `SIGQUIT`: 131

---

## Testing

```bash
./minishell -c 'export A=1; echo $A; unset A; echo $A'
./minishell -c 'printf "a\nB\nc\n" | grep -i b > out && wc -l < out'
./minishell -c 'cat <<EOF\nHello $USER\nEOF'
```

Compare behaviour to `/bin/bash` for edge cases.

---

## Limitations

- No job control (`fg`, `bg`, `jobs`)
- No command substitution `$(...)` or backticks
- No brace or arithmetic expansion
- No wildcard expansion unless you implement it via exec (not parsed by the shell)

---

## Author

Mohamed El hammouchi //G1iTch//\
42 Login: mel-hamm

