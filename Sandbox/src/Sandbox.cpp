#include "Owl.h"

//#include "ThreadPool.h"
//#include "Timer.h"
#include <string>
#include <iostream>
#include <sstream>

#include <filesystem>

// This is kinda the beginning of optimization
// Maybe should me implemented in the optimized version
// yeah, this will be an optimized version mod
// have network templates
// NodeData		-> The Data stored by the Node
// BiasData		-> The Bias stored by the Node
// InputData	-> The Data Input from a source, any source, like a file
// OutputData	-> The Data Output to an output, any output, like a file
// VarainceData	-> The Varaince used during squishification, find actual term, backpropagation
// GradientData	-> The Gradient used during squishification, find actual term, backpropagation
// WeightData	-> The Weight held by a link

// during importation, the first import of the type will dictate the type held.
// Import config
// if it is shrunk, the value held might not be the true representation of the value that was in the original network
// like if the first 

class Sandbox : public Owl::Application
{

public:
	Sandbox()
	{

		OL_TRACE("char Data size: {0}", sizeof(char));
		OL_TRACE("int Data size: {0}", sizeof(int));
		OL_TRACE("Network Data size: {0}", sizeof(Owl::Neural::Network));
		OL_TRACE("Layer Data size: {0}", sizeof(Owl::Neural::Layer));
		OL_TRACE("Node Data size: {0}", sizeof(Owl::Neural::Node));
		OL_TRACE("Link Data size: {0}", sizeof(Owl::Neural::Link));
		OL_TRACE("FileInput<char> Data size: {0}", sizeof(Owl::FileInput<char>));
		OL_TRACE("FileOutput<char> Data size: {0}", sizeof(Owl::FileOutput<char>));

	}

	virtual void Init() override	{
		// set up start up properly
		std::filesystem::path PathToNetworkConfig;
		bool Imported;
		//if (PathToNetworkConfig.is_absolute() && std::filesystem::exists(PathToNetworkConfig) && network.Import(Owl::FileInput<Input_t>(PathToNetworkConfig)) ||
		//	PathToNetworkConfig.is_relative() && std::filesystem::exists(PathToNetworkConfig) && network.Import(Owl::FileInput<Input_t>(PathToNetworkConfig)))

		if (!PathToNetworkConfig.empty() && std::filesystem::exists(PathToNetworkConfig))
		{
			Imported = network.Import(Owl::FileInput<Input_t>(PathToNetworkConfig));
		}
		else
		{
			Imported = network.Import();
		}

		if (!Imported)
		{
			// Default start up config

			// Be best to have a network.DefaultConfig();
			
			network.Layers(5);

			// Nodes
			network.NodesDefaultBiasLimits(-10, 10);
			network.LinksDefaultWeightLimits(-10, 10);
			network.NetLayers[0].Nodes(28 * 28);
			network.NetLayers[0].title("Source Layer");

			for (auto& layer : network.NetLayers)
			{
				if (layer.id() > 1)
				{
					layer.Nodes(20);
				}
			}

			// Links

			for (size_t i = 0; i < network.size() - 1; i++)
			{
				network.Links(network.NetLayers[i], network.NetLayers[i+1]);
			}


		}


		// Network
		network.init();
		train.Activation(Owl::Activation::ActivationFunc::Sigmoid);

		//network.InputSourceConfig("train-images.idx3-ubyte", (28 * 28), 60000);
		//network.OutputDataConfig("temp.txt", (28 * 28), 60000);

		//network.Threads(20);

		//OL_TRACE(network.NetLayers[2]);

	}

	virtual void Run() override
	{
		// also develop test and live for continous data.

		network.AddInput("Image Data", "train-images.idx3-ubyte", std::ios_base::in | std::ios_base::binary);
		network.AddInput("Image Labels", "train-labels.idx3-ubyte", std::ios_base::in | std::ios_base::binary);

		// We can have this before run. So that all data regarding the training proccess,
		// or whatever will be happening can all hapen in train
		// get data context
		//int dataMagicalNumber = 0x0803;
		//int dataCount = 0xea60;
		int dataRows = 0x1c;
		int dataColumns = 0x1c;
		// get label context
		//int labelMagicalNumber;
		//int labelCount;

		// Normal setup -> exported from cpp
		// In memory, it's in little endian
		// and the file, is in little endian

		// the data file, it's in big endian
		// then putting it into memory just messes everything up

		// For now till we enable an endian param, or a way to read from a different endian
		//network.FileInputs[0].read(dataMagicalNumber);
		//network.FileInputs[0].read(dataCount);
		//network.FileInputs[0].read(dataRows);
		//network.FileInputs[0].read(dataColumns);

		//network.FileInputs[1].read(labelMagicalNumber);
		//network.FileInputs[1].read(labelCount);

		train.Iterations(1);
		// keep it one for now
		network.FileInputs[0].ignore(4 * 4);
		auto image = new unsigned char[dataRows * dataColumns];

		for (; train; train.NextIteraton()) // while train is still in session, like iterations and stuff
		{
			// kinda fucked that it only chucks formated data
			// Data for current image

			network.FileInputs[0].read((char *)image, dataRows * dataColumns);
			for (int i = 0; i < 28 * 28; ++i)
			{
				std::cout << (int)image[i] << std::endl;
			}
			//std::cout << image;

			// Data from image
			for (int i = 0; i < dataRows; ++i)
			{
				for (int j = 0; j < dataColumns; ++j)
				{
					unsigned char pixel = image[(i * dataColumns) + j];

					// lets just do a normal loop
					Owl::Neural::Node& node = network.NetLayers[0].LayNodes[(i * dataColumns) + j];
					node.value(pixel);

					// logic to place the pixel in the network.
					// we can hopefully simplify it by having the buffer put straight into the layer
				}
			}

			// First layer has data
			// Now to propagate it through the network

			for (int layer = 1; layer < network.size(); ++layer)
			{
				for (Owl::Neural::Node& node : network.NetLayers[layer].LayNodes)
				{
					for (Owl::Neural::Link& link : node.NodeLinks)
					{
						link.m_InBuffer = network.NetLayers[layer - 1].GetNode(link.inID()).value();
					}

					auto linkVals = train.Activation().Resolve(node.Resolve());
					node.value(node.bias() + linkVals);
				}
			}

			//OL_INFO("Sandbox Loop");

			// the user can chose their iterations,
			// in the current case, it can be every image,
			// or after all images
			// it is then all calculated in the end the loss


			// and a loss after every iteration
			// calculate loss and stuff
			// backpropagation will occur after all of the loops
			// have a train value that then loops the iterations
			// might take a lambda tho reset values or something
		}

		delete[] image;
	}

	virtual void Deinit() override
	{
		// log stuff
		// and export

		// Export in case anything was changed.
		// Thought should not export if it is in default state
		OL_INFO("Full Export: {0}", Owl::Timer::Duration([&]
			{
				network.Export();
			}));

		OL_INFO("Shrink Export: {0}", Owl::Timer::Duration([&]
			{
				network.Export(Owl::ExportType::Shrink);
			}));

		OL_INFO("JSON Export: {0}", Owl::Timer::Duration([&]
			{
				network.Export(Owl::ExportType::JSON);
			}));
	}

	~Sandbox()
	{

	}

	Owl::Neural::Network network;
	Owl::Neural::Train train;
};

Owl::Application* Owl::CreateApplication()
{
	return new Sandbox;
}
