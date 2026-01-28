#include "DeserializeData.h"

#include <assert.h>

DeserializeData::DeserializeData(std::vector<std::uint8_t> array) : m_byteArray(std::move(array))
{
}

void DeserializeData::Move(const std::size_t amount)
{
    m_reader += amount;
}

std::uint8_t* DeserializeData::GetByte()
{
    return &m_byteArray[m_reader];
}

std::uint8_t* DeserializeData::GetByteAt(const std::size_t pos)
{
    assert(pos < m_byteArray.size() && pos >= 0);
    return &m_byteArray[pos];
}
