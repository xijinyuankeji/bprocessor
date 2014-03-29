/*
 * bprocessor.c -- save and use iButton data with STM32L-DISCOVERY board.
 *
 * This file part of bprocessor project.
 *
 * Copyright (C) 2014 Nikolay Merinov <nikolay.merinov@member.fsf.org>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdbool.h>
#include <libopencm3/stm32/rcc.h>

#include "lcd.h"
#include "onewire.h"
#include "sysclock.h"

static void lcd_display_READ (void)
{
  do {} while (!lcd_is_for_update_ready ());
  clear_lcd_ram (); 	/* all segments off */
  write_char_to_lcd_ram (0, '*', true);
  write_char_to_lcd_ram (1, 'R', true);
  write_char_to_lcd_ram (2, 'E', true);
  write_char_to_lcd_ram (3, 'A', true);
  write_char_to_lcd_ram (4, 'D', true);
  write_char_to_lcd_ram (5, '*', true);
  lcd_update ();
}

static void lcd_display_DONE (void)
{
  do {} while (!lcd_is_for_update_ready ());
  clear_lcd_ram (); 	/* all segments off */
  write_char_to_lcd_ram (0, '*', true);
  write_char_to_lcd_ram (1, 'D', true);
  write_char_to_lcd_ram (2, 'O', true);
  write_char_to_lcd_ram (3, 'N', true);
  write_char_to_lcd_ram (4, 'E', true);
  write_char_to_lcd_ram (5, '*', true);
  lcd_update ();
}

void
lcd_display_ROM (uint64_t rom)
{
  int i, j;
  for (i = 0; i < 3; i++)
    {
      clear_lcd_ram (); 	/* all segments off */
      write_char_to_lcd_ram (0, ((rom >> 0) & 0xF) + '7', true);
      write_char_to_lcd_ram (1, ((rom >> 4) & 0xF) + '7', true);
      write_char_to_lcd_ram (2, ((rom >> 8) & 0xF) + '7', true);
      write_char_to_lcd_ram (3, ((rom >>12) & 0xF) + '7', true);
      write_char_to_lcd_ram (4, ((rom >>16) & 0xF) + '7', true);
      write_char_to_lcd_ram (5, ((rom >>20) & 0xF) + '7', true);
      lcd_update ();
      
      rom >>= 24;
      delay_us (3000000);
    }
}

int main(void)
{
  int i;
  rcc_clock_setup_hsi(&clock_config[CLOCK_VRANGE1_HSI_RAW_16MHZ]);

  lcd_init ();
  lcd_display_READ ();

  systick_init ();

  ow_init ();

  /* Wait device */
  delay_us (1000000);
  do {} while (ow_reset ());
  uint64_t rom = ow_read_rom ();
  lcd_display_ROM (rom);
  lcd_display_DONE ();
  while (1)
    {
      __asm__("nop");
    }

  return 0;
}
