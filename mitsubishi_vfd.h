#ifndef __MITSUBISHI_VFD_H
#define __MITSUBISHI_VFD_H

typedef struct _vfd_t {
  int width;
  int pos;
  int dimmer;

  int ss_pin;
} vfd_t;

#define VFD_SPI_FREQUENCY          100000

#define VFD_CMD_SET_WIDTH          0x00
#define VFD_CMD_SET_DIMMER         0x08
#define VFD_CMD_SET_REFRESH        0xf6
#define VFD_CMD_SET_POSITION       0xe0
#define VFD_CMD_SET_AUTO_INCREMENT 0xf4
#define VFD_CMD_CURSOR_ON          0x10
#define VFD_CMD_CURSOR_OFF         0x80
#define VFD_CMD_DISPLAY_MODE       0xf0
#define VFD_CMD_DEFINE_CHAR        0xfc
#define VFD_CMD_OUTPUT_PORT        0xf8

#define DISP_MODE_ALL_OFF          0x00
#define DISP_MODE_NORMAL           0x01
#define DISP_MODE_ALL_ON           0x03

void vfd_init(vfd_t *vfd, int ss_pin, int width);

void vfd_write_byte(vfd_t *vfd, int b);
void vfd_write_string(vfd_t *vfd, const char *str);

/* Sets the number of characters on the display. Additional characters are
   turned off. */
void vfd_set_width(vfd_t *vfd, int width);

/* Sets the brightness of the display. 0 = dimmest, 7 = brightest */
void vfd_set_dimmer(vfd_t *vfd, int dimmer);
void vfd_set_refresh(vfd_t *vfd, int freq);

/* Sets the cursor position where subsequent characters will be written. */
void vfd_set_position(vfd_t *vfd, int pos);

/* If enabled, the cursor will automatically move to the next position after
   a character is written. */
void vfd_set_auto_increment(vfd_t *vfd, int auto_increment);

/* Enables and disables the cursor (some displays do not have a cursor
   segment) */
void vfd_enable_cursor(vfd_t *vfd);
void vfd_disable_cursor(vfd_t *vfd);

/* Call with the symbolic constant DISP_MODE_ALL_OFF, DISP_MODE_NORMAL,
   or DISP_MODE_ALL_ON. */
void vfd_set_display_mode(vfd_t *vfd, int disp_mode);

/* Copies the `data` array to the M66004's memory to define a custom
   character. `loc` can be between 0 and 15. The character can then be
   accessed by displaying ASCII codes 0x90 through 0x9f. 

   Each element of `data` represents a column of the character. chargen.html
   features an interactive character generator that provides an array
   to use with this function. */
void vfd_define_custom_char(vfd_t *vfd, int loc, const int data[5]);

/* Controls the M66004's 2 GPIO pins. `state` can be between 0 and 3. */
void vfd_write_output_port(vfd_t *vfd, int state);

#endif

