#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>

struct Player
{
	int hp; //< plage: [0;100]
	int shield; //< plage: [0;100000]
	float x, y, z;
	std::string name;
};

void SerializeU8(std::vector<std::uint8_t>& byteArray, std::uint8_t value);
void SerializeU16(std::vector<std::uint8_t>& byteArray, std::uint16_t value);
void SerializeU32(std::vector<std::uint8_t>& byteArray, std::uint32_t value);
void SerializeF32(std::vector<std::uint8_t>& byteArray, float value);
void SerializeString(std::vector<std::uint8_t>& byteArray, const std::string& value);

std::uint8_t DeserializeU8(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::uint16_t DeserializeU16(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::uint32_t DeserializeU32(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
float DeserializeF32(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::string DeserializeString(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);

int not_main(int argc, char **argv)
{
	Player player = {
		.hp = 42,
		.shield = 1542,
		.x = -412.f, .y = -47.f, .z = 63.f,
		.name = "Volodia"
	};

	std::vector<std::uint8_t> byteArray;
	SerializeU8(byteArray, (std::uint8_t) player.hp);
	SerializeU32(byteArray, (std::uint32_t) player.shield);
	SerializeF32(byteArray, player.x);
	SerializeF32(byteArray, player.y);
	SerializeF32(byteArray, player.z);
	SerializeString(byteArray, player.name);

	// Déserialisation
	Player deserializedVolodia;

	std::size_t offset = 0;
	deserializedVolodia.hp = DeserializeU8(byteArray, offset);
	deserializedVolodia.shield = DeserializeU32(byteArray, offset);
	deserializedVolodia.x = DeserializeF32(byteArray, offset);
	deserializedVolodia.y = DeserializeF32(byteArray, offset);
	deserializedVolodia.z = DeserializeF32(byteArray, offset);
	deserializedVolodia.name = DeserializeString(byteArray, offset);

	return 0;
}

void SerializeU8(std::vector<std::uint8_t>& byteArray, std::uint8_t value)
{
	byteArray.push_back(value);
}

void SerializeU16(std::vector<std::uint8_t>& byteArray, std::uint16_t value)
{
	value = htons(value); //< conversion en big endian

	std::size_t offset = byteArray.size();
	byteArray.resize(offset + sizeof(value));
	std::memcpy(&byteArray[offset], &value, sizeof(value));
}

void SerializeU32(std::vector<std::uint8_t>& byteArray, std::uint32_t value)
{
	value = htonl(value); //< conversion en big endian

	std::size_t offset = byteArray.size();
	byteArray.resize(offset + sizeof(value));
	std::memcpy(&byteArray[offset], &value, sizeof(value));
}

void SerializeF32(std::vector<std::uint8_t>& byteArray, float value)
{
	std::size_t offset = byteArray.size();
	byteArray.resize(offset + sizeof(value));
	std::memcpy(&byteArray[offset], &value, sizeof(value));
}

void SerializeString(std::vector<std::uint8_t>& byteArray, const std::string& value)
{
	// Sérialisation de la taille
	assert(value.size() < 0xFFFF);
	SerializeU16(byteArray, value.size());

	std::size_t offset = byteArray.size();
	byteArray.resize(offset + value.size());
	std::memcpy(&byteArray[offset], &value[0], value.size());
}

std::uint8_t DeserializeU8(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
{
	std::uint8_t value = byteArray[offset];
	offset++;
	return value;
}

std::uint16_t DeserializeU16(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
{
	std::uint16_t value;
	std::memcpy(&value, &byteArray[offset], sizeof(value));
	offset += sizeof(value);

	value = ntohs(value);

	return value;
}

std::uint32_t DeserializeU32(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
{
	std::uint32_t value;
	std::memcpy(&value, &byteArray[offset], sizeof(value));
	offset += sizeof(value);

	return value;
}

float DeserializeF32(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
{
	float value;
	std::memcpy(&value, &byteArray[offset], sizeof(value));
	offset += sizeof(value);

	return value;
}

std::string DeserializeString(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
{
	std::uint16_t stringLength = DeserializeU16(byteArray, offset);

	std::string value;
	value.resize(stringLength, ' ');
	std::memcpy(&value[0], &byteArray[offset], stringLength);

	offset += stringLength;

	return value;
}