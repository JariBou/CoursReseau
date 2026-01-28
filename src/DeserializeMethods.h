#pragma once
#include <chrono>
#include <cstdint>
#include <WinSock2.h>

#include "DeserializeData.h"
#include "Decls.h"

template<UintVal UInt>
UInt DeserializeUInt(DeserializeData& deserializeData)
{
    if constexpr (std::is_same_v<UInt, std::uint8_t>)
    {
        UInt value = *deserializeData.GetByte();
        deserializeData.Move(1);
        return value;
    } else
    {
        UInt value;
        std::memcpy(&value, deserializeData.GetByte(), sizeof(value));
        deserializeData.Move(sizeof(value));

        value = ntohs(value);

        return value;
    }
}

template<UintVal UInt>
void DeserializeUIntTo(DeserializeData& deserializeData, UInt& value)
{
    if constexpr (std::is_same_v<UInt, std::uint8_t>)
    {
        value = *deserializeData.GetByte();
        deserializeData.Move(1);
    } else
    {
        UInt val;
        std::memcpy(&val, deserializeData.GetByte(), sizeof(val));
        deserializeData.Move(sizeof(val));

        value = ntohs(val);
    }
}