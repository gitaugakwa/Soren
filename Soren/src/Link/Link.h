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

	class BaseLink {
	protected:
		inline static size_t sId = 1;
	};

	template<typename TWeight = double>
	class SOREN_API Link : LinkerLink, public EventEmmiter, public BaseLink
	{
		//template<typename TValue, typename TBias>
		//friend class Node;
		template<typename TWeight>
		friend class Linker;
	public:

		Link(TWeight weight = Random::Number<TWeight>())
			: mWeight{ weight }, mId{ sId++ }
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

		// Links
		inline TWeight weight() const { return mWeight; }
		inline bool running() const { return mRunning; }
		inline bool locked() const { return mLocked; }

		void clear() {
			if (mLocked) return;

			const weight = mWeight;
			mWeight = NULL;

			LinkClearedEvent event{};
			Emit(event);

			// Reset Activation function if stored by Link
		};

		void halt() {

			//const running = m_Running;
			mRunning = FALSE;

			LinkDisabledEvent event{};
			Emit(event);

		};
		void resume() {
			mRunning = TRUE;

			LinkEnabledEvent event{};
			Emit(event);
		};

		void reset() {
			LinkResetEvent event{};
			Emit(event);
		};

		inline void weight(TWeight weight) { if (mLocked) return; mWeight = weight; }
		inline void lock(bool lock) { mLocked = lock; }
		inline void lock() { mLocked = true; }
		inline void unlock() { mLocked = false; }

		// Callbacks
		//void SetEventCallbackFunction(const EventCallbackFn& callback);

		// Events
		#pragma region
		// OnEvent functions
		// Events added to links will be to all links
		
		#pragma endregion

		std::string str(const std::string& linkpref = "", const std::string& linksuf = "") const {
			std::stringstream ss;

			ss << linkpref;
			ss << "Link[" << m_LinkID << "]";
			ss << " Weight: " << std::setprecision(std::numeric_limits<TWeight>::digits10 + 1) << mWeight;
			if (m_InBuffer)
			{
				ss << " InBuffer: " << m_InBuffer;
			}
			ss << linksuf;
			ss << std::endl;

			return ss.str();
		};

		//void SetLinkEventfunc(const EventType& type, LinkEventfunc func);
		//inline EventEmmiter& emmiter() { return m_Emmitter; }

		// Functional
		template<typename TValue>
		inline typename std::common_type<TWeight, TValue>::type pipe(TValue value) const {
			return mWeight * value;
		}


		// Special member functions
		#pragma region
		Link(const Link& arg)
			: mWeight(arg.mWeight),
			mId(sId++),
			mRunning(arg.mRunning),
			mLocked(arg.mLocked)
		{
			LinkCopiedEvent event{};
			Emit(event);
		};
		Link(Link&& arg) noexcept
			: mWeight(std::exchange(arg.mWeight, 0)),
			mId(std::exchange(arg.mId, 0)),
			mRunning(std::exchange(arg.mRunning, false)),
			mLocked(std::exchange(arg.mLocked, true))
		{
			LinkMovedEvent event{};
			Emit(event);
		};
		Link& operator=(const Link& arg) {
			if (*this == arg) return *this;

			mWeight = arg.mWeight;
			mRunning = arg.mRunning;
			mLocked = arg.mLocked;
			//mId = sId++;

			LinkCopyAssignedEvent event{};
			Emit(event);

			return *this;
		};
		Link& operator=(Link&& arg) noexcept {

			mWeight = std::move(arg.mWeight);
			mRunning = std::move(arg.mRunning);
			mLocked = std::move(arg.mLocked);
			mId = std::move(arg.mId);

			LinkMoveAssignedEvent event{};
			Emit(event);
			return *this;
		};
		#pragma endregion


		// Operators
		#pragma region
		bool operator==(const Link& rhs) const {
			return (m_LinkID == rhs.m_LinkID &&
				mWeight == rhs.mWeight &&
				mRunning == rhs.mRunning &&
				mLocked == rhs.mLocked);
		};
		//bool operator==(const Node& rhs) const;
		bool operator!=(const Link& rhs) const {
			return !(*this == rhs);
		};
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
			if (!mRunning) return;
			EventDispatcher dispatcher(e);

			//dispatcher.Dispatch<NetworkCreatedEvent>(BIND_EVENT_FN(Node::Init)); <- Might be virtuals
			//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN());
			//SOREN_CORE_TRACE(e);
			return;

		};
		#pragma endregion

		size_t mId;

		bool mRunning = true; // So as to allow links to be disabled and enabled
		bool mLocked = false;

		TWeight mWeight = 0;
	};


} // namespace Neural
} // namespace Soren

