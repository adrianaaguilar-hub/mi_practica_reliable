# Reliable Protocol Lab

> **Computer Networks practice** | Reliable data transfer over UDP

This project implements **stop-and-wait ARQ** in C. The framework calls the protocol functions in [`21_reliable/reliable.c`](21_reliable/reliable.c).

## What I Implemented

| Callback | What it does |
| --- | --- |
| `connection_initialization` | Initializes sequence numbers, timeout, and state. |
| `send_callback` | Reads data, sends one packet, starts timer `0`, and pauses new data. |
| `receive_callback` | Validates packets, processes ACKs, accepts ordered data, and sends ACKs. |
| `timer_callback` | Retransmits the last packet if its ACK does not arrive. |

## Stop-and-Wait: Implemented ✅

Only one packet is pending at a time. The sender does not advance to packet `N + 1` until it receives ACK `N`.

```mermaid
sequenceDiagram
    participant A as Sender
    participant B as Receiver
    A->>B: DATA seq=1
    B->>B: Validate checksum
    B->>B: Accept data
    B-->>A: ACK 1
    A->>A: Stop timer and send next packet
    A-->>B: Retransmit DATA 1 if timer expires
```

`last_data` stores the current payload so the timer callback can retransmit it with the same sequence number.

## Sliding Window: Not Implemented Yet 🚧

Sliding window allows several packets to be sent before waiting for ACKs, improving throughput:

```mermaid
sequenceDiagram
    participant A as Sender
    participant B as Receiver
    A->>B: DATA 1
    A->>B: DATA 2
    A->>B: DATA 3
    B-->>A: ACK 1
    A->>B: DATA 4
    B-->>A: ACK 2
    B-->>A: ACK 3
```

This is an optional extension. The current code intentionally uses a window size of `1`.

## Run It

From `21_reliable/`:

```bash
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

## Files

- [`reliable.c`](21_reliable/reliable.c): protocol logic we implemented.
- [`rlib.h`](21_reliable/rlib.h): framework API and packet definitions.
- [`rlib.c`](21_reliable/rlib.c): framework internals; do not modify.
