#pragma once
#include "Core.h"
#include "Base.h"

#include "Status/Status.h"
#include "Type.h"

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

	class Node;

	class Event;

	class SOREN_API Link
	{
		friend class Node;
		friend class Layer;
		friend class Network;
	public:
		Link(Node& node, const Node& input);
		~Link();

		// Links
		inline size_t id() const { return m_LinkID; }
		inline Weight_t weight() const { return m_Weight; }
		inline bool running() const { return m_Running; }
		inline bool locked() const { return m_Locked; }
		inline size_t inID() const { return m_InID; }
		inline size_t outID() const { return m_OutID; }

		void clear();

		void halt();
		void resume();

		void reset();

		inline void weight(Weight_t weight) { if (m_Locked) return; m_Weight = weight; }
		inline void lock(bool lock) { m_Locked = lock; }
		inline void lock() { m_Locked = true; }
		inline void unlock() { m_Locked = false; }

		inline void ChangeWeight(Weight_t difference) { if (m_Locked) return; m_Weight += difference; }

		// Callbacks
		void SetEventCallbackFunction(const EventCallbackFn& callback);
		void DefaultLinkCallbacks();
		void DeleteEventCallback(const EventType& type);

		// OnEvent functions
		// Events added to links will be to all links
		void AddEvent(const EventType& type, void(*func)(const Soren::Event&));
		void DeleteEvent(const EventType& type, void(*func)(const Soren::Event&));
		void DeleteEvent(const EventType& type, const unsigned int& number);

		std::string str(const std::string& linkpref = "", const std::string& linksuf = "") const;

		// Import
		bool Import(FileInput<Input_t>& fi, const ImportType importtype = ImportType::Normal, const bool override = true);
		bool Import(const nlohmann::json& j, const bool override = true);

		// Output
		bool Export(FileOutput<Output_t>& fo, const ExportType exporttype = ExportType::Normal) const;

		// Public callbacks
		void SetLinkEventfunc(const EventType& type, LinkEventfunc func);

		// Special member functions
		Link(const Link& arg);
		Link(Link&& arg) noexcept;
		Link& operator=(const Link& arg);
		Link& operator=(Link&& arg) noexcept;


		// Operators
		bool operator==(const Link& rhs) const;
		bool operator==(const Node& rhs) const;
		bool operator!=(const Link& rhs) const;
		bool operator!=(const Node& rhs) const;
		friend std::ostream& operator<<(std::ostream& os, const Link& link);
		template <typename T>
		friend FileOutput<T>& operator<<(Soren::FileOutput<T>& fos, const Link& link)
		{
			fos << link.str();
			return fos;
		}

		Node_t m_InBuffer = 0; // Not sure

	private:

		// Events
		inline void Bind() { SetEventCallbackFunction(BIND_EVENT_FN(Link::OnEvent)); }
		void CheckAdditionalEvents(const Soren::Event& e);
		void OnEvent(const Soren::Event& e);

		// File
		bool SeekBegin(Soren::FileInput<Input_t>& fi);
		bool SeekEnd(Soren::FileInput<Input_t>& fi);

		// Data
		const size_t m_LinkID;

		// Network
		Network& network();

		Weight_t m_Weight = 0;

		bool m_Running = true; // So as to allow links to be disabled and enabled

		size_t m_InID = 0;
		size_t m_OutID = 0;
		bool m_Locked = false;

		Node& m_Node;

		// Export
		// move to network

		const std::string m_OpeningTag = "<Link>";
		const std::string m_ClosingTag = "</Link>";
	};

	void to_json(nlohmann::json& j, const Link& link);
	void from_json(const nlohmann::json& j, Link& link);

} // namespace Neural
} // namespace Soren

