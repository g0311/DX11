#pragma once

enum FileMode : uint8
{
	Write,
	Read,
};

class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	void Open(wstring filePath, FileMode mode);

	template<typename T>
	void Write(const T& data)
	{
		DWORD numOfBytes = 0;
		assert(::WriteFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}//스트링 >> 동적 배열 >> sizeof 시 이상하게 가져감 >> 템플릿 특수화 문법 사용

	template<>
	void Write<string>(const string& data)
	{
		return Write(data);
	}

	void Write(void* data, uint32 dataSize);
	void Write(const string& data);

	template<typename T>
	void Read(OUT T& data)
	{
		DWORD numOfBytes = 0;
		assert(::ReadFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}

	template<typename T>
	T Read()
	{
		T data;
		Read(data);
		return data;
	}

	void Read(void** data, uint32 dataSize);
	void Read(OUT string& data);

private:
	HANDLE _handle = INVALID_HANDLE_VALUE;
};

