#pragma once
#include <cstdint>

template<typename T>
concept UintVal = std::_Is_any_of_v<T, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>;
