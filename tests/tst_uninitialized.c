/* tst_uninitialized.c --- use-of-uninitialized-value trigger
 * Copyright (C) 2022-2026 Simon Josefsson
 *
 * This file is part of GNU Libidn.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <idna.h>
#include <idn-free.h>

#include "utils.h"

void
doit (void)
{
  char *out;
  int rc;

  /* https://lists.gnu.org/archive/html/help-libidn/2026-05/msg00000.html
     Reproducible with valgrind or clang MSAN using old test vector:
     fuzz/libidn_tounicode_fuzzer.in/99904c3ca7c50905adbe8d835c5382051678cdb9
   */

  rc = idna_to_unicode_8z8z ("\xcb\xa3\x4e\x2d\x2d\x7f\x2d", &out, 0);
  if (rc != IDNA_SUCCESS)
    {
      fail ("idna_to_unicode_8z8z failed: %d\n", rc);
      return;
    }

  idn_free (out);
}
