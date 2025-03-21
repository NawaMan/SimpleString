#include "compare_result.hpp"

namespace simple_string {

// Initialize static constant instances
const CompareResult CompareResult::LESS = CompareResult(-1);
const CompareResult CompareResult::EQUAL = CompareResult(0);
const CompareResult CompareResult::GREATER = CompareResult(1);

} // namespace simple_string
