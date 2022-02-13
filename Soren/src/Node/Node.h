#pragma once
#include "Core.h"
#include "Base.h"

#include "nodeData.h"

#include "Status/Status.h"
#include "Type.h"

#include "Log/Log.h"
#include "Events/Event.h"

#include "CallbackFunctions.h"

#include "nlohmann/json.hpp"

namespace Soren {

	template <typename T>
	class FileInput;
	template <typename T>
	class FileOutput;

	class Event;

namespace Neural {

	class SOREN_API Layer;
	class SOREN_API Link;
	class SOREN_API Network;

	class SOREN_API Node //: public Write
	{
		friend class Link;
		friend class Layer;
		friend class Network;

		friend class Train;
	public:
		Node(Layer& layer, const std::string& name = "Node");
		~Node();

		// Nodes
		size_t size() const;
		inline std::string title() const { return m_Title; }
		inline size_t id() const { return m_NodeID; }
		inline Node_t value() const { return m_Value; }
		inline Bias_t bias() const { return m_Bias; }
		inline size_t LayerID() const { return m_LayerID; }
		inline bool running() const { return m_Running; }
		inline bool locked() const { return m_Locked; }
		inline Layer& layer() const;

		void clear();

		void halt();
		void resume();

		void reset();

		inline void title(const std::string& title) { if (m_Locked) return; m_Title = title; }
		inline void bias(Bias_t bias) { if (m_Locked) return; m_Bias = bias; }
		inline void value(Node_t value) { if (m_Locked) return; m_Value = value; }
		inline void lock(bool lock) { m_Locked = lock; }
		inline void lock() { m_Locked = true; }
		inline void unlock() { m_Locked = false; }

		// Callbacks
		void SetEventCallbackFunction(const EventCallbackFn& callback);
		inline void DefaultCallbacks() { m_Callbacks.Default(); }
		void DeleteEventCallback(const EventType& type);

		// OnEvent functions
		void AddEvent(const EventType& type, void(*func)(const Event&));
		void DeleteEvent(const EventType& type, void(*func)(const Event&));
		void DeleteEvent(const EventType& type, const unsigned int& number);

		// Links
		//std::vector<Link*>& Links(const std::initializer_list<Node&>& nodes);
		Accessor<Link>& Links(const std::initializer_list<Node*>& nodes);
		Accessor<Link>& Links(const Node& node);
		Accessor<Link>& Links(const Node* node);
		Accessor<Link>& Links(const Layer& layer);
		Accessor<Link>& Links(const Layer* layer);

		inline std::vector<Link>& GetAllLinks() { return m_Links; }
		Link& GetLink(const Node& node);

		void DeleteLink(const Node& node);
		void DeleteLink(const Link& link);

		bool LinkCreated(const Node& node) const;

		// Link function implementation
		Node_t Resolve();

		Accessor<Link> NodeLinks{};

		//TODO
		//~Node();

		std::string str(const std::string& nodepref = "", const std::string& nodesuf = "", const std::string& linkpref = "", const std::string& linksuf = "") const;

		//Members

		// Input
		bool Import(FileInput<Input_t>& fi, const ImportType importtype = ImportType::Normal, const bool override = true, const bool cascade = true);
		bool Import(const nlohmann::json& j, const bool override = true, const bool cascade = true);

		// Output
		bool Export(FileOutput<Output_t>& fo, const ExportType exporttype = ExportType::Normal, const bool cascade = true) const;

		// Public callbacks
		inline void SetNodeEventfunc(const EventType& type, NodeEventfunc func);

		// Special member functions
		Node(const Node& arg);
		Node(Node&& arg) noexcept;
		Node& operator=(const Node& arg);
		Node& operator=(Node&& arg) noexcept;

		// Operators
		Link& operator[](size_t rhs);
		bool operator==(const Node& rhs) const;
		bool operator==(size_t rhs) const;
		bool operator!=(const Node& rhs) const;
		bool operator!=(size_t rhs) const;
		friend std::ostream& operator<<(std::ostream& os, const Node& e);
		template <typename T>
		friend FileOutput<T>& operator<<(FileOutput<T>& fos, const Node& node)
		{
			fos << node.str("","","\t");
			return fos;
		}

	private:

		// Event
		inline void Bind() { SetEventCallbackFunction(BIND_EVENT_FN(Node::OnEvent)); }
		void CheckAdditionalEvents(const Event& e);
		void OnEvent(const Soren::Event& e);

		// File
		bool SeekBegin(Soren::FileInput<Input_t>& fi);
		bool SeekEnd(Soren::FileInput<Input_t>& fi);

		// Const mambers

		// store position in vector cause all pointers in vector get invalidated after resize
		// do we want position to be stored in layer or always to be derived from network
		// what if the layer moves???
		// should layer id == to layer pos -> first layer == 0(id), 0(pos) what if layer insert that would change this ordering
		// 

		// Network
		Network& network();

		// Data

		const size_t m_LayerID;
		const size_t m_NodeID;
		std::string m_Title{};
		bool m_Running = true;
		bool m_Locked = false;

		Layer& m_Layer;

		//std::vector<unsigned int> ScopeIDs;

		std::vector<Link> m_Links{};

		Node_t m_Value = 0;
		Bias_t m_Bias = 0;

		// Export
		// move to network

		const std::string m_OpeningTag = "<Node>";
		const std::string m_ClosingTag = "</Node>";

		NodeCallbacks m_Callbacks{};
		NodeEvents m_Events{};
	};

	void to_json(nlohmann::json& j, const Node& node);
	void from_json(const nlohmann::json& j, Node& node);
} // namespace Neural
} // namespace Soren

