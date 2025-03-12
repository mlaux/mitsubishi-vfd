# mitsubishi-vfd

This is an Arduino library that allows the board to control vacuum
fluorescent displays based on the Mitsubishi M66004 as well as the M66005.

## Example

Connect the display's serial data input to the MOSI output of your Arduino,
and connect the display's clock input to the Arduino's SCK. For the Arduino
Uno, these pins are 11 and 13 respectively.

Finally, connect the display's Chip Select input to any available digital
I/O pin on the Arduino. I used pin 2 in the following example.

```c++
#include <SPI.h>
#include "mitsubishi_vfd.h"

#define CS_PIN 2
#define DISPLAY_WIDTH 16

vfd_t vfd;

void setup(void)
{
  SPI.begin();
  vfd_init(&vfd, CS_PIN, DISPLAY_WIDTH);

  vfd_set_position(&vfd, 0);
  vfd_write_string(&vfd, "helloooo!");
}
```

## Custom Character Generator
chargen.html is a simple JavaScript app for generating custom characters
in M66004 format. Toggle the pixels and the page will generate an array
for use with `vfd_define_custom_char()`.

Further documentation is available in `mitsubishi_vfd.h`.

## Micropython version

The micropython version was tested on an ESP32, but should work on other devices with SPI too. It allows for easy fiddling in the python REPL.