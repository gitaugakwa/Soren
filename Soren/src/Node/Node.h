#pragma once
#include "Core.h"
#include "Base.h"

#include "Status/Status.h"

#include "Log/Log.h"
#include "Events/Event.h"

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

		class BaseNode {
			template<typename TValue, typename TBias, typename TWeight>
			friend class Layer;

		public:
			BaseNode() {};
			~BaseNode() {};


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
				{};
			/*BaseNode(BaseNode&& arg) noexcept
				{};*/
			BaseNode& operator=(const BaseNode&) {
				//mId = sId++;
				return *this;
			};
			/*BaseNode& operator=(BaseNode&& arg) noexcept {
				return *this;
			};*/
			#pragma endregion


		protected:

			inline static size_t sId = 1;
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
			{
				// SOREN_CORE_WARN("No Default Link Weight Limits Set");

				//Bind();

				NodeCreatedEvent event{};
				Emit(event);
			};
			//Node(std::shared_ptr<Activation::Activator<TValue>> activation, TBias bias = 0, TValue value = 0)
			//{
			//	// SOREN_CORE_WARN("No Default Link Weight Limits Set");

			//	//Bind();

			//	NodeCreatedEvent event{};
			//	Emit(event);
			//};
			~Node() {
				NodeDeletedEvent event{};
				Emit(event);
			};

			//inline size_t id() const override { return mId; }
			

			void clear() {
				NodeClearedEvent event{};
				Emit(event);

				// Reset Activation function if stored by Link
			};

			void halt() {

				//const running = m_Running;

				NodeDisabledEvent event{};
				Emit(event);

			};
			void resume() {

				NodeEnabledEvent event{};
				Emit(event);
			};

			void reset() {
				NodeResetEvent event();
				Emit(event);
			};

			//std::vector<double> backPropagate(std::vector<double> derivatives, size_t node) {
			//	// DOutput/DInput
			//	std::vector<double> DOutputDInput;

			//	for (auto& [id, node] : mNodes) {
			//		node->backPropagate(derivatives);
			//	}

			//	for (size_t iOut = 0; iOut < mInput.size(); iOut++) {
			//		DOutputDInput[iOut] = mActivation->prime(mInput[iOut]);
			//	}

			//	return DOutputDInput;

			//}

			// Special member functions
			#pragma region
			Node(const Node& arg)
			{
				NodeCopiedEvent event{};
				Emit(event);
			};
			Node(Node<TValue, TBias, TWeight>&& arg) noexcept
			{
				NodeMovedEvent event{};
				Emit(event);
			};
			Node& operator=(const Node& arg) {
				BaseNode::operator=(arg);
					//if (*this == arg) return *this;

				//mId = sId++;

				NodeCopyAssignedEvent event{};
				Emit(event);

				return *this;
			};
			Node& operator=(Node&& arg) noexcept {
				BaseNode::operator=(arg);

				//mId = std::move(arg.mId);

				NodeMoveAssignedEvent event{};
				Emit(event);
				return *this;
			};
			#pragma endregion


			// Operators
			#pragma region
			//template<typename T>
			//inline T& operator>>(T& rhs) {
			//	rhs = output();
			//	return rhs;
			//}
			/*template<typename TRightValue,typename TRightBias, typename TRightWeight>
			bool operator==(const Node<TRightValue, TRightBias, TRightWeight>& rhs) const {
				return mId == rhs.mId;
			}*/
			/*friend bool operator==(Node& lhs, Linkable& rhs) {
				return lhs.mId == rhs.id();
			}*/
			#pragma endregion


		private:

			// Events
			#pragma region
			//inline void Bind() { SetEventCallbackFunction(BIND_EVENT_FN(Link::OnEvent)); }

			void OnEvent(const Soren::Event& e) {
				EventDispatcher dispatcher(e);

				//dispatcher.Dispatch<NetworkCreatedEvent>(BIND_EVENT_FN(Node::Init)); <- Might be virtuals
				//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN());
				//SOREN_CORE_TRACE(e);
				return;

			};
			#pragma endregion

			//size_t mId;

		};


	} // namespace Neural
} // namespace Soren