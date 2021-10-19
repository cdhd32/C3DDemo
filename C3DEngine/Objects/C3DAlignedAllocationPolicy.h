#pragma once

//XMMATRIX ������ 8 -> 16����Ʈ ���ķ� �ٲ����
//__declspec(align(16)) struct XMMATRIX <-���ǰ� �̸��� �Ǿ��־��Ф�
//�� �ϸ� �����Ͱ� SSE ��ɾ� ���� �����͸� �����ϰ� ���� ���� �� ����
//https://copynull.tistory.com/241
//https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4316?redirectedfrom=MSDN&view=vs-2019

//XMMATRIX�� ���� ����Ʈ ������ ���� ���ǵ� ����� �ִ� Ŭ�������� ��ӹ޾� ����ϸ� �˴ϴ�
//8 -> T ����Ʈ ���ķ� new/delete �����ε�

//����� �� Ŭ���� object Ŭ������ �ɼ��� �ֽ��ϴ� ������
template<size_t T>
class C3DAlignedAllocationPolicy
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};

