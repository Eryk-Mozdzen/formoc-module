# Field Oriented Motor Controller | FORMOC module
BLDC motor controller using FOC algorithm by Wojciech Bohdan and Eryk Możdżeń.

## Project overview
The goal is to create an opportunity for easy implementation of BLDC motors to small autonomous robots like linefollower or minisumo.

Right now project is in mid development stage.

## Interface

```c
struct {
  uint8_t fdc : 5;  // field code
  uint8_t tg  : 1;  // toggle
  uint8_t rw  : 1;  // read/write operation
  uint8_t mot : 1;  // left/right motor
} msg_header_tx;

struct {
  uint8_t fdc : 5;  // field code
  uint8_t tg  : 1;  // toggle
  uint8_t un  : 1;  // unused
  uint8_t mot : 1;  // left/right motor
} msg_header_rx;

struct {
  uint8_t sta : 7;  // status field
  uint8_t tg  : 1;  // toggle
} msg_hearbeat;
```

| Field                 | Code  | R/W    | Type          | Unit      | Details |
| -----                 | ----  | ---    | ----          | ----      | ------- |
| status                | 0x00  | r/w    | `uint8_t`     |           | bit 0 - MCP8024 error flag <br>bit 1 - read/write access error flag <br> bit 2 - read/write wrong toggle flag <br>  bit 3 - write timeout flag <br> bit 4 - write wrong param flag <br> bit 5-6 (unused) <br> bit 7 (reserved)        |
| control (toggle free)               | 0x01  | w    | `uint8_t`     |           | bit 0 - reset <br> bit 1-7 (unused)         |
|||||||
| hardware config          | 0x0A  | r/w    | `uint8_t` <br> `uint16_t` <br> `uint16_t` | <br> count <br> count         | motor pole pairs <br> encoder CPR <br> encoder offset  |
| Id controller config          | 0x0B  | r/w    | `float32_t` <br> `float32_t` <br> `float32_t` ||  Kp <br> Ki <br> Kd |
| Iq controller config          | 0x0C  | r/w    | `float32_t` <br> `float32_t` <br> `float32_t` ||  Kp <br> Ki <br> Kd |
| velocity controller config          | 0x0D  | r/w    | `float32_t` <br> `float32_t` <br> `float32_t` ||  Kp <br> Ki <br> Kd |
|||||||
| velocity setpoint     | 0x11 | w      | `float32_t`   | rps       |           |
|||||||
| rotor position        | 0x20 | r      | `int32_t`     | count     |
| rotor velocity        | 0x21 | r      | `float32_t`   | rps       |           |
| power supply voltage  | 0x23 | r      | `uint16_t`    | mV        |           |
|||||||
| torque limit          | 0x30 | w      | `float32_t`   | Nm        |           |
| velocity limit        | 0x31 | w      | `float32_t`   | rps       |           |

# TODO
- improve code organization
- improve PID tunning
- find torque measurement method
- add interface
- add filter for phase current reading
