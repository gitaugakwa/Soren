#pragma once
#include "Core.h"
#include "ThreadPool.h"
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

	template<typename TGradient>
	struct GradientData {
		TGradient Gradient = 0;
		bool Initialized = false;

		operator bool() noexcept {
			return Initialized;
		}
	};

	template<typename TVariance>
	struct VarianceData {
		TVariance Variance = 0;
		bool Initialized = false;

		operator bool() noexcept {
			return Initialized;
		}
	};

	struct ThreadData {
		ThreadData(size_t threadcount = 10)
			: Count(threadcount), Threads(threadcount), Initialized(true) { }

		const bool Initialized;
		const size_t Count;
		ThreadPool Threads;

		operator bool() noexcept {
			return Initialized;
		}

		template<class F, class... Args>
		auto enqueue(F&& f, Args&&... args)
			->std::future<typename std::invoke_result<F,Args...>::type>
		{
			return Threads.enqueue(f, std::forward<Args>(args)...);
		}

	};

	struct ActivationData {
		bool Initialized = false;
		Soren::Activation::ActivationFunc Activation{};

		operator bool() noexcept {
			return Initialized;
		}

		template <typename T>
		T Resolve(T node) const
		{
			if (!Initialized)
			{
				SOREN_CORE_ERROR("Initialize Activation");
				assert(false);
				return -1;
			}

			switch (Activation)
			{
			case Soren::Activation::ActivationFunc::Identity:
			{
				return Soren::Activation::Identity(node);
				break;
			}
			case Soren::Activation::ActivationFunc::BinaryStep:
			{
				return Soren::Activation::BinaryStep(node);
				break;
			}
			case Soren::Activation::ActivationFunc::Sigmoid:
			{
				return Soren::Activation::Sigmoid(node);
				break;
			}
			case Soren::Activation::ActivationFunc::TanH:
			{
				return Soren::Activation::TanH(node);
				break;
			}
			case Soren::Activation::ActivationFunc::SQNL:
			{
				return Soren::Activation::SQNL(node);
				break;
			}
			case Soren::Activation::ActivationFunc::ArcTan:
			{
				return Soren::Activation::ArcTan(node);
				break;
			}
			case Soren::Activation::ActivationFunc::ArSinH:
			{
				return Soren::Activation::ArSinH(node);
				break;
			}
			case Soren::Activation::ActivationFunc::ElliotSig:
			{
				return Soren::Activation::ElliotSig(node);
				break;
			}
			case Soren::Activation::ActivationFunc::ReLU:
			{
				return Soren::Activation::ReLU(node);
				break;
			}
			//case Soren::Activation::ActivationFunc::BReLU:
			//{
			//	return Soren::Activation::BReLU(node);
			//	break;
			//}
			case Soren::Activation::ActivationFunc::LeakyReLU:
			{
				return Soren::Activation::LeakyReLU(node);
				break;
			}
			//case Soren::Activation::ActivationFunc::RReLU:
			//{
			//	return Soren::Activation::RReLU(node);
			//	break;
			//}
			case Soren::Activation::ActivationFunc::GELU:
			{
				return Soren::Activation::GELU(node);
				break;
			}
			case Soren::Activation::ActivationFunc::ELU:
			{
				return Soren::Activation::ELU(node);
				break;
			}
			//case Soren::Activation::ActivationFunc::SELU:
			//{
			//	return Soren::Activation::SELU(node);
			//	break;
			//}
			//case Soren::Activation::ActivationFunc::SReLU:
			//{
			//	return Soren::Activation::SReLU(node);
			//	break;
			//}
			//case Soren::Activation::ActivationFunc::APL:
			//{
			//	return Soren::Activation::APL(node);
			//	break;
			//}
			case Soren::Activation::ActivationFunc::SoftPlus:
			{
				return Soren::Activation::SoftPlus(node);
				break;
			}

			//case Soren::Activation::ActivationFunc::SiLU:
			//{
			//	return Soren::Activation::SiLU(node);
			//	break;
			//}
			//case Soren::Activation::ActivationFunc::SoftExponential:
			//{
			//	return Soren::Activation::SoftExponential(node);
			//	break;
			//}

			case Soren::Activation::ActivationFunc::Sinusoid:
			{
				return Soren::Activation::Sinusoid(node);
				break;
			}
			case Soren::Activation::ActivationFunc::Sinc:
			{
				return Soren::Activation::Sinc(node);
				break;
			}
			case Soren::Activation::ActivationFunc::Gaussian:
			{
				return Soren::Activation::Gaussian(node);
				break;
			}
			//case Soren::Activation::ActivationFunc::SQRBF:
			//{
			//	return Soren::Activation::SQRBF(node);
			//	break;
			//}
			case Soren::Activation::ActivationFunc::ISRU:
			{
				return Soren::Activation::ISRU(node);
				break;
			}
			case Soren::Activation::ActivationFunc::ISRLU:
			{
				return Soren::Activation::ISRLU(node);
				break;
			}
			case Soren::Activation::ActivationFunc::PLU:
			{

				return Soren::Activation::PLU(node);
				break;
			}
			case Soren::Activation::ActivationFunc::PReLU:
			{
				return Soren::Activation::PReLU(node);
				break;
			}
			case Soren::Activation::ActivationFunc::SoftClipping:
			{
				return Soren::Activation::SoftClipping(node);
				break;
			}
			case Soren::Activation::ActivationFunc::BentIdentity:
			{
				return Soren::Activation::BentIdentity(node);
				break;
			}
			default:
			{
				return -1;
			}
			}
		}

		template <typename TNode, typename TGradient>
		TNode Resolve(TNode node, TGradient grad) const
		{
			if (!Initialized)
			{
				SOREN_CORE_ERROR("Initialize Activation");
				assert(false);
				return -1;
			}

			if (!grad)
				SOREN_CORE_WARN("Activation function Gradient is 0");

			switch(Activation)
			{
			case Soren::Activation::ActivationFunc::ISRU:
			{
				return Soren::Activation::ISRU(node, grad);
				break;
			}
			case Soren::Activation::ActivationFunc::ISRLU:
			{
				return Soren::Activation::ISRLU(node, grad);
				break;
			}
			case Soren::Activation::ActivationFunc::PLU:
			{

				return Soren::Activation::PLU(node, grad);
				break;
			}
			case Soren::Activation::ActivationFunc::PReLU:
			{
				return Soren::Activation::PReLU(node, grad);
				break;
			}
			case Soren::Activation::ActivationFunc::SoftClipping:
			{
				return Soren::Activation::SoftClipping(node, grad);
				break;
			}
			case Soren::Activation::ActivationFunc::BentIdentity:
			{
				return Soren::Activation::BentIdentity(node, grad);
				break;
			}
			}
		}

		template <typename TNode,typename TGradient, typename TVariance>
		TNode Resolve(TNode node, TGradient grad, TVariance var) const
		{
			if (!Initialized)
			{
				SOREN_CORE_ERROR("Initialize Activation");
				assert(false);
				return -1;
			}

			if (!grad)
				SOREN_CORE_WARN("Activation function Gradient is 0");

			if (!var)
				SOREN_CORE_WARN("Activation function Variance is 0");

			switch (Activation)
			{
			case Soren::Activation::ActivationFunc::PLU:
			{
				return Soren::Activation::PLU(node, grad, var);
				break;
			}
			}
		}
	};

}




