#pragma once
#include "Core.h"
#include "Base.h"

#include "Status/Status.h"

#include "Log/Log.h"
#include "Events/Event.h"

#include "CallbackFunctions.h"

#include "nlohmann/json.hpp"

namespace Soren {

	template <typename T>
	class FileOutput;
	template <typename T>
	class FileInput;

	namespace Neural {

		class Event;

		struct Linkable;
		template<typename TWeight>
		class Linker;

		class BaseNode:public Linkable {
			template<typename TValue, typename TBias, typename TWeight>
			friend class Layer;

		public:
			BaseNode():mId{sId++} {};
			~BaseNode() {};

			inline size_t id() const override { return mId; }

			BaseNode& pipe(BaseNode& layer);
			template<typename TValue, typename TBias>
			typename std::common_type<TValue, TBias>::type output() const;
			template<typename TInputValue>
			BaseNode& input(TInputValue& value);
			template<typename TInputValue>
			BaseNode& input(TInputValue&& value);

			// Special member functions
			#pragma region
			BaseNode(const BaseNode&)
				: mId(sId++) {};
			BaseNode(BaseNode&& arg) noexcept
				: mId(std::exchange(arg.mId, 0)) {};
			BaseNode& operator=(const BaseNode&) {
				mId = sId++;
				return *this;
			};
			BaseNode& operator=(BaseNode&& arg) noexcept {
				mId = std::move(arg.mId);
				return *this;
			};
			#pragma endregion


		protected:

			inline static size_t sId = 1;
			size_t mId;
		};


		template<typename TValue = double, typename TBias = TValue, typename TWeight = TValue>
		class SOREN_API Node : public EventEmmiter, public BaseNode // Should be NodeEvent
		{

			template<typename TValue, typename TBias, typename TWeight>
			friend class Node;
			template<typename TValue, typename TBias, typename TWeight>
			friend class Layer;
		public:

			Node(TBias bias = 0, TValue value = 0)
				: mBias{ bias }, mValue{ value }
			{
				// SOREN_CORE_WARN("No Default Link Weight Limits Set");

				//Bind();

				NodeCreatedEvent event{};
				Emit(event);
			};
			Node(std::function<TValue(const typename std::common_type<TValue, TBias>::type)> activation, TBias bias = 0, TValue value = 0)
				: mActivation{ activation }, mBias{ bias }, mValue{ value }
			{
				// SOREN_CORE_WARN("No Default Link Weight Limits Set");

				//Bind();

				NodeCreatedEvent event{};
				Emit(event);
			};
			~Node() {
				NodeDeletedEvent event{};
				Emit(event);
			};

			//inline size_t id() const override { return mId; }
			inline TValue value() const { return mValue; }
			inline TBias bias() const { return mBias; }
			inline bool running() const { return mRunning; }
			inline bool locked() const { return mLocked; }

			void clear() {
				if (mLocked) return;

				const weight = mBias;
				mBias = NULL;

				NodeClearedEvent event{};
				Emit(event);

				// Reset Activation function if stored by Link
			};

			void halt() {

				//const running = m_Running;
				mRunning = FALSE;

				NodeDisabledEvent event{};
				Emit(event);

			};
			void resume() {
				mRunning = TRUE;

				NodeEnabledEvent event{};
				Emit(event);
			};

			void reset() {
				NodeResetEvent event(mBias);
				Emit(event);
			};

			inline void value(TValue value) { if (mLocked) return; mValue = value; }
			inline void bias(TBias bias) { if (mLocked) return; mBias = bias; }
			inline void lock(bool lock) { mLocked = lock; }
			inline void lock() { mLocked = true; }
			inline void unlock() { mLocked = false; }

			template<typename TWeight>
			inline Link<TWeight> link(TWeight weight) {
				return Link<TWeight>(weight);
				//return (Link<TWeight>&)mLinker.link<Link<TWeight>>(*this, node, weight);
			}

			template<typename TInputValue = TValue>
			inline Node& input(TInputValue value) {
				mValue = static_cast<TValue>(value);
				return *this;
			}

			template<typename TValue = TValue, typename TBias = TBias>
			inline typename std::common_type<TValue, TBias>::type output() const {
				return mActivation((mValue + mBias));
			}
			
			inline typename std::common_type<TValue, TBias>::type resolve() const {
				return mActivation((mValue + mBias));
			}

			template <typename TPipeNode>
			inline Node& pipe(std::unique_ptr<Node<TPipeNode>> node) {
				return node.input(resole());
			}

			// Special member functions
			#pragma region
			Node(const Node& arg)
				: BaseNode(arg),
				mValue(arg.mValue),
				mBias(arg.mBias),
				//mId(sId++),
				mRunning(arg.mRunning),
				mLocked(arg.mLocked),
				mActivation(arg.mActivation)
			{
				NodeCopiedEvent event{arg.mId, mId};
				Emit(event);
			};
			Node(Node<TValue, TBias, TWeight>&& arg) noexcept
				: BaseNode(std::move(arg)),
				mValue(std::exchange(arg.mValue, static_cast<TValue>(0))),
				mBias(std::exchange(arg.mBias, static_cast<TBias>(0))),
				//mId(std::exchange(arg.mId, 0)),
				mRunning(std::exchange(arg.mRunning, false)),
				mLocked(std::exchange(arg.mLocked, true)),
				mActivation(std::exchange(arg.mActivation, Activation::Identity<TValue>))
			{
				NodeMovedEvent event{};
				Emit(event);
			};
			Node& operator=(const Node& arg) {
				BaseNode::operator=(arg)
					//if (*this == arg) return *this;

				mValue = arg.mValue;
				mBias = arg.mBias;
				mRunning = arg.mRunning;
				mLocked = arg.mLocked;
				mActivation = arg.mActivation;
				//mId = sId++;

				NodeCopyAssignedEvent event{};
				Emit(event);

				return *this;
			};
			Node& operator=(Node&& arg) noexcept {
				BaseNode::operator=(arg)

				mValue = std::move(arg.mValue);
				mBias = std::move(arg.mBias);
				mRunning = std::move(arg.mRunning);
				mLocked = std::move(arg.mLocked);
				mActivation = std::move(arg.mActivation);
				//mId = std::move(arg.mId);

				NodeMoveAssignedEvent event{};
				Emit(event);
				return *this;
			};
			#pragma endregion


			// Operators
			#pragma region
			template<typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
			friend Node& operator>>(T lhs, Node& rhs) {
				return rhs.input(lhs);
			}
			inline Node& operator>>(Node& rhs) {
				return pipe(rhs);
			}
			template<typename TWeight>
			inline auto operator>>(Link<TWeight>& rhs) {
				return rhs.input(resolve());
				//return rhs;
			}
			//template<typename T>
			//inline T& operator>>(T& rhs) {
			//	rhs = output();
			//	return rhs;
			//}
			template<typename TRightValue,typename TRightBias, typename TRightWeight>
			bool operator==(const Node<TRightValue, TRightBias, TRightWeight>& rhs) const {
				return mId == rhs.mId;
			}
			/*friend bool operator==(Node& lhs, Linkable& rhs) {
				return lhs.mId == rhs.id();
			}*/
			#pragma endregion


		private:

			// Events
			#pragma region
			//inline void Bind() { SetEventCallbackFunction(BIND_EVENT_FN(Link::OnEvent)); }

			void OnEvent(const Soren::Event& e) {
				if (!mRunning) return;
				EventDispatcher dispatcher(e);

				//dispatcher.Dispatch<NetworkCreatedEvent>(BIND_EVENT_FN(Node::Init)); <- Might be virtuals
				//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN());
				//SOREN_CORE_TRACE(e);
				return;

			};
			#pragma endregion

			//size_t mId;

			bool mRunning = true; // So as to allow links to be disabled and enabled
			bool mLocked = false;

			std::function<TValue(const TValue)> mActivation = Activation::Identity<TValue>;
			TValue mValue= 0;
			TBias mBias = 0;
		};


	} // namespace Neural
} // namespace Soren