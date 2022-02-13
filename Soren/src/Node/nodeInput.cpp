#include "olpch.h"
#include "Node/Node.h"

#include "nlohmann/json.hpp"

#include "Network/Network.h"
#include "Layer/Layer.h"
#include "Link/Link.h"

#include "Input/Input.h"

namespace Soren {
namespace Neural {

	bool Node::Import(FileInput<Input_t>& fi, const ImportType importtype, const bool override, const bool cascade)
	{
		// Import will take directly from the raw files
		// If override is false, then nothing is changed

		if (!fi.isValid())
		{
			OL_CORE_ERROR("FileInput provided is invalid");
			OL_CORE_ERROR("Node not imported");
			return false;
		}

		// have something like if it finds a layer nor network close tag before,
		// then it warns the user that it's in another layer or network
		// Look for the data for the current node
		// since it's taken by reference, the it should be fine
		//for (size_t i = m_Layer.LayNodes[0].id(); i < m_NodeID; ++i)
			//SeekEnd(fi);

		if (!SeekBegin(fi)) {
			OL_CORE_ERROR("Unable to find beginning of Node Data");
			OL_CORE_ERROR("Node not imported");
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

		char blank = 0;

		// temp so that in case of error, the original data isn't overwritten
		std::string title;
		size_t nodeid = 0;
		Bias_t bias = 0;
		bool running = false;
		bool locked = false;
		size_t linkcount = 0;

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

			fi.read(nodeid);
			fi.read(bias);
			fi.read(running);
			fi.read(locked);
			fi.read(linkcount);

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

			fi.read(nodeid, true);
			fi.read(bias, true);
			fi.read(running, true);
			fi.read(locked, true);
			fi.read(linkcount, true);

			break;
		}
		}

		if (override)
		{
			// if no errors have been triggered, set members
			// the counters don't need to be set cause the Imporing will increment them in the counter
			// and it will allow for a simple conversion from master -> optimized
			m_Title = title;
			m_Bias = bias;
			m_Running = running;
			m_Locked = locked;
		}

		// Layers
		bool successfulExport = true;
		if (cascade && override) {
			std::vector<Link> links;
			links.reserve(linkcount);
			for (size_t i = 0; i < linkcount; ++i) {
				// this won't work cause what if the links are way larger than it and what if the link is to a different layer
				links.emplace_back(*this, *this);
			}

			m_Links = std::move(links);

			for (Link& link : m_Links) {
				successfulExport = successfulExport && link.Import(fi, importtype, override);
			}
			// then import for each link
		}

		return successfulExport;
	}

	bool Node::Import(const nlohmann::json& j, const bool override, const bool cascade)
	{
		// Ensure that j is proper

		std::string attributes[]{ "Architecture","Title","NodeID","LayerID","Running","Bias","Locked","Links" };
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
			j["Running"].get_to(m_Running);
			j["Bias"].get_to(m_Bias);
			j["Locked"].get_to(m_Locked);
		}

		bool successfulExport = true;
		if (cascade && override)
		{
			nlohmann::json linksjson = j["Links"].get<nlohmann::json>();
			size_t linkcount = j["Architecture"]["Links"].get<size_t>();

			std::vector<Link> links;
			links.reserve(linkcount);
			for (size_t i = 0; i < linkcount; ++i) {
				links.emplace_back(*this, *this);
			}

			m_Links = std::move(links);

			for (size_t i = 0; i < linkcount; ++i) {
				successfulExport = successfulExport && m_Links[i].Import(linksjson[i].get<nlohmann::json>(), override);
			}
		}
		return successfulExport;
	}

	void from_json(const nlohmann::json& j, Node& node)
	{
		node.Import(j);
	}
} // namespace Neural
} // namespace Soren