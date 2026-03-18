/**
 * @file bloom_filter.cpp
 * @brief Explicit template instantiations for common Bloom filter configurations.
 *
 * The BloomFilter class is header-only (template), but we provide explicit
 * instantiations for the preset types to ensure they compile correctly and
 * to allow link-time diagnostics.
 */

#include "strata/bloom/bloom_filter.h"

#include "strata/bloom/sizing.h"

namespace strata {
namespace bloom {

// Explicit instantiations for the preset filter sizes.
// This forces the compiler to generate all methods and catches any errors.
template class BloomFilter<TinyFilter::NUM_BITS, TinyFilter::NUM_HASHES>;
template class BloomFilter<SmallFilter::NUM_BITS, SmallFilter::NUM_HASHES>;
template class BloomFilter<MediumFilter::NUM_BITS, MediumFilter::NUM_HASHES>;
template class BloomFilter<LargeFilter::NUM_BITS, LargeFilter::NUM_HASHES>;

} // namespace bloom
} // namespace strata
