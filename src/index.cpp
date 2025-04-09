#include "../include/index.hpp"

namespace simple {

// This function exists to ensure the header is actually used
// and to provide a placeholder for future non-constexpr implementations
void ensure_index_header_is_used() {
    // Reference the Index class to prevent the "unused header" warning
    Index idx(0);
    (void)idx; // Suppress unused variable warning
}

} // namespace simple
