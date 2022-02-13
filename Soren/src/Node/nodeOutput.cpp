#include "olpch.h"
#include "Node/Node.h"

#include "nlohmann/json.hpp"

#include "Output/Output.h"

#include "Link/Link.h"

namespace Soren {
namespace Neural {

	//FileOutput<char>& operator<<(FileOutput<char>& fos, const Node& node)
	//{
	//	return fos << node.ToString();
	//}

			// Node
		//
		// Name of the node
		// NodeID
		// Bias
		// Running
		// Locked // we could fuse the two bool into one byte using shifting and all
		// Number of Links
		// Links
		//		-> LinkID // in the end, or in the optimized version, this should not matter
		//		-> InNode
		//		-> Weight
		//		-> Locked

	bool Node::Export(Soren::FileOutput<Output_t>& fo, const ExportType exporttype, const bool cascade) const
	{
		// For JSON, one must provide the FileOutput to the actual file

		if (!fo.isValid())
		{
			OL_CORE_ERROR("FileOutput provided is invalid");
			OL_CORE_ERROR("Node not exported");
			return false;
		}

		/*
		// Node
		//
		// Name of the node
		// NodeID
		// Bias
		// Running
		// Locked // we could fuse the two bool into one byte using shifting and all
		// Number of Links
		// Links
		//		-> LinkID // in the end, or in the optimized version, this should not matter
		//		-> InNode
		//		-> Weight
		//		-> Locked
		*/
		switch (exporttype)
		{
		case ExportType::Normal:
		{
			// Node

			// for string shortening since no string should have 00 in it, this can be and is used as a delim
			char blank = 0;

			size_t linkcount = m_Links.size();

			// Opening
			fo.write(m_OpeningTag.c_str(), m_OpeningTag.size());

			fo.write(m_Title.c_str(), m_Title.size());
			fo.write(&blank, 1);

			fo.write((const Output_t*)&m_NodeID, sizeof(size_t));
			fo.write((const Output_t*)&m_Bias, sizeof(Bias_t));
			fo.write((const Output_t*)&m_Running, sizeof(bool));
			fo.write((const Output_t*)&m_Locked, sizeof(bool));
			fo.write((const Output_t*)&linkcount, sizeof(size_t));

			break;
		}
		case ExportType::Shrink:
		{
			// Node

			// for string shortening since no string should have 00 in it, this can be and is used as a delim
			char blank = 0;

			size_t linkcount = m_Links.size();

			// Opening
			fo.write(m_OpeningTag.c_str(), m_OpeningTag.size());

			fo.write(m_Title.c_str(), m_Title.size(), true);

			fo.write((const Output_t*)&m_NodeID, sizeof(size_t), blank);
			fo.write((const Output_t*)&m_Bias, sizeof(Bias_t), blank);
			fo.write((const Output_t*)&m_Running, sizeof(bool), blank);
			fo.write((const Output_t*)&m_Locked, sizeof(bool), blank);
			fo.write((const Output_t*)&linkcount, sizeof(size_t), blank);

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
			// Links
			for (const Link& link : m_Links)
			{
				successfulExport = successfulExport && link.Export(fo, exporttype);
			}
		}

		// Closing
		fo.write(m_ClosingTag.c_str(), m_ClosingTag.size());

		fo.flush();
		return successfulExport;
	}

	std::string Node::str(const std::string& nodepref, const std::string& nodesuf, const std::string& linkpref, const std::string& linksuf) const
	{
		std::stringstream ss;

		ss << nodepref;
		ss << "Node[" << m_NodeID << "]";
		ss << ": " << m_Title;
		ss << " -> Layer[" << m_LayerID << "]";
		ss << " Value: " << std::setprecision(std::numeric_limits<Node_t>::digits10 + 1) << m_Value;
		ss << " Bias: " << std::setprecision(std::numeric_limits<Bias_t>::digits10 + 1) << m_Bias;
		ss << nodesuf;
		ss << std::endl;
		for (auto& link : m_Links)
		{
			ss << link.str(linkpref, linksuf);
		}

		return ss.str();
	}

	void to_json(nlohmann::json& j, const Node& node)
	{
		// links
		using json = nlohmann::json;
		using ordered_json = nlohmann::ordered_json;

		auto linksarray = [&]() -> json
		{
			json jarray = json::array();
			for (const Link& link : node.NodeLinks)
				jarray.emplace_back(link);
			return jarray;
		};

		j = ordered_json{
					{"Architecture",
						{
							{"Links", node.NodeLinks.size()}
						}
					},
					{"Title", node.title()},
					{"NodeID", node.id()},
					{"LayerID", node.LayerID()},
					{"Bias", node.bias()},
					{"Running", node.running()},
					{"Locked", node.locked()},
					{"Links", linksarray()}
				};
	}

} // namespace Neural
} // namespace Soren