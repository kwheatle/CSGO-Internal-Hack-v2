#pragma once
#include <Windows.h>
#include <stdint.h>
#include <stdexcept>
#include <assert.h>

class vmt_hook
{
public:
	uintptr_t * search_free_data_page(const char * module_name, const size_t vmt_size);
	vmt_hook();
	vmt_hook(void * base);
	~vmt_hook();
	bool wasAllocated;
	bool vmt_hook::setup(void* base, const char* moduleName = nullptr);

	size_t estimate_vftbl_length(uintptr_t * vftbl_start);

	void hook_index(size_t fn_index, void * fn_pointer);

	void release();

	template<typename T>
	T get_original(int index)
	{
		return (T)old_vftbl[index];
	}

private:
	void* class_base;
	uintptr_t* old_vftbl;
	uintptr_t* new_vftb1;
	size_t vftbl_len;


};

class protect_guard
{
public:
	protect_guard(void* base, size_t len, uint32_t flags)
	{
		_base = base;
		_length = len;
		if (!VirtualProtect(base, len, flags, (PDWORD)&_old))
			throw std::runtime_error("Failed to protect region.");
	}
	~protect_guard()
	{
		VirtualProtect(_base, _length, _old, (PDWORD)&_old);
	}

private:
	void*         _base;
	size_t        _length;
	uint32_t _old;
};
