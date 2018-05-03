// tuple_element
#include <iostream>     // std::cout
#include <tuple>        // std::tuple, std::make_tuple, std::tuple_element, std::get

int main()
{
	auto mytuple = std::make_tuple(123, 'b');

	std::tuple_element<0, decltype(mytuple)>::type first = std::get<0>(mytuple);
	std::tuple_element<1, decltype(mytuple)>::type second = std::get<1>(mytuple);

	std::cout << "mytuple contains: " << first << " and " << second << '\n';

	return 0;
}