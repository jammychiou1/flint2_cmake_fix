/*
    Copyright (C) 2021 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include "fmpz_mpoly.h"

int
main(void)
{
    slong i, j, k;
    FLINT_TEST_INIT(state);

    flint_printf("used_vars....");
    fflush(stdout);

    for (i = 0; i < 20 * flint_test_multiplier(); i++)
    {
        fmpz_mpoly_ctx_t ctx;
        fmpz_mpoly_t f;
        fmpz_t one, fdeg;
        slong len, var;
        flint_bitcnt_t coeff_bits, exp_bits;
        int * used;

        fmpz_mpoly_ctx_init_rand(ctx, state, 20);
        fmpz_mpoly_init(f, ctx);
        fmpz_init(fdeg);
        fmpz_init_set_ui(one, 1);
        used = FLINT_ARRAY_ALLOC(ctx->minfo->nvars, int);

        for (j = 0; j < ctx->minfo->nvars; j++)
        {
            len = n_randint(state, 200);
            exp_bits = n_randint(state, 200) + 2;
            coeff_bits = n_randint(state, 100);

            fmpz_mpoly_randtest_bits(f, state, len, coeff_bits, exp_bits, ctx);

            for (k = n_randint(state, ctx->minfo->nvars); k > 0; k--)
            {
                var = n_randint(state, ctx->minfo->nvars);
                if (!fmpz_mpoly_evaluate_one_fmpz(f, f, var, one, ctx))
                {
                    flint_printf("FAIL: check evaluation success\n");
                    fflush(stdout);
                    flint_abort();
                }
            }

            fmpz_mpoly_used_vars(used, f, ctx);

            for (var = 0; var < ctx->minfo->nvars; var++)
            {
                fmpz_mpoly_degree_fmpz(fdeg, f, var, ctx);
                if ((fmpz_sgn(fdeg) <= 0) != !used[var])
                {
                    flint_printf("FAIL: checked used matches degree\n");
                    flint_printf("var = %wd\n", var);
                    flint_printf("deg: "); fmpz_print(fdeg); flint_printf("\n");
                    fflush(stdout);
                    flint_abort();
                }
            }
        }

        flint_free(used);
        fmpz_clear(one);
        fmpz_clear(fdeg);
        fmpz_mpoly_clear(f, ctx);
        fmpz_mpoly_ctx_clear(ctx);
    }

    FLINT_TEST_CLEANUP(state);

    flint_printf("PASS\n");
    return 0;
}

