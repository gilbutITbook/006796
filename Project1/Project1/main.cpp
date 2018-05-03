// DevLifePmonTheNormal //
// cout의 소수점 자리수 조절하기//

// '16.11.08.
// power c++ chapter11의 programming 2번문제를 풀다 궁금한.

#include <iostream>
using namespace std;



int main() {
	const double kdNumber = 4321.123456789;

	cout << kdNumber << endl;
	// 4321.12 출력
	// 처음에는 총 자리수(precision, 정수부분 + 소수부분)가
	// 6으로 설정 되있는 것 같습니다.

	cout.precision(5); // 5자리로 설정
	cout << kdNumber << endl;
	// 4321.1 출력
	// 총 자루시가 5자리로 출력되었습니다.

	cout.precision(3); // 3자리로 설정
	cout << kdNumber << endl;
	// 4.32e+03 출력
	// 총 자리수가 3자리로 되어아 하니,
	// 첫 3자리 432를 추출하여 지수 표현식으로써 출력되었습니다.

	cout << fixed;
	cout.precision(16);
	cout << kdNumber << endl;
	// 4321.123457 출력
	// "cout << fixed"는 소수점을 고정해서 표현하자는 뜻입니다.
	// 그래서 "cout.precision(6)"을 통해 소수부분만 6자리 고정하니,
	// 4321.123457이 출력되었습니다.
	// 4321.123456이 아니라 마지막에 7이 표현된 이유는,
	// 그 아래자리에서 반올림(rounding)을 했기 때문입니다.

	cout << scientific;
	cout << kdNumber << endl;
	// 4.321123e+03 출력
	// "scientific"은 지수식으로 표현하자는 뜻입니다.
	// 그리고 scientific은 fixed와 반대의 표현이라
	// 이전에 fixed가 적용되어 있을 때 scientific을 선언했다면
	// fixed가 자동으로 해제가 됩니다.
	// 그러므로, scientific + precision(6)의 결과로,
	// 소수점이 6자리인 지수표현식 4.321123e+03이 출력되었습니다.
	// scientific도 fixed와 마찬가지로
	// 소수점 자리수를 기준으로 고정합니다.

	cout.unsetf(ios::scientific);
	cout << kdNumber << endl;
	// 4321.12 출력
	// 이전에 cout에 적용 되어있던 것은
	// "cout.unsetf();"로 해제 할 수 있습니다.
	// fixed와 scientific키워드는 원래 ios의 네임스페이스안에 있어서,
	// 인자로 전달해 줄 때 "ios::"를 붙여야 합니다.
	// 따라서, precision(6)인 상태만 남으므로,
	// 4321.12가 출력되었습니다.

	cout.setf(ios::fixed);
	cout << kdNumber << endl;
	// 4321.123457 출력
	// 마찬가지로 설정을 "cout.setf();"멤버 함수로도 할 수 있습니다.

	return 0;
}

