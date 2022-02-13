#include "olpch.h"
#include "Network/Network.h"
#include "Layer/Layer.h"

#include "nlohmann/json.hpp"

#include "Input/Input.h"

#include <limits>

// caused warning C4003: not enough arguments for function-like macro invocation 'min'
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// if were planning to have multi threading, we'll have to have either delimiting characters, or we use atomic
// technically that would still slow down importation, cause they have to wait for one layer to finish
// then use the same filestream for the next

namespace Soren {
namespace Neural {

	void Network::AddInput(Soren::FileInput<Input_t>& fi)
	{
		m_FileInputs.push_back(std::move(fi));
	}

	void Network::AddInput(const std::string& name, const std::filesystem::path& filepath, std::ios::openmode mode)
	{
		m_FileInputs.emplace_back(name, filepath, mode);
	}

	void Network::DeleteInput(const std::string& name)
	{
		auto pos = std::find(m_FileInputs.begin(), m_FileInputs.end(), name);
		if (pos != m_FileInputs.end())
		{
			m_FileInputs.erase(pos);
		}

		OL_CORE_ERROR("Input with Name: {0} not found", name);
		assert(false);
	}

	// Network Importation of both json and raw
	// what about support for network names that then are shown in the .nn/.snn
	bool Network::Import(const ImportType importtype, const bool override, const bool cascade)
	{
		std::filesystem::path importpath = m_BaseDir;
		importpath += "config";

		switch (importtype)
		{
		case ImportType::None:
		default:
		case ImportType::Normal:
		{
			importpath /= "Network.nn";
			break;
		}
		case ImportType::Shrink:
		{
			importpath /= "Network.snn";
			break;
		}
		case ImportType::JSON:
		{
			importpath /= "Network.snn";
			break;
		}
		}

		FileInput<Input_t> import(importpath, std::ios_base::binary | std::ios_base::in);
		return Import(import, importtype, override, cascade);
		// create stream to default location for network data
	}

	bool Network::Import(FileInput<Input_t> fi, const ImportType importtype, const bool override, const bool cascade)
	{
		// Import will take directly from the raw files
		// If override is false, then nothing is changed

		if (!fi.isValid())
		{
			OL_CORE_ERROR("FileInput provided is invalid");
			OL_CORE_ERROR("Network not imported");
			return false;
		}

		if (!SeekBegin(fi)) {
			OL_CORE_ERROR("Unable to find beginning of Network Data");
			OL_CORE_ERROR("Network not imported");
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
		//		-> all layers in network
		//
		// Add when more data is required
		*/

		char blank = 0;

		// temp so that in case of error, the original data isn't overwritten
		std::string title;
		size_t layercount = 0;
		size_t nodecount = 0;
		size_t linkcount = 0;
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

			fi.read(layercount);
			fi.read(nodecount);
			fi.read(linkcount);
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

			auto width = [&] {
				char temp;
				fi.get(temp);
				return temp;
			};

			char length = width();
			title.resize(length);
			fi.read(title.data(), length);

			fi.read(layercount, true);
			fi.read(nodecount, true);
			fi.read(linkcount, true);
			fi.read(locked, true);

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

		// Layers

		bool successfulExport = true;
		if (cascade && override) {
			std::vector<Layer> layers;
			layers.reserve(layercount);
			for (size_t i = 0; i < layercount; ++i) {
				layers.emplace_back(*this);
			}

			m_Layers = std::move(layers);

			for (Layer& layer : m_Layers) {
				successfulExport = successfulExport && layer.Import(fi, importtype, override, cascade);
			}
			// then import for each layer
		}
		return successfulExport;
	}

} // namespace Neural
} // namespace Soren