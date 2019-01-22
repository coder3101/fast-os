/**
 * Copyright 2019 Ashar <ashar786khan@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if !defined(__i386__)
#error "This file needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 8;
}

size_t strlen(const char* str) {
  size_t len = 0;
  while (str[len]) len++;
  return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_WHITE);
  terminal_buffer = (uint16_t*)0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
  if (c == '\n') {
    terminal_column = 0;
    terminal_row++;
    return;
  }
  if (c == '\t') {
    terminal_column += 4;
    return;
  }
  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) terminal_row = 0;
  }
}

void terminal_write(const char* data, size_t size) {
  for (size_t i = 0; i < size; i++) terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
  terminal_write(data, strlen(data));
}

void terminal_center_write(const char* data) {
  size_t write_offset = (VGA_WIDTH - strlen(data)) / 2;
  if (write_offset > VGA_WIDTH)
    return;  // Skip if we are to write a very big line.
  size_t i = 0;
  terminal_column = write_offset;
  for (; i < strlen(data); i++)
    terminal_putentryat(data[i], terminal_color, terminal_column++,
                        terminal_row);
}

void terminal_horizontal_line_of(char c) {
  terminal_row++;
  terminal_column = 0;
  for (int t = 0; t < 80; t++) terminal_putchar(c);
  terminal_putchar('\n');
}

void kernel_main(void) {
  terminal_initialize();
  terminal_setcolor(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
  terminal_center_write("Hello from Fast OS!");
  terminal_writestring("\n");
  terminal_center_write("Build with Love by coder3101");
  terminal_horizontal_line_of('*');
  terminal_center_write("Congratulations upon booting this new OS.");
  terminal_center_write("Now you should see some VGA color Tests..");
  terminal_horizontal_line_of('=');
  terminal_setcolor(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
  terminal_writestring(
      "\n\tThis text should be in Blue Color over white background\n");
  terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
  terminal_writestring(
      "\n\tThis text should be in White Color over black background\n");
  terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_LIGHT_BLUE));
  terminal_writestring(
      "\n\tThis text should be in Red Color over Light blue background\n");
  terminal_setcolor(vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_MAGENTA));
  terminal_writestring("\n\tThis text should be in Cyan Over Magenta.\n");
  terminal_setcolor(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
  terminal_horizontal_line_of('=');
  terminal_writestring(
      "\n\tThank you. Many other drivers and features will be added soon.");
}
