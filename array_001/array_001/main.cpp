//array:begin example

#include<iostream>
#include<array>

int main() {
	std::array<int, 5> myarray = { 2,16,77,34,50 };

	std::cout << "my array contains";
	//주석을 달아 본다.
	//auto를 이용하여 자동으로 변수를 선언하고, 첫값은 배열의 제일 첫번째 값을 할당한후
	// 배열의 마지막이 아니라면,계속 이터레이트 시킨다. 
	//계속 값을 증가시켜서 끝까지 한다.
	//배열은 지정된 주소가 할당되고 그 안에 값이 있으니까. 계쏙 이터레이트 시킨다느깐
	for (auto it = myarray.begin();
		it != myarray.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	return 0;

}