# Field Oriented Motor Controller | FORMOC module
BLDC motor controller using FOC algorithm by Wojciech Bohdan and Eryk Możdżeń.

## Project overview
The goal is to create an opportunity for easy implementation of BLDC motors to small autonomous robots like linefollower or minisumo.

Right now project is in mid development stage.

## Interface

```c
struct {
  uint8_t fdc : 6;  // field code
  uint8_t rw  : 1;  // read/write operation
  uint8_t mot : 1;  // left/right motor
} msg_header;
```

| Field                 | Code  | R/W    | Type          | Unit      | Details |
| -----                 | ----  | ---    | ----          | ----      | ------- |
| error flags           | 0x00  | r/w    | `uint8_t`     |           |         |
|||||||
| hardware config          | 0x0A  | r/w    | `uint8_t` <br> `uint16_t` <br> `uint16_t` | <br> count <br> count         | motor pole pairs <br> encoder CPR <br> encoder offset  |
| control mode          | 0x0B | r/w    | `uint8_t`     |           |           |
|||||||
| torque setpoint       | 0x10 | w      | `float32_t`   | Nm        |           |
| velocity setpoint     | 0x11 | w      | `float32_t`   | rps       |           |
|||||||
| rotor position        | 0x20 | r      | `uint16_t`    | count     | in range [0; encoder_CPR)
| rotor velocity        | 0x21 | r      | `float32_t`   | rps       |           |
| power supply voltage  | 0x23  | r     | `uint16_t`    | mV        |           |

# TODO
- improve code organization
- improve PID tunning
- find torque measurement method
- add interface
- add filter for phase current reading
