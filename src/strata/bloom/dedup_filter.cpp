/**
 * @file dedup_filter.cpp
 * @brief DedupFilter implementation (mostly header-only, this provides linkage).
 *
 * The DedupFilter is largely implemented inline in the header for hot-path
 * performance.  This file exists for completeness and to anchor the
 * translation unit.
 */

#include "strata/bloom/dedup_filter.h"

// All methods are inline in the header.  This TU ensures the header compiles
// in isolation and provides a place for future non-inline additions.
