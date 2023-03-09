#include <immintrin.h>

__attribute((always_inline)) inline bool
contains(uint8_t *arr, size_t size, uint8_t value)
{
  // SIMD yüksek hızlandırma için, 256-bitlik SIMD türü olan __m256i kullanıyoruz.
  // __m256i, 32 adet uint8_t değerini içerebilen bir türdür.
  constexpr size_t SimdWidth = 32;
  size_t           simdCount = size / SimdWidth;
  __m256i          simdValue = _mm256_set1_epi8(value);
  for( size_t i = 0; i < simdCount; i++ )
  {
    __m256i simdArray = _mm256_load_si256(reinterpret_cast<__m256i *>(arr + i * SimdWidth));
    __m256i result    = _mm256_cmpeq_epi8(simdArray, simdValue);
    int     mask      = _mm256_movemask_epi8(result);
    if( mask != 0 ) { return true; }
  }
  for( size_t i = simdCount * SimdWidth; i < size; i++ )
  {
    if( arr[i] == value ) { return true; }
  }
  return false;
}
