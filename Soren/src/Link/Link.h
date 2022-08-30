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

	class BaseLink {
	protected:
		inline static size_t sId = 1;
	};

	template<typename TWeight = double>
	class SOREN_API Link : public EventEmmiter, public BaseLink
	{
		//template<typename TValue, typename TBias>
		//friend class Node;
		template<typename TWeight>
		friend class Linker;
	public:

		Link(TWeight weight = Random::Number<TWeight>())
		{
			// SOREN_CORE_WARN("No Default Link Weight Limits Set");

			//Bind();

			LinkCreatedEvent event{};
			Emit(event);
		};
		~Link() {
			LinkDeletedEvent event{};
			Emit(event);
		};

		void clear() {

			LinkClearedEvent event{};
			Emit(event);

			// Reset Activation function if stored by Link
		};

		void halt() {

			LinkDisabledEvent event{};
			Emit(event);

		};
		void resume() {
			LinkEnabledEvent event{};
			Emit(event);
		};

		void reset() {
			LinkResetEvent event{};
			Emit(event);
		};

	

		// Callbacks
		//void SetEventCallbackFunction(const EventCallbackFn& callback);

		// Events
		#pragma region
		// OnEvent functions
		// Events added to links will be to all links
		
		#pragma endregion

		// Special member functions
		#pragma region
		Link(const Link& arg)
		{
			LinkCopiedEvent event{};
			Emit(event);
		};
		Link(Link&& arg) noexcept
		{
			LinkMovedEvent event{};
			Emit(event);
		};
		Link& operator=(const Link& arg) {

			LinkCopyAssignedEvent event{};
			Emit(event);

			return *this;
		};
		Link& operator=(Link&& arg) noexcept {


			LinkMoveAssignedEvent event{};
			Emit(event);
			return *this;
		};
		#pragma endregion


		// Operators
		#pragma region
		/*bool operator==(const Link& rhs) const {
			return ();
		};*/
		//bool operator==(const Node& rhs) const;
		/*bool operator!=(const Link& rhs) const {
			return !(*this == rhs);
		};*/
		//bool operator!=(const Node& rhs) const;
		friend std::ostream& operator<<(std::ostream& os, const Link& link) {
			return os << link.str();
		};
		template <typename T>
		friend FileOutput<T>& operator<<(Soren::FileOutput<T>& fos, const Link& link)
		{
			fos << link.str();
			return fos;
		}
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

	};


} // namespace Neural
} // namespace Soren

