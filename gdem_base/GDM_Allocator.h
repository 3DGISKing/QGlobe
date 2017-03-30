// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine" and the "irrXML" project.
// For conditions of distribution and use, see copyright notice in geoEngine.h and irrXML.h

#ifndef _GDM_ALLOCATOR_H_
#define _GDM_ALLOCATOR_H_

#include <new>
#include <memory.h>

//! Very simple allocator implementation, containers using it can be used across dll boundaries
template<typename T>
class gdmAllocator
{
public:

	//! Destructor
	virtual ~gdmAllocator() {}

	//! Allocate memory for an array of objects
	T* allocate(size_t cnt)
	{
		return (T*)internal_new(cnt* sizeof(T));
	}

	//! Deallocate memory for an array of objects
	void deallocate(T* ptr)
	{
		internal_delete(ptr);
	}

	//! Construct an element
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}

	//! Destruct an element
	void destruct(T* ptr)
	{
		ptr->~T();
	}

protected:

	virtual void* internal_new(size_t cnt)
	{
		return operator new(cnt);
	}

	virtual void internal_delete(void* ptr)
	{
		operator delete(ptr);
	}

};

#endif // _GDM_ALLOCATOR_H_

