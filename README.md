## Run It

From WSL, enter the exercise directory and compile it:

```bash
cd ~/mi_practica_reliable/21_reliable
make
```

Open two WSL terminals:

```bash
./reliable 5555 127.0.0.1:6666 -d 2
```

```bash
./reliable 6666 127.0.0.1:5555 -d 2
```

Test corruption with `-e 10`. Stop either endpoint with `Ctrl+C`.

For example, to enable 10% packet corruption on the first endpoint:

```bash
./reliable 5555 127.0.0.1:6666 -d 2 -e 10
```

## Test Result

The following local run shows packets, checksum validation, ACKs, timers, and transmission resuming after acknowledgement:

<img width="686" height="364" alt="Stop-and-wait test result" src="https://github.com/user-attachments/assets/8049ab61-60b3-4972-b61d-40d990c29b6f" />

## Files

- [`reliable.c`](21_reliable/reliable.c): protocol logic I implemented with AI help.
- [`rlib.h`](21_reliable/rlib.h): framework API and packet definitions.
- [`rlib.c`](21_reliable/rlib.c): framework internals; do not modify.
