#include <smmintrin.h>
#include <stdint.h>

__attribute((always_inline)) inline bool
contains(const uint8_t *data, uint8_t value, size_t size)
{
  const __m128i  needle = _mm_set1_epi8(value);
  const uint8_t *end    = data + size;

  for( ; data + 16 <= end; data += 16 )
  {
    const __m128i haystack = _mm_loadu_si128((__m128i *)data);
    const __m128i cmp      = _mm_cmpeq_epi8(haystack, needle);
    const int     mask     = _mm_movemask_epi8(cmp);
    if( mask != 0 ) { return true; }
  }

  for( ; data < end; ++data )
  {
    if( *data == value ) { return true; }
  }

  return false;
}