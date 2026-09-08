#include "../include/Serializer.hpp"
#include <iostream>
#include <cassert>

int main(void)
{
	// ── Setup ────────────────────────────────────────────────────────────────
	Data original;
	original.id    = 1;
	original.name  = "Miguel";
	original.value = 3.14;

	std::cout << "=== Serializer Test ===" << std::endl;
	std::cout << "Original pointer   : " << &original << std::endl;
	std::cout << "original.id        : " << original.id << std::endl;
	std::cout << "original.name      : " << original.name << std::endl;
	std::cout << "original.value     : " << original.value << std::endl;
	std::cout << std::endl;

	// ── Serialize ────────────────────────────────────────────────────────────
	uintptr_t raw = Serializer::serialize(&original);
	std::cout << "Serialized (raw)   : " << raw << std::endl;

	// ── Deserialize ──────────────────────────────────────────────────────────
	Data *recovered = Serializer::deserialize(raw);
	std::cout << "Deserialized ptr   : " << recovered << std::endl;
	std::cout << std::endl;

	// ── Verify ───────────────────────────────────────────────────────────────
	std::cout << "Same pointer?      : " << (recovered == &original ? "YES" : "NO") << std::endl;
	std::cout << "recovered->id      : " << recovered->id << std::endl;
	std::cout << "recovered->name    : " << recovered->name << std::endl;
	std::cout << "recovered->value   : " << recovered->value << std::endl;

	assert(recovered == &original);
	assert(recovered->id == original.id);
	assert(recovered->name == original.name);
	assert(recovered->value == original.value);

	std::cout << std::endl;
	std::cout << "All assertions passed." << std::endl;

	return 0;
}
