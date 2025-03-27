#include "../include/compare_result.hpp"
namespace simple {

// Initialize static constant instances
const simple::CompareResult CompareResult::LESS    = CompareResult(-1);
const simple::CompareResult CompareResult::EQUAL   = CompareResult(0);
const simple::CompareResult CompareResult::GREATER = CompareResult(1);

} // namespace simple
