/*
 * Copyright(c) 2017 Tim Ruehsen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "../config.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include "fuzzer.h"

#ifdef TEST_RUN

# include <dirent.h>

static int
test_all_from (const char *dirname)
{
  DIR *dirp;
  struct dirent *dp;

  if ((dirp = opendir (dirname)))
    {
      while ((dp = readdir (dirp)))
	{
	  char *fname = NULL;
	  int fd;
	  struct stat st;
	  uint8_t *data;
	  ssize_t n;

	  if (*dp->d_name == '.')
	    continue;

	  if (asprintf (&fname, "%s/%s", dirname, dp->d_name) < 0)
	    {
	      fprintf (stderr, "malloc failure");
	      continue;
	    }

	  if ((fd = open (fname, O_RDONLY)) == -1)
	    {
	      fprintf (stderr, "Failed to open %s (%d)\n", fname, errno);
	      continue;
	    }

	  if (fstat (fd, &st) != 0)
	    {
	      fprintf (stderr, "Failed to stat %d (%d)\n", fd, errno);
	      close (fd);
	      continue;
	    }

	  data = malloc (st.st_size);
	  if ((n = read (fd, data, st.st_size)) == st.st_size)
	    {
	      printf ("testing %llu bytes from '%s'\n",
		      (unsigned long long) st.st_size, fname);
	      LLVMFuzzerTestOneInput (data, st.st_size);
	    }
	  else
	    fprintf (stderr,
		     "Failed to read %llu bytes from %s (%d), got %zd\n",
		     (unsigned long long) st.st_size, fname, errno, n);

	  free (data);
	  close (fd);
	}
      closedir (dirp);
      return 0;
    }

  return 1;
}

int
main (int argc, char **argv)
{
  const char *target = strrchr (argv[0], '/');
  target = target ? target + 1 : argv[0];

  (void) argc;

  {
    int rc;
    char *corporadir;

    if (asprintf (&corporadir, SRCDIR "/%s.in", target) < 0)
      {
	fprintf (stderr, "malloc failure");
	return EXIT_FAILURE;
      }

    rc = test_all_from (corporadir);
    if (rc)
      fprintf (stderr, "Failed to find %s\n", corporadir);

    if (asprintf (&corporadir, SRCDIR "/%s.repro", target) < 0)
      {
	fprintf (stderr, "malloc failure");
	return EXIT_FAILURE;
      }
    test_all_from (corporadir);
    if (test_all_from (corporadir) && rc)
      return 77;		// SKIP
  }

  return 0;
}

#else

# ifndef __AFL_LOOP
static int
__AFL_LOOP (int n)
{
  static int first = 1;

  if (first)
    {
      first = 0;
      return 1;
    }

  return 0;
}
# endif

int
main (void)
{
  int ret;
  unsigned char buf[64 * 1024];

  while (__AFL_LOOP (10000))
    {				// only works with afl-clang-fast
      ret = fread (buf, 1, sizeof (buf), stdin);
      if (ret < 0)
	return 0;

      LLVMFuzzerTestOneInput (buf, ret);
    }

  return 0;
}

#endif /* TEST_RUN */
