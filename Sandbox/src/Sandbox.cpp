#pragma warning( disable : 4244 ) // Disable Warning C4244: conversion from * to *, possible loss of data
#include "Soren.h"

//#include "ThreadPool.h"
//#include "Timer.h"
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

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

class Sandbox : public Soren::Application
{

public:
	Sandbox()
	{

		SOREN_TRACE("char Data size: {0}", sizeof(char));
		SOREN_TRACE("int Data size: {0}", sizeof(int));
		SOREN_TRACE("Network Data size: {0}", sizeof(Soren::Neural::Network<float>));
		SOREN_TRACE("Layer Data size: {0}", sizeof(Soren::Neural::Layer<float>));
		SOREN_TRACE("Node Data size: {0}", sizeof(Soren::Neural::Node<double,float>));
		SOREN_TRACE("Link Data size: {0}", sizeof(Soren::Neural::Link<double>));
		SOREN_TRACE("FileInput<char> Data size: {0}", sizeof(Soren::FileInput<char>));
		SOREN_TRACE("FileOutput<char> Data size: {0}", sizeof(Soren::FileOutput<char>));

	}

	virtual void Init() override {
		//Soren::Neural::Node<double>::GlobalSubscribe<Soren::NodeCreatedEvent>([](Soren::Event& event) {
		//	SOREN_TRACE("Global Subscribe A");
		//	SOREN_TRACE(event.ToString());
		//	return true;
		//	});
		Soren::Neural::Node<double>::GlobalSubscribe<Soren::NodeCopiedEvent>([](Soren::Event& event) {
			//SOREN_TRACE("Global Subscribe B");
			SOREN_TRACE(event.ToString());
			return true;
			});
		Soren::Neural::Node<double>::GlobalSubscribe<Soren::LayerCopiedEvent>([](Soren::Event& event) {
			//SOREN_TRACE("Global Subscribe B");
			SOREN_TRACE(event.ToString());
			return true;
			});
		Soren::Neural::Node<double>::GlobalSubscribe<Soren::LayerMovedEvent>([](Soren::Event& event) {
			//SOREN_TRACE("Global Subscribe B");
			SOREN_TRACE(event.ToString());
			return true;
			});
		Soren::Neural::Node<double>::GlobalSubscribe<Soren::NodeCopyAssignedEvent>([](Soren::Event& event) {
			//SOREN_TRACE("Global Subscribe C");
			SOREN_TRACE(event.ToString());
			return true;
			});

		// MNIST
		// Input and Normalization
		// i.e. to the basis 255
		network.addLayer({ 28 * 28, [](std::vector<float> values)->std::vector<float> {
			std::vector<float> activatedValues;
			activatedValues.reserve(values.size());
			for (auto value : values) {
				activatedValues.push_back(value / 255);
			}
			return activatedValues; },  [](float value) -> float { return value / 255; } }, ([]() -> float {return Soren::Random::Float(0, 1); }));
		// 28 x 28 image
		//network.addLayer({ 28 * 28, Soren::Activation::Identity<float> }, ([]() -> float {return Soren::Random::Float(0, 10); }));
		// Convolution Layer
		network.addLayer({ 28 * 28, Soren::Activation::ReLU<float> }, Soren::Linking::SquareConvolution<float>(1, 28, 28, 28*28), ([]() -> float {return Soren::Random::Float(0, 1); }));
		// SoftMax Layer
		network.addLayer({ 10, Soren::Activation::SoftMax<float>, Soren::Activation::Identity<float> });
	}

	virtual void Run() override {

		size_t dataRows = 0x1c;
		size_t dataColumns = 0x1c;

		trainData.ignore(4 * 4);
		trainLabels.ignore(4 * 2);
		std::vector<unsigned char> image(dataRows * dataColumns);
		unsigned char label;

		trainData.read(image.data(), dataRows * dataColumns);
		trainLabels.read(&label, 1);

		network.input(image);

		SOREN_TRACE(Soren::Timer::Duration([&]() {network.resolve(); }));

		//SOREN_TRACE("Layer 1");
		//SOREN_TRACE(network.layers().at(1)->outputMatrix());
		//SOREN_TRACE("Layer 2");
		//SOREN_TRACE(network.layers().at(2)->outputMatrix());
		SOREN_TRACE("Layer 3");
		SOREN_TRACE(network.layers().at(3)->outputMatrix());

	}
	
	virtual void Deinit() override {
		
	}

	~Sandbox()
	{

	}

	Soren::FileInput<unsigned char> trainData{ "train-images.idx3-ubyte", std::ios_base::in | std::ios_base::binary };
	Soren::FileInput<unsigned char> trainLabels{ "train-labels.idx3-ubyte", std::ios_base::in | std::ios_base::binary };

	Soren::FileInput<unsigned char> testData{ "t10k-images.idx3-ubyte", std::ios_base::in | std::ios_base::binary };
	Soren::FileInput<unsigned char> testLabels{ "t10k-labels.idx3-ubyte", std::ios_base::in | std::ios_base::binary };

	Soren::Neural::Network<float> network{};

};

Soren::Application* Soren::CreateApplication()
{
	return new Sandbox;
}
