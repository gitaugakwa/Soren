#include "olpch.h"
#include "Link/Link.h"

#include "nlohmann/json.hpp"

#include "Node/Node.h"

#include "Input/Input.h"

namespace Soren {
namespace Neural {

	bool Link::Import(FileInput<Input_t>& fi, const ImportType importtype, const bool override)
	{
		// cascade is unused

		// Import will take directly from the raw files
		// If override is false, then nothing is changed

		if (!fi.isValid())
		{
			OL_CORE_ERROR("FileInput provided is invalid");
			OL_CORE_ERROR("Link not imported");
			return false;
		}

		// Look for the data for the current node
		//for (size_t i = m_Node.NodeLinks[0].id(); i < m_LinkID; ++i)
			//SeekEnd(fi);

		if (!SeekBegin(fi)) {
			OL_CORE_ERROR("Unable to find beginning of Link Data");
			OL_CORE_ERROR("Link not imported");
			return false;
		}

		/*
		// Link
		//
		// LinkID
		// InID
		// OutID
		// Weight
		// Running
		// Locked
		*/

		// temp so that in case of error, the original data isn't overwritten
		//std::string title;
		size_t linkid = 0;
		size_t inid = 0;
		size_t outid = 0;
		Weight_t weight = 0;
		bool running = true;
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

			//std::stringstream ss;
			//fi.get(*ss.rdbuf(), blank);
			// check if we need to get the delim
			//fi.ignore();

			//title = ss.str();

			fi.read(linkid);
			fi.read(inid);
			fi.read(outid);
			fi.read(weight);
			fi.read(running);
			fi.read(locked);

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

			//auto width = [&] {
			//	char temp;
			//	fi.get(temp);
			//	return temp;
			//};

			//char length = width();
			//title.resize(length);
			//fi.read(title.data(), length);

			fi.read(linkid, true);
			fi.read(inid, true);
			fi.read(outid, true);
			fi.read(weight, true);
			fi.read(running, true);
			fi.read(locked, true);

			break;
		}
		}

		if (override)
		{
			// if no errors have been triggered, set members
			// the counters don't need to be set cause the Imporing will increment them in the counter
			// and it will allow for a simple conversion from master -> optimized
			//m_Title = title;
			m_InID = inid;
			m_OutID = outid;
			m_Weight = weight;
			m_Running = running;
			m_Locked = locked;
		}

		return true;
	}

	bool Link::Import(const nlohmann::json& j, const bool override)
	{
		std::string attributes[]{ "Architecture","InNodeID","OutNodeID","Weight","Running","Locked"};
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
			j["InNodeID"].get_to(m_InID);
			j["OutNodeID"].get_to(m_OutID);
			j["Weight"].get_to(m_Weight);
			j["Running"].get_to(m_Running);
			j["Locked"].get_to(m_Locked);
		}

		return true;
	}

	void from_json(const nlohmann::json& j, Link& link)
	{
		link.Import(j);
	}
} // namespace Neural
} // namespace Soren