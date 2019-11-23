//My Little Memory Editing / Reading Class.
//ALL Credits go to: http://www.unknowncheats.me/forum/c-and-c/119466-moving-internally-and-getting-rid-rpm.html
//Special Thanks to all members who posted in that thread.

#include <Windows.h>



int filter(unsigned int code, struct _EXCEPTION_POINTERS* ep)
{
	// Handle only AV
	return code == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}

class Memory
{
public:
	template <typename T>
	T read(uintptr_t Address, const T& def_val = T())
	{
		__try
		{
			return *(T*)(Address);
		}
		__except (filter(GetExceptionCode(), GetExceptionInformation()))
		{
			return def_val;
		}
	}

	template <class T>
	bool write(uintptr_t Address, T Value)
	{
		__try
		{
			*(T*)Address = Value;
		}
		__except (filter(GetExceptionCode(), GetExceptionInformation()))
		{
			return false;
		}
		return true;
	}

	template <class T>
	bool add(uintptr_t Address, T Value)
	{
		__try
		{
			*(T*)Address += Value;
		}
		__except (filter(GetExceptionCode(), GetExceptionInformation()))
		{
			return false;
		}
		return true;
	}

	template <class T>
	bool sub(uintptr_t Address, T Value)
	{
		__try
		{
			*(T*)Address -= Value;
		}
		__except (filter(GetExceptionCode(), GetExceptionInformation()))
		{
			return false;
		}
		return true;
	}

	template <class T>
	bool multiply(uintptr_t Address, T Value)
	{
		__try
		{
			*(T*)Address *= Value;
		}
		__except (filter(GetExceptionCode(), GetExceptionInformation()))
		{
			return false;
		}
		return true;
	}

	template <class T>
	bool divide(uintptr_t Address, T Value)
	{
		__try
		{
			*(T*)Address /= Value;
		}
		__except (filter(GetExceptionCode(), GetExceptionInformation()))
		{
			return false;
		}
		return true;
	}

	DWORD* GetAddress(uintptr_t Address1, uintptr_t Address2)
	{
		__try
		{
			DWORD* address = (DWORD*)((*(DWORD*)(Address1)) + Address2);
			return address;
		}
		__except (filter(GetExceptionCode(), GetExceptionInformation()))
		{
			return 0;
		}
	}

	DWORD BaseAddress()
	{
		return (DWORD)GetModuleHandle(NULL);
	}
};