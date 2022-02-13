#include "olpch.h"
#include "Layer/Layer.h"

#include "nlohmann/json.hpp"

#include "Node/Node.h"

#include "Input/Input.h"


namespace Soren {
namespace Neural {

	bool Layer::InitializeInput()
	{
		auto links = [&]()
		{
			size_t linkcount = 0;
			for (Node& node : m_Nodes)
			{
				linkcount += node.size();
			}
			return linkcount;
		};

		m_InBuffer.resize(links());
		m_InBuffer.initialized = true;

		return m_InBuffer.initialized;
	}

	void Layer::PutToNodes()
	{
		for (Node& node : m_Nodes)
		{
			Put(node);
		}
	}

	bool Layer::Import(Soren::FileInput<Input_t> fi, const ImportType importtype, const bool override, const bool cascade)
	{
		// Import will take directly from the raw files
		// If override is false, then nothing is changed

		if (!fi.isValid())
		{
			OL_CORE_ERROR("FileInput provided is invalid");
			OL_CORE_ERROR("Layer not imported");
			return false;
		}

		// Look for the data for the current layer
		for (size_t i = 1; i < m_LayerID; ++i)
			SeekEnd(fi);

		if (!SeekBegin(fi)) {
			OL_CORE_ERROR("Unable to find beginning of Layer Data");
			OL_CORE_ERROR("Layer not imported");
			return false;
		}

		// Layer
		//
		// Name of the layer
		// LayerID
		// Locked
		// Number of Nodes
		//		-> all nodes in layer
		//

		char blank = 0;

		// temp so that in case of error, the original data isn't overwritten
		std::string title;
		size_t layerid = 0;
		size_t nodecount = 0;
		bool locked = false;

		switch (importtype)
		{
		case ImportType::Normal:
		{
			/*
			// will not sync JSON files
			// will not import JSON files
			// will import .nn file
			*/

			std::stringstream ss;
			fi.get(*ss.rdbuf(), blank);
			// check if we need to get the delim
			fi.ignore();

			title = ss.str();

			fi.read(layerid);
			fi.read(locked);
			fi.read(nodecount);

			break;
		}
		case ImportType::Shrink:
		{
			/*
			// will not sync JSON files
			// will not import JSON files
			// will import .snn file
			*/
			// template <T>
			// read(T& object, bool readsize)

			auto width = [&] {
				char temp;
				fi.get(temp);
				return temp;
			};

			char length = width();
			title.resize(length);
			fi.read(title.data(), length);

			fi.read(layerid, true);
			fi.read(locked, true);
			fi.read(nodecount, true);

			break;
		}
		}

		if (override)
		{
			// if no errors have been triggered, set members
			// the counters don't need to be set cause the Imporing will increment them in the counter
			// and it will allow for a simple conversion from master -> optimized
			m_Title = title;
			m_Locked = locked;
		}

		// Nodes
		bool successfulExport = true;
		if (cascade && override) {
			std::vector<Node> nodes;
			nodes.reserve(nodecount);
			for (size_t i = 0; i < nodecount; ++i) {
				nodes.emplace_back(*this);
			}

			m_Nodes = std::move(nodes);

			for (Node& node : m_Nodes) {
				successfulExport = successfulExport && node.Import(fi, importtype, override, cascade);
			}
			// then import for each node
		}
		return successfulExport;
	}

	bool Layer::Import(const nlohmann::json& j, const bool override, const bool cascade)
	{
		// Ensure that j is proper

		std::string attributes[]{ "Architecture","Title","LayerID","Locked","Nodes" };
		for (auto& attrib : attributes)
		{
			if (j[attrib].is_null())
			{
				OL_CORE_WARN("{0} does not exist in JSON", attrib);
				OL_CORE_WARN("{0} will be null", attrib);
			}
		}

		if (override)
		{
			j["Title"].get_to(m_Title);
			j["Locked"].get_to(m_Locked);
		}

		bool successfulExport = true;
		if (cascade && override)
		{
			nlohmann::json nodesjson = j["Nodes"].get<nlohmann::json>();
			size_t nodecount = j["Architecture"]["Nodes"].get<size_t>();

			std::vector<Node> nodes;
			nodes.reserve(nodecount);
			for (size_t i = 0; i < nodecount; ++i) {
				nodes.emplace_back(*this);
			}

			m_Nodes = std::move(nodes);

			for (size_t i = 0; i < nodecount; ++i) {
				successfulExport = successfulExport && m_Nodes[i].Import(nodesjson[i].get<nlohmann::json>() , override, cascade);
			}
		}
		return successfulExport;
	}

	void from_json(const nlohmann::json& j, Layer& layer)
	{
		layer.Import(j);
	}
} // namespace Neural
} // namespace Soren