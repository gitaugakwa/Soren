#pragma once
#include "Core.h"
#include "Base.h"

#include "layData.h"

#include "Type.h"

#include "Log/Log.h"
#include "Events/Event.h"

#include "CallbackFunctions.h"

#include "nlohmann/json.hpp"

namespace Soren {
	class Event;

	template <typename T>
	class FileInput;
	template <typename T>
	class FileOutput;

namespace Neural {

	class SOREN_API Node;
	class SOREN_API Network;

	class SOREN_API Layer
	{
		friend class Train;

		friend class Node;
		friend class Network;

	public:

		Layer(Network& network, const std::string& name = "Layer");
		~Layer();

		// add begin and end which are the ones for the LayNodes

		// Layers
		size_t size() const;
		inline size_t id() const { return m_LayerID; }
		inline std::string title() const { return m_Title; }
		inline bool locked() const { return m_Locked; }
		size_t pos() const;
		inline size_t currentCall() const { return m_CurrentCall; }
		Network& network() const;

		void clear();

		void reset();

		inline void title(const std::string& name) { if (m_Locked) return; m_Title = name; }
		inline void lock(const bool& lock) { m_Locked = lock; }
		inline void lock() { m_Locked = true; }
		inline void unlock() { m_Locked = false; }

		// Callbacks
		void SetEventCallbackFunction(const EventCallbackFn& callback);
		inline void DefaultCallbacks() { m_Callbacks.Default(); }
		void DeleteEventCallback(const EventType& type);

		// Input
		bool InitializeInput();
		void PutToNodes();

		// Output
		bool InitializeOutput();
		void PullFromNodes();

		// OnEvent functions
		void AddEvent(const EventType& type, void(*func)(const Soren::Event&));
		void DeleteEvent(const EventType& type, void(*func)(const Soren::Event&));
		void DeleteEvent(const EventType& type, size_t number);

		// Node
		Accessor<Node>& Nodes(size_t nodes, const std::string& name = "Node");
		inline Accessor<Node>& Nodes() { return LayNodes; }
		Accessor<Node>& AddNodes(size_t nodes, const std::string& name = "Node");

		inline std::vector<Node>& GetAllNodes() { return m_Nodes; }
		Node& GetNode(size_t nodeid);
		inline std::vector<size_t>& GetNodeIDs() { return m_NodeIDs; }
		size_t FindNode(const Node& node);


		void DeleteNode(size_t id);
		void DeleteNode(const Node& node);

		Soren::Accessor<Node> LayNodes{};


		// Link
		void Links(const Node& node);
		void Links(const Layer& layer);

		// Locks

		void UnlockNodes();
		void UnlockLinks();

		// Element Access

		DataBuffer& GetInData() { return m_InBuffer; }
		DataBuffer& GetOutData() { return m_OutBuffer; }

		size_t& GetCurrentCall() { return m_CurrentCall; }

		std::string str(const std::string& layerpref = "", const std::string& layersuf = "", const std::string& nodepref = "", const std::string& nodesuf = "", const std::string& linkpref = "", const std::string& linksuf = "") const;

		// Import
		bool Import(Soren::FileInput<Input_t> fi, const ImportType importtype = ImportType::Normal, const bool override = true, const bool cascade = true);
		bool Import(const nlohmann::json& j, const bool override = true, const bool cascade = true);

		// Output
		bool Export(Soren::FileOutput<Output_t>& fo, const ExportType exporttype = ExportType::Normal, const bool cascade = true) const;

		// Public callbacks
		void SetLayerEventfunc(const EventType& type, LayerEventfunc func);

		// Special member functions
		Layer(const Layer& arg);
		Layer(Layer&& arg) noexcept;
		Layer& operator=(const Layer& arg);
		Layer& operator=(Layer&& arg) noexcept;

		// Operators
		bool operator<(const Layer& rhs) const;
		bool operator>(const Layer& rhs) const;
		bool operator<=(const Layer& rhs) const;
		bool operator>=(const Layer& rhs) const;

		Node& operator[](size_t rhs);
		bool operator==(const Layer& rhs) const;
		bool operator==(const Node& rhs) const;
		bool operator==(size_t rhs) const;
		bool operator!=(const Layer& rhs) const;
		bool operator!=(const Node& rhs) const;
		bool operator!=(size_t rhs) const;

		friend std::ostream& operator<<(std::ostream& os, const Layer& layer);
		//friend FileOutput<char>& operator<<(FileOutput<char>& fos, const Layer& layer);
		template <typename T>
		friend Soren::FileOutput<T>& operator<<(Soren::FileOutput<T>& fos, const Layer& layer)
		{
			fos << layer.str("","","\t","","\t\t");
			return fos;
		}

	private:

		// Layer function implementation

		// Events
		inline void Bind() { SetEventCallbackFunction(BIND_EVENT_FN(Layer::OnEvent)); }
		void CheckAdditionalEvents(const Soren::Event& e);
		void OnEvent(const Soren::Event& e);

		// File
		bool SeekBegin(Soren::FileInput<Input_t>& fi);
		bool SeekEnd(Soren::FileInput<Input_t>& fi);

		// Node functions
		void Put(Node& node);
		void Pull(const Node& node);

		// Data
		const size_t m_LayerID;
		std::string m_Title{};
		bool m_Locked = false;

		// Remember to reset when network is moved
		Network& m_Network;

		std::vector<Node> m_Nodes{};

		// resizing occurs during pulling and putting
		// m_InBuffer is dependent on the number of nodes linked
		DataBuffer m_InBuffer{};
		// m_OutBuffer is dependent on the number of nodes in the layer
		DataBuffer m_OutBuffer{};

		size_t m_CurrentCall = 0;

		std::vector<size_t> m_NodeIDs{};

		// Export
		// move to network
		const std::string m_OpeningTag = "<Layer>";
		const std::string m_ClosingTag = "</Layer>";

		LayerCallbacks m_Callbacks{};
		LayerEvents m_Events{};
	};

	void to_json(nlohmann::json& j, const Layer& layer);
	void from_json(const nlohmann::json& j, Layer& layer);

} // namespace Neural
} // namespace Soren

