//array:begin example

#include<iostream>
#include<array>

int main() {
	std::array<int, 5> myarray = { 2,16,77,34,50 };

	std::cout << "my array contains";
	//�ּ��� �޾� ����.
	//auto�� �̿��Ͽ� �ڵ����� ������ �����ϰ�, ù���� �迭�� ���� ù��° ���� �Ҵ�����
	// �迭�� �������� �ƴ϶��,��� ���ͷ���Ʈ ��Ų��. 
	//��� ���� �������Ѽ� ������ �Ѵ�.
	//�迭�� ������ �ּҰ� �Ҵ�ǰ� �� �ȿ� ���� �����ϱ�. ��� ���ͷ���Ʈ ��Ų�ٴ���
	for (auto it = myarray.begin();
		it != myarray.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	return 0;

}