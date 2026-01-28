#pragma once
#include <cstdint>
#include <cstdio>
#include <vector>

class DeserializeData
{
public:
    explicit DeserializeData(std::vector<std::uint8_t> array);
    DeserializeData(const DeserializeData& other) = delete;
    DeserializeData(DeserializeData&& other) = delete;
    ~DeserializeData() = default;

    DeserializeData& operator=(const DeserializeData& other) = delete;
    DeserializeData& operator=(DeserializeData&& other) = delete;
    
    void Move(std::size_t amount);
    std::uint8_t* GetByte();
    std::uint8_t* GetByteAt(std::size_t pos);

private:
    
    std::vector<std::uint8_t> m_byteArray;
    std::size_t m_reader = 0;
    
};
