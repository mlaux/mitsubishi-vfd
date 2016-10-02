#include <SPI.h>

#include "mitsubishi_vfd.h"

static void vfd_begin(vfd_t *vfd)
{
  SPI.beginTransaction(SPISettings(VFD_SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
  digitalWrite(vfd->ss_pin, LOW);
}

static void vfd_end(vfd_t *vfd)
{
  digitalWrite(vfd->ss_pin, HIGH);
  SPI.endTransaction();
}

static void vfd_write(int b)
{
  SPI.transfer(b);
  delay(1);
}

void vfd_write_byte(vfd_t *vfd, int b)
{
  vfd_begin(vfd);
  vfd_write(b);
  vfd_end(vfd);
}

void vfd_write_string(vfd_t *vfd, const char *str)
{
  vfd_begin(vfd);
  while (*str) {
    vfd_write(*str);
    str++;
  }
  vfd_end(vfd);
}

void vfd_init(vfd_t *vfd, int ss_pin, int width)
{
  int c;

  vfd->ss_pin = ss_pin;
  pinMode(vfd->ss_pin, OUTPUT);

  vfd_set_width(vfd, 16);

  vfd_set_display_mode(vfd, DISP_MODE_ALL_ON);
  delay(1000);
  vfd_set_display_mode(vfd, DISP_MODE_NORMAL);

  vfd_set_position(vfd, 0);

  for (c = 0; c < vfd->width; c++) {
    vfd_write_byte(vfd, ' ');
  }
}

void vfd_set_width(vfd_t *vfd, int width)
{
  vfd->width = width;
  vfd_write_byte(vfd, VFD_CMD_SET_WIDTH | ((width - 9) & 0x07));
}

void vfd_set_dimmer(vfd_t *vfd, int dimmer)
{
  vfd->dimmer = dimmer;
  vfd_write_byte(vfd, VFD_CMD_SET_DIMMER | (dimmer & 0x07));
}

void vfd_set_refresh(vfd_t *vfd, int freq)
{
  vfd_write_byte(vfd, VFD_CMD_SET_REFRESH | (freq & 0x01));
}

void vfd_set_position(vfd_t *vfd, int pos)
{
  vfd->pos = pos;
  vfd_write_byte(vfd, VFD_CMD_SET_POSITION | (pos & 0x0f));
}

void vfd_set_auto_increment(vfd_t *vfd, int auto_increment)
{
  vfd_write_byte(vfd, VFD_CMD_SET_AUTO_INCREMENT | (auto_increment & 0x01));
}

void vfd_enable_cursor(vfd_t *vfd)
{
  vfd_write_byte(vfd, VFD_CMD_CURSOR_ON);
}

void vfd_disable_cursor(vfd_t *vfd)
{
  vfd_write_byte(vfd, VFD_CMD_CURSOR_OFF);
}

void vfd_set_display_mode(vfd_t *vfd, int disp_mode)
{
  vfd_write_byte(vfd, VFD_CMD_DISPLAY_MODE | (disp_mode & 0x03));
}

void vfd_define_custom_char(vfd_t *vfd, int loc, const int data[5])
{
  int k;

  vfd_write_byte(vfd, VFD_CMD_DEFINE_CHAR);
  vfd_write_byte(vfd, loc & 0x0f);
  for (k = 0; k < 5; k++) {
    vfd_write_byte(vfd, data[k]);
  }
}

void vfd_write_output_port(vfd_t *vfd, int state)
{
  vfd_write_byte(vfd, VFD_CMD_OUTPUT_PORT | (state & 0x03));
}

