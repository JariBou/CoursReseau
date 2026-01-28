#pragma once
#include <cstdint>
#include <vector>

class DeserializeData;

class TestClass
{
public:
    explicit TestClass();
    explicit TestClass(std::uint16_t health);
    TestClass(const TestClass& other) = delete;
    TestClass(TestClass&& other);
    ~TestClass() = default;

    TestClass& operator=(const TestClass& other) = delete;
    TestClass& operator=(TestClass&& other) = delete;
    
    void Serialize(std::vector<std::uint8_t>& byteArray);
    static TestClass Deserialize(DeserializeData& deserializeData);

private:
    std::uint16_t m_health;
};
