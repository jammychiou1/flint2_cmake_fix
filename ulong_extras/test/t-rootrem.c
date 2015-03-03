/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2015 Kushagra Singh

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "ulong_extras.h"

int main(void)
{
   int i, result, j;
   mp_limb_t maxbits, max, upper_limit;

   FLINT_TEST_INIT(state);
   
   flint_printf("rootrem....");
   fflush(stdout);

   max = 0;
   upper_limit = 1626;
   maxbits = 32;


#if FLINT64
   maxbits = 64;
   upper_limit = 2642245;
#endif

   for (j = 0; j<maxbits;j++)
      max += n_pow(2, j);

   /* random n and root */

   for (i = 0; i < 10000 * 100; i++)
   {
      mp_limb_t a, b, c, d, i, j;
      mpz_t e, f, g, h;
      int res;

      mpz_init(e);
      mpz_init(f);
      mpz_init(g);
      mpz_init(h);
      
      c = n_randint(state, 0);    /*number */
      flint_mpz_set_ui(g, c);


      d = n_randint(state, 0);   /*root */
      flint_mpz_set_ui(h, d);

      res = n_rootrem(&a, &b, c, d);

      mpz_rootrem(e, f, g, flint_mpz_get_ui(h));
      
      i = flint_mpz_get_ui(e);
      j = flint_mpz_get_ui(f);

      result = (res && (a == i) && (b == j));

      if (!result)
      {
         flint_printf("FAIL:\n");
         flint_printf("Passed Parameters : n = %wu root = %wu", c, d);
         flint_printf("Answer generated : base = %wu remainder = %wu", a, b);
         flint_printf("Expected answer : base = %wu remainder = %wu", i, j);
         abort();
      }


      mpz_clear(e);
      mpz_clear(f);
      mpz_clear(g);
      mpz_clear(h);
   }

   /* n of type a^b */

   for (i = 0; i < 10000 * 100; i++)
   {
      mp_limb_t a, b, c, d, max_pow, base;
      int res;

      base = n_randint(state, upper_limit - 2) + 2;     /* base form 2 to 2642245*/
      max_pow = n_flog(max, base);    
      d = n_randint(state, max_pow);       /* root */
      if (!d)
        d+=1;
      c = n_pow(base, d);                  /* number */

      res = n_rootrem(&a, &b, c, d);
      

      result = (res && (a == base) && (b == 0));

      if (!result)
      {
         flint_printf("FAIL:\n");
         flint_printf("Passed Parameters : n = %wu root = %wu", c, d);
         printf("\n");
         flint_printf("Answer generated : base = %wu remainder = %wu", a, b);
         printf("\n");
         flint_printf("Expected answer : base = %wu remainder = 0", base);
         abort();
      }

   }

   /* n of type a^b + 1 */
   
   for (i = 0; i < 10000 * 100; i++)
   {
      mp_limb_t a, b, c, d, max_pow, base;
      int res;

      base = n_randint(state, upper_limit - 2) + 2;     /* base between 2 to 2642245*/
      max_pow = n_flog(max, base);    
      d = n_randint(state, max_pow);       
      if (d < 2)                                /* root between 2 to max_pow */
        d = 2;

      c = n_pow(base, d) + 1;                   /* number */

      res = n_rootrem(&a, &b, c, d);
      

      result = (res && (a == base) && (b == 1));

      if (!result)
      {
         flint_printf("FAIL:\n");
         flint_printf("Passed Parameters : n = %wu root = %wu", c, d);
         printf("\n");
         flint_printf("Answer generated : base = %wu remainder = %wu", a, b);
         printf("\n");
         flint_printf("Expected answer : base = %wu remainder = 1", base);
         abort();
      }

   }

   /* n of type a^b - 1 */
   
   for (i = 0; i < 10000 * 100; i++)
   {
      mp_limb_t a, b, c, d, i, j, max_pow, base;
      mpz_t e, f, g, h;
      int res;

      mpz_init(e);
      mpz_init(f);
      mpz_init(g);
      mpz_init(h);
 
      base = n_randint(state, upper_limit - 2) + 2;     /* base between 2 to 2642245*/
      max_pow = n_flog(max, base);    
      d = n_randint(state, max_pow);       
      if (d < 2)                                /* root between 2 to max_pow */
        d = 2;
      flint_mpz_set_ui(h, d);

      c = n_pow(base, d) - 1;                   /* number */
      flint_mpz_set_ui(g, c);

      res = n_rootrem(&a, &b, c, d);
      
      mpz_rootrem(e, f, g, flint_mpz_get_ui(h));
      
      i = flint_mpz_get_ui(e);
      j = flint_mpz_get_ui(f);

      result = (res && (a == i) && (b == j));

      if (!result)
      {
         flint_printf("FAIL:\n");
         flint_printf("Passed Parameters : n = %wu root = %wu", c, d);
         flint_printf("Answer generated : base = %wu remainder = %wu", a, b);
         flint_printf("Expected answer : base = %wu remainder = %wu", i, j);
         abort();
      }

   }

   FLINT_TEST_CLEANUP(state);
   
   flint_printf("PASS\n");
   return 0;
}