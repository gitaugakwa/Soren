#pragma once
//#include <filesystem>
#include "Core.h"
#include "Base.h"

#include "netData.h"
#include "Link/linkData.h"

#include "Status/Status.h"
#include "Type.h"
#include "Random/Random.h"

#include "Log/Log.h"
#include "Events/Event.h"

#include "Input/Input.h"
#include "Output/Output.h"

#include "CallbackFunctions.h"

#include "nlohmann/json.hpp"


// Find a way to have Link events static per network
// Everything static needs to be changed relative to network
namespace Soren {

namespace Neural {

	class SOREN_API Link;
	class SOREN_API Node;
	class SOREN_API Layer;

	class SOREN_API Network
	{
		//#define BIND_EVENT_FN(x) std::bind(&Network::x, this, std::placeholders::_1)
		friend class Input;
		friend class Output;

		friend class Train;

		friend class Layer;
		friend class Node;
		friend class Link;
	public:

		Network();
		~Network();

		// Network

		inline void title(const std::string& title) { if (m_Locked) return; m_Title = title; }
		bool NetworkValid() const;
		void clear();
		void reset();

		void init();
		void deinit();

		inline std::string title() const { return m_Title; }
		inline bool locked() const { return m_Locked; }
		size_t size() const;
		
		void locked(bool lock) { m_Locked = lock; }
		void lock() { m_Locked = true; }
		void unlock() { m_Locked = false; }

		//void SetActivationFunction(ActivationFunction activation);
		//void SetActivationFunction(lambda func) -> lambda that takes in Node_t then returns Node_t

		// Callbacks
		inline void SetEventCallbackFunction(const EventCallbackFn& callback) { m_EventCallback = callback; }
		inline void DefaultCallbacks() { m_Callbacks.Default(); }
		void DeleteEventCallback(const EventType& type);

		// Data functions

		// Input
		void AddInput(Soren::FileInput<Input_t>& fi);
		void AddInput(const std::string& name, const std::filesystem::path& filepath, std::ios::openmode mode);
		void DeleteInput(const std::string& name);

		// Import
		bool Import(const ImportType importtype = ImportType::Normal, const bool override = true, const bool cascade = true);
		bool Import(Soren::FileInput<Input_t> fi, const ImportType importtype = ImportType::Normal, const bool override = true, const bool cascade = true);
		bool Import(const nlohmann::json j, const ImportType importtype = ImportType::Normal, const bool override = true, const bool cascade = true);

		Soren::Accessor<FileInput<Input_t>> FileInputs;

		// Output
		void AddOutput(Soren::FileOutput<Output_t>& fo);
		void AddOutput(const std::string& name, const std::filesystem::path& filepath, std::ios::openmode mode);
		void DeleteOutput(const std::string& name);

		// Export
		bool Export(const ExportType exporttype = ExportType::Normal, const bool cascade = true) const;
		bool Export(Soren::FileOutput<Output_t>& fo, const ExportType exporttype = ExportType::Normal, const bool cascade = true) const;

		Soren::Accessor<FileOutput<Output_t>> FileOutputs;

		// Sync
		void JSONSync(const std::string& networkname = "Network") const;

		// OnEvent functions
		void AddEvent(const EventType& eventtype, void(*func)(const Soren::Event&));
		void DeleteEvent(const EventType& type, void (*func)(const Soren::Event&));
		void DeleteEvent(const EventType& type, size_t number);

		// Turn this into a vector like opject with push and stuff like that
		// Layer
		inline Soren::Accessor<Layer>& Layers() { return NetLayers; }
		Soren::Accessor<Layer>& Layers(const size_t layers, const std::string& name = "Layer");
		Soren::Accessor<Layer>& Layers(const size_t layers, size_t nodes, const std::string& name = "Layer");
		Soren::Accessor<Layer>& AddLayers(const size_t layers, const std::string& name = "Layer");
		Soren::Accessor<Layer>& AddLayers(const size_t layers, size_t nodes, const std::string& name = "Layer");

		Layer* GetLayer(const size_t id);
		Layer* GetLayer(const Node& id);
		Layer* GetLayerPos(const size_t pos);
		// Get the position in the vector that the layer is in
		size_t FindLayer(const Layer& layer) const;
		size_t FindLayer(const size_t layerid) const;

		void InsertLayer(const size_t pos, const std::string& name = "Layer");
		void DeleteLayer(const size_t id);
		void DeleteLayer(const Layer& layer);

		void SetPutFunc(void(*func)(Layer& layer));
		void SetPullFunc(void(*func)(const Layer& layer));

		void Put(Layer& layer);
		void Pull(const Layer& layer);
		
		Soren::Accessor<Layer> NetLayers{};

		// Node
		void Nodes(size_t nodes);
		void NodesDefaultBiasLimits(Bias_t min, Bias_t max);
		void NodesDefaultBiasLimits(Bias_t max);

		void DeleteNode(const Node& node);
		void DeleteNode(const size_t id);

		size_t DefaultNodeCount{};

		// Link functions
		void Links(const Layer& source, Layer& output);
		void LinksDefaultWeightLimits(Weight_t min, const Weight_t& max);
		void LinksDefaultWeightLimits(Weight_t max);

		std::string str(const std::string& networkpref = "", const std::string& networksuf = "", const std::string& layerpref = "", const std::string& layersuf = "", const std::string& nodepref = "", const std::string& nodesuf = "", const std::string& linkpref = "", const std::string& linksuf = "") const;

		// Public callbacks
		void SetNetworkEventfunc(const EventType& type, NetworkEventfunc func);

		// Status functions
		inline std::string GetStatus() const { return m_Status.str(); }

		// Special member functions
		Network(const Network& arg);
		Network(Network&& arg) noexcept;
		Network& operator=(const Network& arg);
		Network& operator=(Network&& arg) noexcept;

		// Operators
		Layer& operator[](size_t rhs);
		bool operator==(const Network& rhs) const;
		bool operator==(const Layer& rhs) const;
		bool operator!=(const Network& rhs) const;
		bool operator!=(const Layer& rhs) const;

		friend std::ostream& operator<<(std::ostream& os, const Network& network);
		//friend FileOutput<char>& operator<<(FileOutput<char>& fos, const Network& network);
		template <typename T>
		friend Soren::FileOutput<T>& operator<<(Soren::FileOutput<T>& fos, const Network& network)
		{
			fos << network.str("", "", "\t", "", "\t\t", "", "\t\t\t");
			return fos;
		}

	private:

		// Events
		inline void Bind() { SetEventCallbackFunction(BIND_EVENT_FN(Network::OnEvent)); }
		void CheckAdditionalEvents(const Soren::Event& e);
		void OnEvent(const Soren::Event& e);

		// Status function
		void SetStatus(const StatusCategory& status);
		inline NetworkStatus& GetStatusFlags() { return m_Status; }
		bool Is(const StatusCategory& status) const { return m_Status.IsInCategory(status); }

		// File
		bool SeekBegin(Soren::FileInput<Input_t>& fi);
		bool SeekEnd(Soren::FileInput<Input_t>& fi);

		// Layer
		size_t m_LayerCounter = 0;
		EventCallbackFn m_LayerEventCallback{};

		// Node
		Bias m_BiasData{};
		inline Bias BiasData() const { return m_BiasData; }
		void BiasData(Bias_t min, Bias_t max);
		void BiasData(Bias_t max);

		size_t m_NodeCounter = 0;
		EventCallbackFn m_NodeEventCallback{};

		// Link
		Weight m_WeightData{};
		inline Weight WeightData() const { return m_WeightData; }
		void WeightData(Weight_t min, Weight_t max);
		void WeightData(Weight_t max);

		size_t m_LinkCounter = 0;
		EventCallbackFn m_LinkEventCallback{};

		LinkCallbacks m_LinkCallbacks{};

		LinkEvents m_LinkEvents{};

		// Data

		std::string m_Title{};
		bool m_Locked = false;

		EventCallbackFn m_EventCallback{};

		std::vector<Layer> m_Layers{};

		NetworkStatus m_Status{};

		// maybe vector so that the user can store functions that can then be easily accessed through the network itself
		// this i guess allows for better use, like emplace_back (name of function, actual function)
		std::function<void(Layer& layer)> m_PutFunc = nullptr;
		std::function<void(const Layer& layer)> m_PullFunc = nullptr;

		std::vector<FileInput<Input_t>> m_FileInputs{};
		std::vector<FileOutput<Output_t>> m_FileOutputs{};

		// Directories

		std::filesystem::path m_BaseDir= "./";

		// Export

		const std::string m_OpeningTag = "<Network>";
		const std::string m_ClosingTag = "</Network>";

		NetworkCallbacks m_Callbacks{};
		NetworkEvents m_Events{};
	};

	void to_json(nlohmann::json& j, const Network& network);
	//void from_json(const nlohmann::json& j, Network& network);
} // namespace Neural



} // namespace Soren
