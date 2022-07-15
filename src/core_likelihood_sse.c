/*
    Copyright (C) 2016-2022 Tomas Flouri, Bruce Rannala and Ziheng Yang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact: Tomas Flouri <t.flouris@ucl.ac.uk>,
    Department of Genetics, Evolution and Environment,
    University College London, Gower Street, London WC1E 6BT, England
*/

#include "bpp.h"

double pll_core_root_loglikelihood_sse(unsigned int states,
                                       unsigned int sites,
                                       unsigned int rate_cats,
                                       const double * clv,
                                       const unsigned int * scaler,
                                       double * const * frequencies,
                                       const double * rate_weights,
                                       const unsigned int * pattern_weights,
                                       const unsigned int * freqs_indices,
                                       double * persite_lnl)
{
  unsigned int i,j,k;
  double logl = 0;

  const double * freqs = NULL;

  double term, term_r;

  unsigned int states_padded = (states+3) & 0xFFFFFFFC;

  __m128d xmm0, xmm1, xmm2, xmm3;

  for (i = 0; i < sites; ++i)
  {
    term = 0;
    for (j = 0; j < rate_cats; ++j)
    {
      freqs = frequencies[freqs_indices[j]];
      xmm3 = _mm_setzero_pd();

      for (k = 0; k < states_padded; k += 2)
      {
        /* load frequencies for current rate matrix */
        xmm0 = _mm_load_pd(freqs);

        /* load clv */
        xmm1 = _mm_load_pd(clv);

        /* multiply with frequencies */
        xmm2 = _mm_mul_pd(xmm0,xmm1);

        xmm3 = _mm_add_pd(xmm3,xmm2);

        freqs += 2;
        clv += 2;
      }

      term_r = ((double *)&xmm3)[0] + ((double *)&xmm3)[1];

      term += term_r * rate_weights[j];
    }

    /* compute site log-likelihood and scale if necessary */
    term = log(term);
    if (scaler && scaler[i])
      term += scaler[i] * log(PLL_SCALE_THRESHOLD);

    term *= pattern_weights[i];

    /* store per-site log-likelihood */
    if (persite_lnl)
      persite_lnl[i] = term;

    logl += term;
  }
  return logl;
}

double pll_core_root_loglikelihood_4x4_sse(unsigned int sites,
                                           unsigned int rate_cats,
                                           const double * clv,
                                           const unsigned int * scaler,
                                           double * const * frequencies,
                                           const double * rate_weights,
                                           const unsigned int * pattern_weights,
                                           const unsigned int * freqs_indices,
                                           double * persite_lnl)
{
  unsigned int i,j;
  double logl = 0;

  const double * freqs = NULL;

  double term, term_r;

  __m128d xmm0, xmm1, xmm2, xmm3, xmm4, xmm5;

  for (i = 0; i < sites; ++i)
  {
    term = 0;
    for (j = 0; j < rate_cats; ++j)
    {
      freqs = frequencies[freqs_indices[j]];

      /* load frequencies for current rate matrix */
      xmm0 = _mm_load_pd(freqs+0);
      xmm1 = _mm_load_pd(freqs+2);

      /* load clv */
      xmm2 = _mm_load_pd(clv+0);
      xmm3 = _mm_load_pd(clv+2);

      /* multiply with frequencies */
      xmm4 = _mm_mul_pd(xmm0,xmm2);
      xmm5 = _mm_mul_pd(xmm1,xmm3);

      /* add up the elements of xmm2 */
      xmm1 = _mm_add_pd(xmm4,xmm5);

      term_r = ((double *)&xmm1)[0] + ((double *)&xmm1)[1];

      term += term_r * rate_weights[j];

      clv += 4;
    }

    /* compute site log-likelihood and scale if necessary */
    term = log(term);
    if (scaler && scaler[i])
      term += scaler[i] * log(PLL_SCALE_THRESHOLD);

    term *= pattern_weights[i];

    /* store per-site log-likelihood */
    if (persite_lnl)
      persite_lnl[i] = term;

    logl += term;
  }
  return logl;
}

void pll_core_root_likelihood_vec_sse(unsigned int states,
                                      unsigned int sites,
                                      unsigned int rate_cats,
                                      const double * clv,
                                      const unsigned int * scaler,
                                      double * const * frequencies,
                                      const double * rate_weights,
                                      const unsigned int * pattern_weights,
                                      const unsigned int * freqs_indices,
                                      double * persite_lh)
{
  unsigned int i,j,k;

  const double * freqs = NULL;

  double term, term_r;

  unsigned int states_padded = (states+3) & 0xFFFFFFFC;

  __m128d xmm0, xmm1, xmm2, xmm3;

  for (i = 0; i < sites; ++i)
  {
    term = 0;
    for (j = 0; j < rate_cats; ++j)
    {
      freqs = frequencies[freqs_indices[j]];
      xmm3 = _mm_setzero_pd();

      for (k = 0; k < states_padded; k += 2)
      {
        /* load frequencies for current rate matrix */
        xmm0 = _mm_load_pd(freqs);

        /* load clv */
        xmm1 = _mm_load_pd(clv);

        /* multiply with frequencies */
        xmm2 = _mm_mul_pd(xmm0,xmm1);

        xmm3 = _mm_add_pd(xmm3,xmm2);

        freqs += 2;
        clv += 2;
      }

      term_r = ((double *)&xmm3)[0] + ((double *)&xmm3)[1];

      term += term_r * rate_weights[j];
    }

    persite_lh[i] = term;

    #if 0
    /* compute site log-likelihood and scale if necessary */
    term = log(term);
    if (scaler && scaler[i])
      term += scaler[i] * log(PLL_SCALE_THRESHOLD);

    term *= pattern_weights[i];

    /* store per-site log-likelihood */
    if (persite_lnl)
      persite_lnl[i] = term;

    logl += term;
    #endif
  }
}

void pll_core_root_likelihood_vec_4x4_sse(unsigned int sites,
                                          unsigned int rate_cats,
                                          const double * clv,
                                          const unsigned int * scaler,
                                          double * const * frequencies,
                                          const double * rate_weights,
                                          const unsigned int * pattern_weights,
                                          const unsigned int * freqs_indices,
                                          double * persite_lh)
{
  unsigned int i,j;

  const double * freqs = NULL;

  double term, term_r;

  __m128d xmm0, xmm1, xmm2, xmm3, xmm4, xmm5;

  for (i = 0; i < sites; ++i)
  {
    term = 0;
    for (j = 0; j < rate_cats; ++j)
    {
      freqs = frequencies[freqs_indices[j]];

      /* load frequencies for current rate matrix */
      xmm0 = _mm_load_pd(freqs+0);
      xmm1 = _mm_load_pd(freqs+2);

      /* load clv */
      xmm2 = _mm_load_pd(clv+0);
      xmm3 = _mm_load_pd(clv+2);

      /* multiply with frequencies */
      xmm4 = _mm_mul_pd(xmm0,xmm2);
      xmm5 = _mm_mul_pd(xmm1,xmm3);

      /* add up the elements of xmm2 */
      xmm1 = _mm_add_pd(xmm4,xmm5);

      term_r = ((double *)&xmm1)[0] + ((double *)&xmm1)[1];

      term += term_r * rate_weights[j];

      clv += 4;
    }

    persite_lh[i] = term;
    #if 0
    /* compute site log-likelihood and scale if necessary */
    term = log(term);
    if (scaler && scaler[i])
      term += scaler[i] * log(PLL_SCALE_THRESHOLD);

    term *= pattern_weights[i];

    /* store per-site log-likelihood */
    if (persite_lnl)
      persite_lnl[i] = term;

    logl += term;
    #endif
  }
}
