#include <immintrin.h>

__attribute((always_inline)) inline bool
contains(const uint8_t *data, uint8_t value, size_t size)
{
  const __m256i  needle = _mm256_set1_epi8(value);
  const uint8_t *end    = data + size;

  for( ; data + 32 <= end; data += 32 )
  {
    const __m256i haystack1 = _mm256_loadu_si256((__m256i *)data);
    const __m256i haystack2 = _mm256_loadu_si256((__m256i *)(data + 32));
    const __m256i cmp1      = _mm256_cmpeq_epi8(haystack1, needle);
    const __m256i cmp2      = _mm256_cmpeq_epi8(haystack2, needle);
    const int     mask1     = _mm256_movemask_epi8(cmp1);
    const int     mask2     = _mm256_movemask_epi8(cmp2);
    if( mask1 != 0 || mask2 != 0 ) { return true; }
  }

  for( ; data + 16 <= end; data += 16 )
  {
    const __m128i haystack = _mm_loadu_si128((__m128i *)data);
    const __m128i cmp      = _mm_cmpeq_epi8(haystack, _mm256_castsi256_si128(needle));
    const int     mask     = _mm_movemask_epi8(cmp);
    if( mask != 0 ) { return true; }
  }

  for( ; data < end; ++data )
  {
    if( *data == value ) { return true; }
  }

  return false;
}
