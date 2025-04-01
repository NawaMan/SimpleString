#include "../include/compare_result.hpp"

namespace simple {

// Initialize static constant instances
const CompareResult CompareResult::LESS    = CompareResult(-1);
const CompareResult CompareResult::EQUAL   = CompareResult(0);
const CompareResult CompareResult::GREATER = CompareResult(1);

CompareResult CompareResult::from_int(int value) {
    return CompareResult(value);
}

} // namespace simple
