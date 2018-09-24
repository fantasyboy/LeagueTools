#ifndef StringCovert_2016_4_8_52_H_
#define StringCovert_2016_4_8_52_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

#define AnsiToUnicode(str)	StringToWString<>()(str)
#define UnicodeToAnsi(str)	WStringToString<>()(str)
#define Utf8ToUnicode(str)	StringToWString<CP_UTF8>()(str)
#define UnicodeToUtf8(str)	WStringToString<CP_UTF8>()(str)

#define Utf8ToAnsi(str) UnicodeToAnsi(Utf8ToUnicode(str))
#define AnsiToUtf8(str) UnicodeToUtf8(AnsiToUnicode(str))

#define UCS2ToAnsi(str) UCSToString<>()(str)
#define UCS2ToUtf8(str) UCSToString<3>()(str)

#define nil nullptr
#define SD_ARRAY(ptr) if(nil != ptr) { delete[] ptr; ptr = nil; }

template<int code = CP_ACP, int nbytes = 512>
class StringToWString
{
public:
	StringToWString() : _buffer(nil) {}
	~StringToWString() { SD_ARRAY(_buffer); }

public:
	std::wstring operator()(const char* p) {
		int mount = ::MultiByteToWideChar(code, 0, p, -1, nil, 0);
		if(0 == mount) {
			return std::wstring();
		} else if(nbytes >= mount) {
			::MultiByteToWideChar(code, 0, p, -1, _array, mount);
			return _array;
		}

		_buffer = new wchar_t[mount];
		::MultiByteToWideChar(code, 0, p, -1, _buffer, mount);
		return _buffer;
	}

	std::wstring operator()(const char* p, unsigned size) {
		int mount = ::MultiByteToWideChar(code, 0, p, size, nil, 0);
		if(0 == mount) {
			return std::wstring();
		} else if(nbytes >= mount) {
			::MultiByteToWideChar(code, 0, p, size, _array, mount);
			return std::wstring(_array, mount);
		}

		_buffer = new wchar_t[mount];
		::MultiByteToWideChar(code, 0, p, size, _buffer, mount);
		return std::wstring(_buffer, mount);
	}

	std::wstring operator()(const std::string& str) {
		return operator()(str.c_str(), str.size());
	}

private:
	wchar_t* _buffer;
	wchar_t _array[nbytes];
};

template<int code = CP_ACP, int nbytes = 512>
class WStringToString
{
public:
	WStringToString() : _buffer(nil) {}
	~WStringToString() { SD_ARRAY(_buffer); }

public:
	std::string operator()(const wchar_t* p) {
		int need = ::WideCharToMultiByte(code, 0, p, -1, nil, 0, nil, nil);
		if(0 == need) {
			return std::string();
		} else if(nbytes >= need) {
			::WideCharToMultiByte(code, 0, p, -1, _array, need, nil, nil);
			return _array;
		}

		_buffer = new char[need];
		::WideCharToMultiByte(code, 0, p, -1, _buffer, need, nil, nil);
		return _buffer;
	}

	std::string operator()(const wchar_t* p, unsigned size) {
		int need = ::WideCharToMultiByte(code, 0, p, size, nil, 0, nil, nil);
		if(0 == need) {
			return std::string();
		} else if(nbytes >= need) {
			::WideCharToMultiByte(code, 0, p, size, _array, need, nil, nil);
			return std::string(_array, need);
		}

		_buffer = new char[need];
		::WideCharToMultiByte(code, 0, p, size, _buffer, need, nil, nil);
		return std::string(_buffer, need);
	}

	std::string operator()(const std::wstring& wstr) {
		return operator()(wstr.c_str(), wstr.size());
	}

private:
	char* _buffer;
	char _array[nbytes];
};

/**********************************************************
*@brief  : 
* 模板参数2表示将ucs2转换为ansi编码（2将"\\u5f6d\\u65af\\u9676"字符转换为可显示字符）
*@return : 返回ansi编码格式的字符
***********************************************************/
template<int code = 2>
class UCSToString 
{
public:
	std::string operator()(const char* p, unsigned size) {
		std::wstring rst;
		auto escape = false;

		for(auto i = 0; i < size; i++) {
			char c = p[i];
			switch (c) {
			case '//':
			case '\\':
			case '%': escape = true; break;

			case 'u':
			case 'U':
				if(escape) {
					char temp[5] = {0};
					memcpy(temp, p + i + 1, 4);
					unsigned hex = 0;
					sscanf(temp, "%x", &hex);
					rst.push_back(hex);
					i += 4;
					escape=false;

				} else {
					rst.push_back(c);
				} break;

			default: rst.push_back(c); break;
			}
		}

		if(3 == code) {
			return UnicodeToUtf8(rst);
		}

		return UnicodeToAnsi(rst);
	}

	std::string operator()(const std::string& str) {
		return operator()(str.c_str(), str.size());
	}
};


#endif //StringCovert_2016_4_8_52_H_