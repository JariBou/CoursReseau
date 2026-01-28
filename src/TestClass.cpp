#include "TestClass.h"

#include "DeserializeMethods.h"
#include "SerializeMethods.h"

TestClass::TestClass() : m_health(0)
{
}

TestClass::TestClass(const std::uint16_t health) : m_health(health)
{
}

TestClass::TestClass(TestClass&& other)
{
    m_health = other.m_health;
}

void TestClass::Serialize(std::vector<std::uint8_t>& byteArray)
{
    SerializeUInt(byteArray, m_health);
}

TestClass TestClass::Deserialize(DeserializeData& deserializeData)
{
    TestClass testClass;
    DeserializeUIntTo(deserializeData, testClass.m_health);
    return testClass;
}
