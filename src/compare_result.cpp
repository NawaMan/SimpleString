#include "../include/compare_result.hpp"
namespace mosaic {

// Initialize static constant instances
const mosaic::CompareResult CompareResult::LESS    = CompareResult(-1);
const mosaic::CompareResult CompareResult::EQUAL   = CompareResult(0);
const mosaic::CompareResult CompareResult::GREATER = CompareResult(1);

} // namespace mosaic
