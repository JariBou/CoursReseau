#pragma once
#include <cstdint>
#include <vector>
#include "Decls.h"

template<UintVal UInt>
void SerializeUInt(std::vector<std::uint8_t>& byteArray, const UInt& value)
{
    if constexpr (std::is_same_v<UInt, std::uint8_t>)
    {
        byteArray.push_back(value);
    } else
    {
        UInt val = htons(value);

        const std::size_t offset = byteArray.size();
        byteArray.resize(offset + sizeof(val));
        std::memcpy(&byteArray[offset], &val, sizeof(val));
    }
}