#include "olpch.h"
#include "Network/Network.h"

#include "nlohmann/json.hpp"

#include "Input/Input.h"
#include "Output/Output.h"

namespace Soren {
namespace Neural {

	void Network::JSONSync(const std::string& networkname) const
	{
		// Sync will purse .json files then if they were updated after the .nn/.snn file, update .nn/.snn file

		// ANY CHANGES MADE TO LAYER[].json WILL OVERWRITE THE EXISTING IN NETWORK.JSON

		// loop through all json files
		// get their times of last modified
		// if any of them has been modified after respective .nn/.snn file,
		// purse the JSON and update the .nn/.snn file

		// network config will be a .json file that is read right at the start during initialization

		// IF THE .NN/.SNN FILE HAS BEEN MODIFIED AFTER THE JSON FILE, NO UPDATING WILL TAKE PLACE AND JSON WILL HOLD INVALID DATA
		// UNLESS USER UPDATE WAS CORRECT

		std::filesystem::path importpath = m_BaseDir;
		importpath /= "config";

		FileInput<Input_t> SyncIn{};

		std::filesystem::path networkpath = importpath;
		networkpath /= networkname;
		networkpath += ".json";


		nlohmann::json network{};

		// if it exists, we are gonna need it's Architecture
		if (!std::filesystem::exists(networkpath))
		{
			// Network.json is required for proper sync
			OL_CORE_ERROR("{0}.json does not exist at {1}", networkname, networkpath);
			assert(false);
			// if network.json doesn't exit, maybe exit?
			return;
		}
		else
		{
			// if network.json exists, then check if it's valid
			// just for the architecture

			// get network attributes
			std::stringstream temp;
			SyncIn.open(networkpath);
			SyncIn.get(*temp.rdbuf(), '[');

			// close the json
			temp << "[]}";

			// check if the simplified version is valid
			// technically, if it parses, it is valid
			if (network = nlohmann::json::parse(temp.str(), nullptr, false); network.is_discarded())
			{
				// invalid
				OL_CORE_ERROR("{0}.json Architecture is invalid", networkname);
				assert(false);
				return;
			}
			// set the read pointer back to the beginning if the json is valid
			SyncIn.seekg(0);
		}

		// check if Network.json was edited
		// jsons for layers override the edits made by Network.json,
		// this is because it is easier to edit and thus less likely to result in an error
		// changes to the layers jsons will also change the Network.json

		// network data will be held

		// since this function is basically supposed to check if the json files were updated,
		// if they weren't might as well exit

		//if ((std::filesystem::exists(nnpath) && std::filesystem::last_write_time(networkpath) > std::filesystem::last_write_time(nnpath)) ||
			//(std::filesystem::exists(snnpath) && std::filesystem::last_write_time(networkpath) > std::filesystem::last_write_time(snnpath)))

		// means that Network.json was not modified
		// thus we can conclude that it holds valid data and thus 

		// Get values from the network.json
		size_t layercount = 0;
		size_t nodecount = 0;
		size_t linkcount = 0;

		layercount = network["Architecture"]["Layers"];
		nodecount = network["Architecture"]["Nodes"];
		linkcount = network["Architecture"]["Links"];

		// at this point, we've ensured that either/both .snn/.nn exist and Network.json exists
		// and that Network.json was modified after either/both .snn/.nn
		// so we then loop through all layer json and see which was modified after .snn/.nn

		// Read the network.json
		// check and put json in network
		if (network = nlohmann::json::parse((std::ifstream&)SyncIn, nullptr, false);
			network.is_discarded())
		{
			// check if Network.json is valid json
			// we check this if we are syncing
			OL_CORE_ERROR("{0}.json is invalid", networkname);
			assert(false);
		}
		SyncIn.close();

		std::filesystem::path layerbase = importpath;
		layerbase /= "Layers";

		// create layers then use their threads for importing
		// go through all the layers and compare them and the content in network.json
		// if the layer[].json does not exist, then no modifications will occur in network.json regarding that layer
		for (size_t i = 0;
			i < layercount;
			++i)
		{
			std::filesystem::path layerpath = layerbase;
			layerpath /= "Layer";
			layerpath += "[";
			layerpath += std::to_string(i);
			layerpath += "]";
			layerpath += ".json";

			// check if the layer's json exists in case the user put a size greater than the current one
			if (!std::filesystem::exists(layerpath))
			{
				OL_CORE_WARN("Layer[{0}].json does not exist", i);
				OL_CORE_WARN("Layer will be created if with any values found in {0}.json", networkname);
				OL_CORE_WARN("Otherwise will be empty");
				continue;
			}

			// if the layer[].json exists, open and put it in a json object that then is used to compare with network
			SyncIn.open(layerpath);
			// check if json is valid
			// if not, no modifications occur
			nlohmann::json layer;
			if (layer = nlohmann::json::parse((std::ifstream&)SyncIn, nullptr, false); layer.is_discarded())
			{
				OL_CORE_WARN("Layer[{0}].json is invalid", i);
				OL_CORE_WARN("Layer will be created if with any values found in {0}.json", networkname);
				OL_CORE_WARN("Otherwise will be empty");
				continue;
			}

			// layer is valid json

			if (network["Layers"][i] == layer)
			{
				// if the layer data in the network is the same as the one in layer
				// no modifications required
				continue;
			}

			// layer data is different from the data in network
			// update network
			network["Layers"][i] = layer;
			SyncIn.close();
		}

		// check if the updated network is the same as network.json
		// if it is, no need to update the json file
		SyncIn.open(networkpath);
		if (nlohmann::json networktemp = nlohmann::json::parse((std::ifstream&)SyncIn, nullptr, false); networktemp == network) return;
		SyncIn.close();

		// update the network.json
		FileOutput<Output_t> SyncOut;
		SyncOut << std::setw(4) << network;

		// AT THIS POINT NETWORK.JSON IS UPDATED AND HAS ALL CHANGES FROM LAYER[].JSON

		std::filesystem::path snnpath = importpath;
		snnpath /= networkname;
		snnpath += ".snn";
		std::filesystem::path nnpath = importpath;
		nnpath /= networkname;
		nnpath += ".nn";

		bool writenew = false;
		// check if the write time for .nn/.snn is before the json files
		// the user could export to both types so, check if either exists

		if (!std::filesystem::exists(nnpath) && !std::filesystem::exists(snnpath))
		{
			// if none of the export files exist
			// warn and create one from the json
			OL_CORE_WARN("No {0}.nn or {0}.snn in {1}", networkname, importpath);
			// have a preference for what type to create
			// have .nn be the default type thus will create a .nn
			OL_CORE_WARN("{0}.nn will be created from {0}.json", networkname);
			writenew = true;
		}

		// convert the json into an exportable format
		if (writenew)
		{

		}
		else
		{

		}
	}

} // namespace Neural
} // namespace Soren

