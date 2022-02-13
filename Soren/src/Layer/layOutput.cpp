#include "olpch.h"
#include "Layer/Layer.h"

#include <variant>

#include "nlohmann/json.hpp"

#include "Output/Output.h"

#include "Node/Node.h"

namespace Soren {
namespace Neural {

	//FileOutput<char>& operator<<(FileOutput<char>& fos, const Layer& layer)
	//{
	//	return fos << layer.ToString();
	//}

	bool Layer::InitializeOutput()
	{
		m_OutBuffer.resize(size());
		m_OutBuffer.initialized = true;

		return m_OutBuffer.initialized;
	}

	void Layer::PullFromNodes()
	{
		for (Node& node : m_Nodes)
		{
			Pull(node);
		}
	}

	// Layer
	//
	// Name of the layer
	// LayerID
	// Locked
	// Number of Nodes
	//		-> all nodes in layer
	//

	bool Layer::Export(FileOutput<Output_t>& fo, ExportType exporttype, const bool cascade) const
	{
		// For JSON, one must provide the FileOutput to the actual file

		if (!fo.isValid())
		{
			OL_CORE_ERROR("FileOutput provided is invalid");
			OL_CORE_ERROR("Layer not exported");
			return false;
		}

		/*
		// Layer
		//
		// Name of the layer
		// LayerID
		// Locked
		// Number of Nodes
		//		-> all nodes in layer
		*/
		switch (exporttype)
		{
		case ExportType::Normal:
		{
			// Layer

			// for string shortening since no string should have 00 in it, this can be and is used as a delim
			char blank = 0;

			size_t nodecount = size();

			// Opening
			fo.write(m_OpeningTag.c_str(), m_OpeningTag.size());

			fo.write(m_Title.c_str(), m_Title.size());
			fo.write(&blank, 1);

			fo.write((const Output_t*)&m_LayerID, sizeof(size_t));
			fo.write((const Output_t*)&m_Locked, sizeof(bool));
			fo.write((const Output_t*)&nodecount, sizeof(size_t));

			break;
		}
		case ExportType::Shrink:
		{
			// Layer

			// for string shortening since no string should have 00 in it, this can be and is used as a delim
			char blank = 0;

			size_t nodecount = size();

			// Opening
			fo.write(m_OpeningTag.c_str(), m_OpeningTag.size());

			fo.write(m_Title.c_str(), m_Title.size(), true);

			fo.write((const Output_t*)&m_LayerID, sizeof(size_t), blank);
			fo.write((const Output_t*)&m_Locked, sizeof(bool), blank);
			fo.write((const Output_t*)&nodecount, sizeof(size_t), blank);

			break;
		}
		case ExportType::JSON:
		{
			fo << std::setw(4) << nlohmann::json(*this);
			fo.flush();
			return true;
		}
		default:
		{
			// will Export Normal if None was set
			OL_WARN("ExportType was None");
			OL_WARN("Will Export with ExportType::Normal");
			return Export(fo, ExportType::Normal, cascade);
		}
		}

		bool successfulExport = true;
		if (cascade)
		{
			// Nodes
			for (const Node& node : m_Nodes)
			{
				successfulExport = successfulExport && node.Export(fo, exporttype, cascade);
			}
		}

		// Closing
		fo.write(m_ClosingTag.c_str(), m_ClosingTag.size());

		fo.flush();
		return successfulExport;
	}


	std::string Layer::str(const std::string& layerpref, const std::string& layersuf, const std::string& nodepref, const std::string& nodesuf, const std::string& linkpref, const std::string& linksuf) const
	{
		std::stringstream ss;

		ss << layerpref;
		ss << "Layer[" << m_LayerID << "]";
		ss << ": " << m_Title;
		ss << layersuf;
		ss << std::endl;
		if (!m_InBuffer.empty())
		{
			ss << "InBuffer: ";

			for (size_t i = 0; i < m_InBuffer.size(); i++)
			{
				ss << "[" << m_InBuffer.at(i).NodeID << "]" << " : " << "{" << m_InBuffer.at(i).NodeValue << "}";

				if (i != m_InBuffer.size() - 1)
					ss << ", ";
			}
			ss << layersuf;
			ss << std::endl;
		}
		if (!m_OutBuffer.empty())
		{
			ss << "OutBuffer: ";

			for (size_t i = 0; i < m_InBuffer.size(); i++)
			{
				ss << "[" << m_OutBuffer.at(i).NodeID << "]" << " : " << "{" << m_OutBuffer.at(i).NodeValue << "}";

				if (i != m_OutBuffer.size() - 1)
					ss << ", ";
			}
			ss << layersuf;
			ss << std::endl;
		}
		for (auto& node : m_Nodes)
		{
			ss << node.str(nodepref, nodesuf, linkpref, linksuf);
		}

		return ss.str();
	}

	void to_json(nlohmann::json& j, const Layer& layer)
	{
		using json = nlohmann::json;
		using ordered_json = nlohmann::ordered_json;

		auto linksize = [&]() -> size_t
		{
			size_t linkcount = 0;
			for (const Node& node : layer.LayNodes)
				linkcount += node.size();
			return linkcount;
		};

		auto nodesarray = [&]() -> json
		{
			json jarray = json::array();
			for (const Node& node : layer.LayNodes)
				jarray.emplace_back(node);
			return jarray;
		};

		j = ordered_json{
					{"Architecture", 
						{
							{"Nodes", layer.LayNodes.size()},
							{"Links", linksize()}
						}
					},
					{"Title", layer.title()},
					{"LayerID", layer.id()},
					{"Locked", layer.locked()},
					{"Nodes", nodesarray()}
				};


	}

} // namespace Neural
} // namespace Soren