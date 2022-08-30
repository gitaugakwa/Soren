#pragma once
#include "Core.h"
#include "Activation/Activation.h"
#include "Random/Random.h"
#include "Log/Log.h"

#include <functional>
#include <type_traits>

namespace Soren {

	class SOREN_API Event;

	enum class ExportType {
		None = 0,
		Normal, Shrink, JSON
	};

	enum class ImportType {
		None = 0,
		Normal, Shrink, JSON
	};

	// Swaps the provided pointers
#define CHANGE_POINTERS(x, y)	\
    {							\
        void* t;				\
        t = x;					\
        x = y;					\
    }
        //y = t;					\

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	using EventCallbackFn = std::function<void(Event&)>;

	// nother template for the return type of access functions, by default it would be equal to the vector type
	template <typename VecType, typename ReturnType = VecType>
	struct SOREN_API Accessor {

		// Iterators

		typedef typename std::vector<VecType>::iterator iterator;

		iterator begin()
		{
			return (*Data).begin();
		}
		iterator end()
		{
			return (*Data).end();
		}
		const iterator begin() const
		{
			return (*Data).begin();
		}
		const iterator end() const
		{
			return (*Data).end();
		}
		// Access

		// there should be an implicit conversion form VecType to ReturnType
		// see if viable
		// enable if return type is same as Vectype, otherwise, the user will have to implement their own
		//template <std::enable_if_t<std::is_same<VecType, ReturnType>::value, int> = 0>
		ReturnType& at(size_t pos)
		{
			return (*Data).at(pos);
		}

		//template <std::enable_if_t<std::is_same<VecType, ReturnType>::value, int> = 0>
		ReturnType& operator[](size_t pos)
		{
			return (*Data)[pos];
		}

		//template <std::enable_if_t<std::is_same<VecType, ReturnType>::value, int> = 0>
		ReturnType& front()
		{
			return (*Data).front();
		}

		//template <std::enable_if_t<std::is_same<VecType, ReturnType>::value, int> = 0>
		ReturnType& back()
		{
			return (*Data).back();
		}

		void operator=(std::vector<VecType>& data)
		{
			Data = &data;
		}

		void operator=(Soren::Accessor<VecType, ReturnType>& data)
		{
			Data = data.Data;
		}

		// Capacity

		bool empty() const
		{
			return (*Data).empty();
		}

		size_t size() const
		{
			return (*Data).size();
		}

		size_t max_size() const
		{
			return (*Data).max_size();
		}

		size_t capacity() const
		{
			return (*Data).capacity();
		}
		
	private:
		std::vector<VecType>* Data = nullptr;
	};

}




