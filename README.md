# sysmon_linux

A lightweight **Linux System Monitor** daemon written in C.  
Monitors CPU usage, RAM usage, and system uptime — logging results to a file and/or serving them over a TCP socket.

---

## Features

- **CPU Usage** — calculated from `/proc/stat` over a 2-second interval
- **RAM Usage** — read from `/proc/meminfo`
- **Uptime** — read from `/proc/uptime`
- **File Logging** — writes timestamped logs to `/tmp/sysmon.log`
- **TCP Socket** — clients can connect on port `9000` to trigger a log
- **Timer Mode** — automatically logs stats every few seconds
- **Daemon** — runs fully in the background, detached from terminal
- **Thread-safe** — uses mutexes to prevent race conditions

---

## Project Structure

```
sysmon_linux/
├── main.c          # Entry point, argument parsing
├── daemon.c        # daemonize() — detach from terminal
├── daemon.h
├── timer.c         # Periodic timer using SIGEV_THREAD
├── timer.h
├── socket.c        # TCP server on port 9000
├── socket.h
├── logger.c        # Reads CPU, RAM, uptime from /proc
├── logger.h
├── file_writer.c   # Writes stats to /tmp/sysmon.log
├── file_writer.h
└── Makefile
```

---

## Build

```bash
make        # build
make re     # clean and rebuild
make fclean # remove all build artifacts
```

> Requires `gcc`, `pthread`, and `librt` (`-lrt`).

---

## Usage

```bash
./sysmon <option>
```

| Option   | Description                          |
|----------|--------------------------------------|
| `timer`  | Log stats every few seconds          |
| `socket` | Start TCP server on port 9000        |

### Examples

```bash
# run as daemon with timer
./sysmon timer

# run as daemon with socket server
./sysmon socket

# connect to socket server (from another terminal)
nc localhost 9000
```

---

## Log Output

Logs are written to `/tmp/sysmon.log` in this format:

```
[2026-04-19 10:01:00] [INFO] CPU Usage  : 3.45%
[2026-04-19 10:01:00] [INFO] RAM Usage  : 60.12%
[2026-04-19 10:01:00] [INFO] Up Time (s): 3600
=================================================
```

Watch logs live:
```bash
tail -f /tmp/sysmon.log
```

---

## Stop the Daemon

```bash
# find the PID
ps aux | grep sysmon

# stop it
kill $(pidof sysmon)
```

---

## How It Works

```
main()
  │
  ├── daemonize()        fork twice, detach from terminal
  │
  ├── timer mode:
  │     └── timer fires every N seconds
  │           └── logger_starter() ──► file_writer()
  │
  └── socket mode:
        └── accept() waits for clients
              └── each client → new thread
                    └── logger_starter() ──► file_writer()
                          └── send response to client
```

---

## Platform

Linux only — uses `/proc/stat`, `/proc/meminfo`, `/proc/uptime`.

---

## Author

**Behdad Damnab** — [github.com/BehdadDamnab](https://github.com/BehdadDamnab)