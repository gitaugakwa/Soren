#include "olpch.h"
#include "Link/Link.h"

#include "nlohmann/json.hpp"

#include "Output/Output.h"

namespace Soren {
namespace Neural {

	//FileOutput<char>& operator<<(FileOutput<char>& fos, const Link& link)
	//{
	//	return fos << link.ToString();
	//}

	// NOT SURE IF WE NEED TO OUTPUT OUTNODEID

	bool Link::Export(Soren::FileOutput<Output_t>& fo, const Soren::ExportType exporttype) const
	{
		// For JSON, one must provide the FileOutput to the actual file

		if (!fo.isValid())
		{
			OL_CORE_ERROR("FileOutput provided is invalid");
			OL_CORE_ERROR("Link not exported");
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
		switch (exporttype)
		{
		case ExportType::Normal:
		{
			// Link


			// Opening
			fo.write(m_OpeningTag.c_str(), m_OpeningTag.size());

			fo.write((const Output_t*)&m_LinkID, sizeof(size_t));
			fo.write((const Output_t*)&m_InID, sizeof(size_t));
			fo.write((const Output_t*)&m_OutID, sizeof(size_t));
			fo.write((const Output_t*)&m_Weight, sizeof(Weight_t));
			fo.write((const Output_t*)&m_Running, sizeof(bool));
			fo.write((const Output_t*)&m_Locked, sizeof(bool));

			// Closing
			fo.write(m_ClosingTag.c_str(), m_ClosingTag.size());

			fo.flush();
			break;
		}
		case ExportType::Shrink:
		{
			// Link

			// for string shortening since no string should have 00 in it, this can be and is used as a delim
			char blank = 0;

			// Opening
			fo.write(m_OpeningTag.c_str(), m_OpeningTag.size());

			fo.write((const Output_t*)&m_LinkID, sizeof(size_t), blank);
			fo.write((const Output_t*)&m_InID, sizeof(size_t), blank);
			fo.write((const Output_t*)&m_OutID, sizeof(size_t), blank);
			fo.write((const Output_t*)&m_Weight, sizeof(Weight_t), blank);
			fo.write((const Output_t*)&m_Running, sizeof(bool), blank);
			fo.write((const Output_t*)&m_Locked, sizeof(bool), blank);

			// Closing
			fo.write(m_ClosingTag.c_str(), m_ClosingTag.size());

			fo.flush();
			break;
		}
		case ExportType::JSON:
		{
			fo << std::setw(4) << nlohmann::json(*this);
			fo.flush();
			break;
		}
		default:
		{
			// will Export Normal if None was set
			OL_CORE_WARN("ExportType was None");
			OL_CORE_WARN("Will Export with ExportType::Normal");
			return Export(fo, Soren::ExportType::Normal);
		}
		}

		return true;
	}

	void to_json(nlohmann::json& j, const Link& link)
	{
		using json = nlohmann::json;
		using ordered_json = nlohmann::ordered_json;

		j = ordered_json{
					{"Architecture",
						{
							{"InNodeID", link.inID()},
							{"OutNodeID", link.outID()}
						}
					},
					{"LinkID", link.id()},
					{"InNodeID", link.inID()},
					{"OutNodeID", link.outID()},
					{"Weight", link.weight()},
					{"Running", link.running()},
					{"Locked", link.locked()}
				};
	}

} // namespace Neural
} // namespace Soren