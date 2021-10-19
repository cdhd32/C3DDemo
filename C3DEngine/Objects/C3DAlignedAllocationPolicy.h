#pragma once

//XMMATRIX 때문에 8 -> 16바이트 정렬로 바꿔야함
//__declspec(align(16)) struct XMMATRIX <-정의가 이르케 되어있어요ㅠㅠ
//안 하면 데이터가 SSE 명령어 사용시 데이터를 안전하게 보장 받을 수 없음
//https://copynull.tistory.com/241
//https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4316?redirectedfrom=MSDN&view=vs-2019

//XMMATRIX와 같은 바이트 정렬이 따로 정의된 멤버가 있는 클래스에서 상속받아 사용하면 됩니다
//8 -> T 바이트 정렬로 new/delete 오버로딩

//사실은 이 클래스 object 클래스가 될수도 있습니다 ㄴㅇㄱ
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

