#include "olpch.h"
#include "Network/Network.h"

#include <variant>

#include "nlohmann/json.hpp"

#include "Layer/Layer.h"
#include "Node/Node.h"
#include "Link/Link.h"

#include "Output/Output.h"

namespace Soren {
namespace Neural {

	void Network::AddOutput(Soren::FileOutput<Output_t>& fo)
	{
		m_FileOutputs.push_back(std::move(fo));
	}

	void Network::AddOutput(const std::string& name, const std::filesystem::path& filepath, std::ios::openmode mode)
	{
		m_FileOutputs.emplace_back(name, filepath, mode);
	}

	void Network::DeleteOutput(const std::string& name)
	{
		auto pos = std::find(m_FileOutputs.begin(), m_FileOutputs.end(), name);
		if (pos != m_FileOutputs.end())
		{
			m_FileOutputs.erase(pos);
		}

		OL_CORE_ERROR("Output with Name: {0} not found", name);
		assert(false);
	}

	// Network exportation both to raw and json

	bool Network::Export(const ExportType exporttype, const bool cascade) const
	{
		/*
		// Structure of network.nn
		//
		// Network
		//
		// Name of the network
		// Number of Layers
		// Number of Nodes
		// Number of Links
		// Locked
		//		-> all layers in network
		//
		// Add when more data is required
		*/

		Soren::FileOutput<Output_t> Exportfo;

		switch (exporttype)
		{
		case ExportType::Normal:
		{
			/*
			// will not export JSON files
			// will export .nn file with no shrinking
			*/
			//std::filesystem::create_directories(LayerData); -> might use this later



			std::filesystem::path exportpath = m_BaseDir;
			exportpath /= "config";
			std::filesystem::create_directory(exportpath);

			// remember to allow input to take in a name
			exportpath /= m_Title;
			exportpath += ".nn";

			Exportfo.open(exportpath, std::ios_base::binary | std::ios_base::out);

			// Network

			// for string shortening since no string should have 00 in it, this can be and is used as a delim
			char blank = 0;

			size_t nodecount = 0;
			size_t linkcount = 0;
			size_t layercount = size();
			for (const Layer& layer : m_Layers)
			{
				nodecount += layer.size();
				for (const Node& node : layer.m_Nodes)
					linkcount += node.size();
			}

			// Opening
			Exportfo.write(m_OpeningTag.c_str(), m_OpeningTag.size());

			Exportfo.write(m_Title.c_str(), m_Title.size());
			Exportfo.write(&blank, 1);

			Exportfo.write(layercount);
			Exportfo.write(nodecount);
			Exportfo.write(linkcount);
			Exportfo.write(m_Locked);
			Exportfo.flush();
				
			break;
		}
		case ExportType::Shrink:
		{
			/*
			// THIS WILL TRY TO SHRINK DATA AS MUCH AS POSSIBLE

			// will not export JSON files
			// will export .snn file with shrinking
			*/


			std::filesystem::path exportpath = m_BaseDir;
			exportpath /= "config";
			std::filesystem::create_directory(exportpath);

			// remember to allow input to take in a name
			exportpath /= m_Title;
			exportpath += ".snn";

			Exportfo.open(exportpath, std::ios_base::binary | std::ios_base::out);

			// Network

			// for string shortening since no string should have 00 in it, this can be and is used as a delim
			char blank = 0;

			size_t nodecount = 0;
			size_t linkcount = 0;
			size_t layercount = size();
			for (const Layer& layer : m_Layers)
			{
				nodecount += layer.size();
				for (const Node& node : layer.m_Nodes)
					linkcount += node.size();
			}

			// Opening
			Exportfo.write(m_OpeningTag.c_str(), m_OpeningTag.size());

			Exportfo.write(m_Title.c_str(), m_Title.size(), true);

			// this is worth for shrinking, but would require more calculations when importing
			// cause we'd have to look for the next delim,
			// see the size of the data being held in the file
			// position the pointer correctly when importing
			// we slit it to shrink export, shrink import -> different file types
			// normal will be a .nn
			// shrink will be a .snn
			// this is for whole files that do not require the different files
			// actually, we do away with .wb files and have a raw one .nn file
			// this will hold the network attributes
			// if we need another, maybe a .config
			// what if there's a 00 in the middle of some data

			Exportfo.write((const Output_t*)&layercount, sizeof(size_t), blank);
			Exportfo.write((const Output_t*)&nodecount, sizeof(size_t), blank);
			Exportfo.write((const Output_t*)&linkcount, sizeof(size_t), blank);
			Exportfo.write((const Output_t*)&m_Locked, sizeof(bool), blank);
			Exportfo.flush();

			break;
		}
		case ExportType::JSON:
		{
			// cascade -> true		-> also output Layer[].json files
			// cascade -> false		-> will not output Layer[].json files
			// JSON will export

			// This will have to be reworked for multithreading

			std::filesystem::path exportpath = m_BaseDir;
			exportpath /= "config";

			Soren::FileOutput<Output_t> Export;

			// Network
			std::filesystem::path networkpath = exportpath;
			// remember to allow input to take in a name
			networkpath /= m_Title;
			networkpath += ".json";

			Export.open(networkpath, std::ios_base::binary | std::ios_base::out);
			Export << std::setw(4) << nlohmann::json(*this);

			if (cascade)
			{
				Export.close();

				std::filesystem::path layerbase = exportpath;

				// Layers folder
				layerbase /= "Layers";
				for (const Layer& layer : m_Layers)
				{
					std::filesystem::path layerpath = layerbase;

					// individual layers
					std::filesystem::create_directories(layerpath);

					layerpath /= "Layer";
					layerpath += "[";
					// would rather use position in the network than ID
					layerpath += std::to_string(Network::FindLayer(layer));
					layerpath += "]";
					layerpath += ".json";

					Export.open(layerpath, std::ios_base::binary | std::ios_base::out);
					Export << std::setw(4) << nlohmann::json(layer);
					Export.close();
				}
			}
			return true;
		}
		default:
		{
			// will Export Normal if None was set
			OL_CORE_WARN("ExportType was None");
			OL_CORE_WARN("Will Export with ExportType::Normal");
			return Export(ExportType::Normal, cascade);
		}
		}

		bool successfulExport = true;
		if (cascade)
		{
			// Layers
			for (const Layer& layer : m_Layers)
			{
				successfulExport = successfulExport && layer.Export(Exportfo, exporttype, cascade);
			}
		}


		// Closing
		Exportfo.write(m_ClosingTag.c_str(), m_ClosingTag.size());

		Exportfo.close();
		return successfulExport;
	}

	bool Network::Export(Soren::FileOutput<Output_t>& fo, const ExportType exporttype, const bool cascade) const
	{
		// user will handle closing of the stream

		if (!fo.isValid())
		{
			OL_CORE_ERROR("FileOutput provided is invalid");
			OL_CORE_ERROR("Network not exported");
			return false;
		}

		/*
		// Structure of network.nn
		//
		// Network
		//
		// Name of the network
		// Number of Layers
		// Number of Nodes
		// Number of Links
		// Locked
		//
		// Add when more data is required
		*/
		switch (exporttype)
		{
		case ExportType::Normal:
		{
			/*
			// will not export JSON files
			// will export .nn file with no shrinking
			*/
			//std::filesystem::create_directories(LayerData); -> might use this later

			// Network

			// for string shortening since no string should have 00 in it, this can be and is used as a delim
			char blank = 0;

			size_t nodecount = 0;
			size_t linkcount = 0;
			size_t layercount = size();
			for (const Layer& layer : m_Layers)
			{
				nodecount += layer.size();
				for (const Node& node : layer.m_Nodes)
					linkcount += node.size();
			}

			// Opening
			fo.write(m_OpeningTag.c_str(), m_OpeningTag.size());

			fo.write(m_Title.c_str(), m_Title.size());
			fo.write(&blank, 1);

			fo.write((const Output_t*)&layercount, sizeof(size_t));
			fo.write((const Output_t*)&nodecount, sizeof(size_t));
			fo.write((const Output_t*)&linkcount, sizeof(size_t));
			fo.write((const Output_t*)&m_Locked, sizeof(bool));
			fo.flush();

			break;
		}
		case ExportType::Shrink:
		{
			/*
			// THIS WILL TRY TO SHRINK DATA AS MUCH AS POSSIBLE

			// will not export JSON files
			// will export .snn file with shrinking
			*/

			// Network

			// for string shortening since no string should have 00 in it, this can be and is used as a delim
			char blank = 0;

			size_t nodecount = 0;
			size_t linkcount = 0;
			size_t layercount = size();
			for (const Layer& layer : m_Layers)
			{
				nodecount += layer.size();
				for (const Node& node : layer.m_Nodes)
					linkcount += node.size();
			}

			// Opening
			fo.write(m_OpeningTag.c_str(), m_OpeningTag.size());

			fo.write(m_Title.c_str(), m_Title.size(), true);

			// this is worth for shrinking, but would require more calculations when importing
			// cause we'd have to look for the next delim,
			// see the size of the data being held in the file
			// position the pointer correctly when importing
			// we slit it to shrink export, shrink import -> different file types
			// normal will be a .nn
			// shrink will be a .snn
			// this is for whole files that do not require the different files
			// actually, we do away with .wb files and have a raw one .nn file
			// this will hold the network attributes
			// if we need another, maybe a .config
			// what if there's a 00 in the middle of some data

			fo.write((const Output_t*)&layercount, sizeof(size_t), blank);
			fo.write((const Output_t*)&nodecount, sizeof(size_t), blank);
			fo.write((const Output_t*)&linkcount, sizeof(size_t), blank);
			fo.write((const Output_t*)&m_Locked, sizeof(bool), blank);
			fo.flush();

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
			return Export(ExportType::Normal, cascade);
		}
		}

		bool successfulExport = true;
		if (cascade)
		{
			// Layers
			for (const Layer& layer : m_Layers)
			{
				successfulExport = successfulExport && layer.Export(fo, exporttype, cascade);
			}
		}

		// Closing
		fo.write(m_ClosingTag.c_str(), m_ClosingTag.size());

		fo.flush();
		return successfulExport;
	}

	std::string Network::str(const std::string& networkpref, const std::string& networksuf, const std::string& layerpref, const std::string& layersuf, const std::string& nodepref, const std::string& nodesuf, const std::string& linkpref, const std::string& linksuf) const
	{
		std::stringstream ss;

		ss << networkpref;
		ss << m_Title;
		ss << networksuf;
		ss << std::endl;
		for (const Layer& layer : m_Layers)
		{
			ss << layer.str(layerpref, layersuf, nodepref, nodesuf, linkpref, linksuf);
		}

		return ss.str();
	}

	void to_json(nlohmann::json& j, const Network& network)
	{
		using json = nlohmann::json;
		using ordered_json = nlohmann::ordered_json;

		auto nodescount = [&]() -> size_t
		{
			size_t nodecount = 0;
			for (const Layer& layer : network.NetLayers)
				nodecount += layer.size();
			return nodecount;
		};

		auto linkscount = [&]() -> size_t
		{
			size_t linkcount = 0;
			for (const Layer& layer : network.NetLayers)
				for (const Node& node : layer.LayNodes)
					linkcount += node.size();
			return linkcount;
		};

		auto layerarray = [&]() -> json
		{
			json jarray = json::array();
			for (const Layer& layer : network.NetLayers)
				jarray.emplace_back(layer);
			return jarray;
		};

		j = ordered_json{
					{"Architecture",
						{
							{"Layers", network.size()},
							{"Nodes", nodescount()},
							{"Links", linkscount()}
						}
					},
					{"Title", network.title()},
					{"Locked", network.locked()},
					{"Layers", layerarray()}
				};
	}

	//void from_json(const nlohmann::json& j, Network& network)
	//{
	//}

} // namespace Neural
} // namespace Soren