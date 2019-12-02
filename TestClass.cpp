#include "TestClass.h"

TestClass::TestClass() {
	std::cout << "Test constructor" << std::endl;
}

TestClass::~TestClass() {
	std::cout << "Test destructor" << std::endl;
}

void TestClass::dummy(int num) {
	std::cout << "Dummy method got " << num << std::endl;
}