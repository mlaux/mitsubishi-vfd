from machine import SPI, Pin
import time

# Constants
VFD_SPI_FREQUENCY = 100000

# VFD Commands
VFD_CMD_SET_WIDTH = 0x00
VFD_CMD_SET_DIMMER = 0x08
VFD_CMD_SET_REFRESH = 0xf6
VFD_CMD_SET_POSITION = 0xe0
VFD_CMD_SET_AUTO_INCREMENT = 0xf4
VFD_CMD_CURSOR_ON = 0x10
VFD_CMD_CURSOR_OFF = 0x80
VFD_CMD_DISPLAY_MODE = 0xf0
VFD_CMD_DEFINE_CHAR = 0xfc
VFD_CMD_OUTPUT_PORT = 0xf8

# Display Modes
DISP_MODE_ALL_OFF = 0x00
DISP_MODE_NORMAL = 0x01
DISP_MODE_ALL_ON = 0x03

# Initialize SPI
spi_pins = ("GP14", "GP16", "GP30")
spi = SPI(2, baudrate=VFD_SPI_FREQUENCY, polarity=0, phase=0, firstbit=SPI.MSB)
cs = Pin(2, mode=Pin.OUT, value=1)

def vfd_begin():
    cs(0)

def vfd_end():
    cs(1)

def vfd_write(b: int):
    spi.write(bytes([b]))
    time.sleep(0.001)  # 1ms delay

def vfd_write_byte(b: int):
    vfd_begin()
    vfd_write(b)
    vfd_end()

def vfd_write_string( text: str):
    cs(0)
    for char in text:
        vfd_write(ord(char))
    cs(1)

def vfd_init():
    vfd_set_width(16)
    vfd_set_display_mode(DISP_MODE_ALL_ON)
    time.sleep(1)
    vfd_set_display_mode(DISP_MODE_NORMAL)
    vfd_set_position(0)
    vfd_set_auto_increment(1)
    vfd_write_string("                ")

def vfd_set_width(width: int):
    vfd_write_byte(VFD_CMD_SET_WIDTH | ((width - 9) & 0x07))

def vfd_set_dimmer(dimmer: int):
    vfd_write_byte(VFD_CMD_SET_DIMMER | (dimmer & 0x07))

def vfd_set_refresh(freq: int):
    vfd_write_byte(VFD_CMD_SET_REFRESH | (freq & 0x01))

def vfd_set_position(pos: int):
    vfd_write_byte(VFD_CMD_SET_POSITION | (pos & 0x0f))

def vfd_set_auto_increment(auto_increment: int):
    vfd_write_byte(VFD_CMD_SET_AUTO_INCREMENT | (auto_increment & 0x01))

def vfd_enable_cursor():
    vfd_write_byte(VFD_CMD_CURSOR_ON)

def vfd_disable_cursor():
    vfd_write_byte(VFD_CMD_CURSOR_OFF)

def vfd_set_display_mode(disp_mode: int):
    vfd_write_byte(VFD_CMD_DISPLAY_MODE | (disp_mode & 0x03))

def vfd_define_custom_char(loc: int, data: list):
    vfd_write_byte(VFD_CMD_DEFINE_CHAR)
    vfd_write_byte(loc & 0x0f)
    for k in range(5):
        vfd_write_byte(data[k])

def vfd_write_output_port(state: int):
    vfd_write_byte(VFD_CMD_OUTPUT_PORT | (state & 0x03))
