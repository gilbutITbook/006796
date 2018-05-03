// DevLifePmonTheNormal //
// cout�� �Ҽ��� �ڸ��� �����ϱ�//

// '16.11.08.
// power c++ chapter11�� programming 2�������� Ǯ�� �ñ���.

#include <iostream>
using namespace std;



int main() {
	const double kdNumber = 4321.123456789;

	cout << kdNumber << endl;
	// 4321.12 ���
	// ó������ �� �ڸ���(precision, �����κ� + �Ҽ��κ�)��
	// 6���� ���� ���ִ� �� �����ϴ�.

	cout.precision(5); // 5�ڸ��� ����
	cout << kdNumber << endl;
	// 4321.1 ���
	// �� �ڷ�ð� 5�ڸ��� ��µǾ����ϴ�.

	cout.precision(3); // 3�ڸ��� ����
	cout << kdNumber << endl;
	// 4.32e+03 ���
	// �� �ڸ����� 3�ڸ��� �Ǿ�� �ϴ�,
	// ù 3�ڸ� 432�� �����Ͽ� ���� ǥ�������ν� ��µǾ����ϴ�.

	cout << fixed;
	cout.precision(16);
	cout << kdNumber << endl;
	// 4321.123457 ���
	// "cout << fixed"�� �Ҽ����� �����ؼ� ǥ�����ڴ� ���Դϴ�.
	// �׷��� "cout.precision(6)"�� ���� �Ҽ��κи� 6�ڸ� �����ϴ�,
	// 4321.123457�� ��µǾ����ϴ�.
	// 4321.123456�� �ƴ϶� �������� 7�� ǥ���� ������,
	// �� �Ʒ��ڸ����� �ݿø�(rounding)�� �߱� �����Դϴ�.

	cout << scientific;
	cout << kdNumber << endl;
	// 4.321123e+03 ���
	// "scientific"�� ���������� ǥ�����ڴ� ���Դϴ�.
	// �׸��� scientific�� fixed�� �ݴ��� ǥ���̶�
	// ������ fixed�� ����Ǿ� ���� �� scientific�� �����ߴٸ�
	// fixed�� �ڵ����� ������ �˴ϴ�.
	// �׷��Ƿ�, scientific + precision(6)�� �����,
	// �Ҽ����� 6�ڸ��� ����ǥ���� 4.321123e+03�� ��µǾ����ϴ�.
	// scientific�� fixed�� ����������
	// �Ҽ��� �ڸ����� �������� �����մϴ�.

	cout.unsetf(ios::scientific);
	cout << kdNumber << endl;
	// 4321.12 ���
	// ������ cout�� ���� �Ǿ��ִ� ����
	// "cout.unsetf();"�� ���� �� �� �ֽ��ϴ�.
	// fixed�� scientificŰ����� ���� ios�� ���ӽ����̽��ȿ� �־,
	// ���ڷ� ������ �� �� "ios::"�� �ٿ��� �մϴ�.
	// ����, precision(6)�� ���¸� �����Ƿ�,
	// 4321.12�� ��µǾ����ϴ�.

	cout.setf(ios::fixed);
	cout << kdNumber << endl;
	// 4321.123457 ���
	// ���������� ������ "cout.setf();"��� �Լ��ε� �� �� �ֽ��ϴ�.

	return 0;
}

