#pragma warning( disable : 4244 ) // Disable Warning C4244: conversion from * to *, possible loss of data

#define MATTHREADS 8

#include "Soren.h"
//#include "oneapi/dnnl/dnnl.h"
//#include "oneapi/dnnl/dnnl.hpp"
//#include "oneapi/dnnl/dnnl_debug.h"
//#include "oneapi/dnnl/dnnl_config.h"

//#include "ThreadPool.h"
//#include "Timer.h"
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <winbase.h>

#include <filesystem>
//#include "dnnUtils.h"

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

#define N 2048
//#define SOREN_TEST

#define BLOCK 8
#define BLOCK_Y 4
#define BLOCK_X 2

template<typename TPromiseType = void>
struct suspend_never {
	TPromiseType* mPromise{ nullptr };
	bool await_ready() const noexcept { return false; }

	bool await_suspend(std::coroutine_handle<TPromiseType> h) noexcept {
		mPromise = &h.promise();
		return false;
	}

	TPromiseType* await_resume() const noexcept { return mPromise; }
};

template<typename TPromiseType = void> 
struct suspend_always {
	TPromiseType* mPromise{ nullptr };
	bool await_ready() const noexcept { return false; }

	void await_suspend(std::coroutine_handle<TPromiseType> h) noexcept {
		mPromise = &h.promise();
		// For input, get the next input entry
	}

	TPromiseType* await_resume() const noexcept { return mPromise; }
};

template <typename TVal>
class GeneratorObject {
public:

	struct promise_type {
		TVal mVal;
		promise_type() = default;
		GeneratorObject get_return_object() { return { std::coroutine_handle<promise_type>::from_promise(*this) }; }
		std::suspend_always initial_suspend() { return { }; }
		std::suspend_always yield_value(TVal value) {
			mVal = value;
			return {};
		}
		void return_void(TVal value) noexcept { mVal = value; }
		void return_void() noexcept {}
		std::suspend_never final_suspend() noexcept { return { }; }
		void unhandled_exception() {}
	};

	GeneratorObject(std::coroutine_handle<promise_type> handler) : mHandler{ handler } { }

	operator std::coroutine_handle<promise_type>() const { return mHandler; }
private:
	std::coroutine_handle<promise_type> mHandler;
};

template<typename TData>
class InputGenerator {
public:

	struct promise_type {
		Eigen::MatrixX<TData> mImage{};
		Eigen::VectorX<TData> mLabel{};
		std::vector<TData> mImageBuffer{};
		TData mLabelBuffer{};
		//std::vector<TData> mImage;
		//TData mLabel;
		promise_type() = default;
		InputGenerator get_return_object() { return { std::coroutine_handle<promise_type>::from_promise(*this) }; }
		suspend_always<promise_type> initial_suspend() { return { }; }
		void return_void() noexcept {}
		std::suspend_always final_suspend() noexcept { return { }; }
		void unhandled_exception() {}
	};

	InputGenerator(std::coroutine_handle<promise_type> handler) : mHandler{ handler } { }

	operator std::coroutine_handle<promise_type>() const { return mHandler; }
private:
	std::coroutine_handle<promise_type> mHandler;
};

template<>
struct suspend_always<InputGenerator<unsigned char>::promise_type> {
	InputGenerator<unsigned char>::promise_type* mPromise{ nullptr };
	bool await_ready() const noexcept { return false; }

	void await_suspend(std::coroutine_handle<InputGenerator<unsigned char>::promise_type> h) noexcept {
		mPromise = &h.promise();
		SOREN_INFO("Specialization");
		//mPromise->mImage = std::vector<unsigned char>(imageCount);

		// For input, get the next input entry
	}

	InputGenerator<unsigned char>::promise_type* await_resume() const noexcept { return mPromise; }
};


class Sandbox : public Soren::Application
{

public:
	Sandbox()
	{

		SOREN_TRACE("char Data size: {0}", sizeof(char));
		SOREN_TRACE("int Data size: {0}", sizeof(int));
		SOREN_TRACE("float Data size: {0}", sizeof(float));
		SOREN_TRACE("Network Data size: {0}", sizeof(Soren::Neural::Network<float>));
		SOREN_TRACE("Layer Data size: {0}", sizeof(Soren::Neural::Layer<float>));
		SOREN_TRACE("Node Data size: {0}", sizeof(Soren::Neural::Node<double,float>));
		SOREN_TRACE("Link Data size: {0}", sizeof(Soren::Neural::Link<double>));
		SOREN_TRACE("FileInput<char> Data size: {0}", sizeof(Soren::FileInput<char>));
		SOREN_TRACE("FileOutput<char> Data size: {0}", sizeof(Soren::FileOutput<char>));

	}

	InputGenerator<unsigned char> inputGenerator(size_t imageCount, size_t labelCount, size_t size, size_t bacthSize = 1) {
		auto promise = co_await suspend_never<InputGenerator<unsigned char>::promise_type>{};
		promise->mImage;
		//promise->mImage = std::vector<unsigned char>(imageCount);
		for (size_t iSize = 0; iSize < size; iSize++) {
			//trainData.read(promise->mImage.col(1).data(), imageCount);
			//trainLabels.read(&(promise->mLabel), labelCount);
			if (iSize == size - 1) {
				co_return;
			}
			co_await std::suspend_always{};
		}
	}

	virtual void Init() override {
		//Soren::Neural::Network network;

		Soren::Neural::Layer<float>::LinkGenerator linkGenerator = [](size_t index,const Soren::Neural::Layer<float>& input)-> Eigen::Vector<float, Eigen::Dynamic> {
			index;
			return Eigen::Vector<float, Eigen::Dynamic>::Random(input.size());
			//return Eigen::Vector<float, Eigen::Dynamic>(input.size()).unaryExpr([](float val) -> float { val; return Soren::Random::Float(-1, 1); });
		};

#ifdef SOREN_DEBUG
#ifdef SOREN_TEST
		auto testNet = std::make_shared<Soren::Neural::Network<float>>();
		testNet->addLayer({ 3 });
		testNet->addLayer({ 3, sigmoidActivation }, linkGenerator);
		testNet->addLayer({ 2, sigmoidActivation }, linkGenerator);

		Soren::Neural::Model<float> testModel(testNet);

		//for (size_t i = 0; i < 10; i++) {
			testModel.train([&](size_t index) -> Soren::Neural::Model<float>::DataPair<> {
				if (index == 0) {
					return { Eigen::Matrix<float, 3, 1>{3,2,1}, Eigen::Matrix<float, 2, 1>{1,0} };
				}if (index == 1) {
					return { Eigen::Matrix<float, 3, 1>{1,3,2}, Eigen::Matrix<float, 2, 1>{1,0} };
				}if (index == 2) {
					return { Eigen::Matrix<float, 3, 1>{1,2,3}, Eigen::Matrix<float, 2, 1>{0,1} };
				}
				return {};
				},1,10,1);

			//SOREN_INFO(label);
			SOREN_INFO("Network Output");
			SOREN_INFO(testNet->output());
		//}
#endif // SOREN_TEST
#endif // SOREN_DEBUG

		network->addLayer({ 28 * 28 });
		network->addLayer({ 200, sigmoidActivation }, linkGenerator);
		network->addLayer({ 100, sigmoidActivation }, linkGenerator);
		network->addLayer({ 80, sigmoidActivation }, linkGenerator);
		// We can use ReLU only if we make the values really small
		// Not too small, it will not descend fast enough
		// When working with ReLU, I think having small weights is a good way to start
		// For ReLU, the appropriate loss function is Mean Squared Error
		// but that won't work since the output is one-hot and not linear/unbounded
		// ReLU might just need a whole different structure all together
		network->addLayer({ 10, sigmoidActivation, softmaxActivation }, linkGenerator);
		
		//SOREN_INFO(network->resolve().layers().at(2)->values());

		trainData.ignore(4 * 4);
		testData.ignore(4 * 4);
		trainLabels.ignore(4 * 2);
		testLabels.ignore(4 * 2);

	}

	virtual void Run() override {
		size_t rows = 0x1c;
		size_t columns = 0x1c;
		size_t trainCount = 60000;
		size_t testCount = 10000;
		size_t miniBatchSize = 1<<3;
		//unsigned char label{};
		//double cost{};

		//std::vector<unsigned char> image(sizeof(unsigned char) * rows* columns);
		std::vector<unsigned char> labels(miniBatchSize);
		Eigen::MatrixX<unsigned char> images(sizeof(unsigned char) * rows* columns, miniBatchSize);
		Eigen::MatrixX<float> labelOneHot(10, miniBatchSize);
		//Eigen::MatrixX<float> imageMat(1, rows * columns);

		std::coroutine_handle<InputGenerator<unsigned char>::promise_type> inputGen = inputGenerator(sizeof(unsigned char) * rows * columns, sizeof(unsigned char), trainCount);
		InputGenerator<unsigned char>::promise_type* inputPromise = &inputGen.promise();
		inputPromise;
		//inputGen.resume();

		Soren::Timer::Stopwatch stopwatch;


		Soren::Neural::BatchResult trainResult = model->train([&](size_t index) -> Soren::Neural::Model<float>::DataPair<> {
			if (index == 0) {
				trainData.seekg(4 * 4);
				trainLabels.seekg(4 * 2);

				//trainData.read(images.data(), sizeof(unsigned char) * rows * columns * miniBatchSize);
				//trainLabels.read(labels.data(), sizeof(unsigned char) * miniBatchSize);
			}

			trainData.read(images.data(), sizeof(unsigned char) * rows * columns * miniBatchSize);
			trainLabels.read(labels.data(), sizeof(unsigned char) * miniBatchSize);
			labelOneHot.fill(0);
			for (size_t iLabels = 0; iLabels < miniBatchSize; iLabels++) {
				labelOneHot(labels[iLabels], iLabels) = 1;
			}
			return { images.cast<float>(), labelOneHot };
			},
			trainCount / miniBatchSize,
			miniBatchSize,
			5,
			//1);
			10000 / miniBatchSize);


		//for (size_t i = 0; i < 5; i++) {
		//	model->trainReset();

		//	stopwatch.Lap();


		//	auto trainDuration = stopwatch.Lap();

		//	trainData.seekg(4 * 4);
		//	trainLabels.seekg(4 * 2);

		//	//network->input(std::vector<float>(image.begin(), image.end()));
		//	//network->resolve();

		//	SOREN_INFO("Train[{1}] Duration: {0}ms", trainDuration.milliseconds(), i);
		//	//SOREN_INFO(weights == network->layers().at(4)->weights());
		//	//SOREN_INFO(weights - network->layers().at(4)->weights());
		//	SOREN_INFO(label);
		//	SOREN_INFO(network->output());
		//}
		size_t testMiniBatchSize = 1;

		std::vector<unsigned char> testLabelsVec(testMiniBatchSize);
		Eigen::MatrixX<unsigned char> testImages(sizeof(unsigned char) * rows * columns, testMiniBatchSize);
		Eigen::MatrixX<float> testLabelOneHot(10, testMiniBatchSize);

		stopwatch.Lap();
		Soren::Neural::BatchResult testResult = model->test([&](size_t index) -> Soren::Neural::Model<float>::DataPair<> {
			index;
			testData.read(testImages.data(), sizeof(unsigned char) * rows * columns * testMiniBatchSize);
			testLabels.read(testLabelsVec.data(), sizeof(unsigned char) * testMiniBatchSize);
			testLabelOneHot.fill(0);
			for (size_t iLabels = 0; iLabels < testMiniBatchSize; iLabels++) {
				testLabelOneHot(testLabelsVec[iLabels], iLabels) = 1;
			}
			return { testImages.cast<float>(), testLabelOneHot};
		}, testCount / testMiniBatchSize);

		auto testDuration = stopwatch.Lap();

		SOREN_INFO("Test Duration: {0}ms", testDuration.milliseconds());
		//SOREN_INFO(label);
		//SOREN_INFO(network->output());
	
	}

	virtual void Deinit() override {

		
	}

	~Sandbox()
	{

	}

	
	Soren::FileInput<unsigned char> trainData{ "train-images.idx3-ubyte", std::ios_base::in | std::ios_base::binary };
	Soren::FileInput<unsigned char> trainLabels{ "train-labels.idx1-ubyte", std::ios_base::in | std::ios_base::binary };

	Soren::FileInput<unsigned char> testData{ "t10k-images.idx3-ubyte", std::ios_base::in | std::ios_base::binary };
	Soren::FileInput<unsigned char> testLabels{ "t10k-labels.idx1-ubyte", std::ios_base::in | std::ios_base::binary };

	std::shared_ptr<Soren::Activation::Vector::ActivatorBase<float>> softmaxActivation = std::make_shared<
		Soren::Activation::Vector::Activator<float,float>>(Soren::Activation::Vector::SoftMax<float>);

	std::shared_ptr<Soren::Activation::ActivatorBase<float>> sigmoidActivation = std::make_shared<
		Soren::Activation::Activator<float,float>>(Soren::Activation::Sigmoid<float>);
	std::shared_ptr<Soren::Activation::ActivatorBase<float>> reLUActivation = std::make_shared<
		Soren::Activation::Activator<float,float>>(Soren::Activation::ReLU<float>);
	std::shared_ptr<Soren::Activation::ActivatorBase<float>> tanHActivation = std::make_shared<
		Soren::Activation::Activator<float,float>>(Soren::Activation::TanH<float>);

	std::shared_ptr<Soren::Loss::Vector::LossBase<float>> categoricalCrossentropyLoss{ std::make_shared<Soren::Loss::Loss<float>>(Soren::Loss::CategoricalCrossentropy<float>) };

	std::shared_ptr<Soren::Neural::Network<float>> network{ std::make_shared<Soren::Neural::Network<float>>(Soren::Neural::Network<float>()) };

	//std::shared_ptr<Soren::Loss::Loss<float, size_t>> sparseCategoricalLoss{ std::make_shared<Soren::Loss::Loss<float, size_t>>(Soren::Loss::Loss<float, size_t>::SparseCategoricalCrossentropy()) };
	std::shared_ptr<Soren::Neural::Model<float>> model{ std::make_shared<Soren::Neural::Model<float>>(network, categoricalCrossentropyLoss) };
	
	/* Async */

	//std::shared_ptr<Soren::Neural::Async::Network<float>> networkAsync{std::make_shared<Soren::Neural::Async::Network<float>>(Soren::Neural::Async::Network<float>())};

	//std::shared_ptr<Soren::Neural::Async::Train<float, unsigned char>> trainAsync{ std::make_shared<Soren::Neural::Async::Train<float, unsigned char>>(networkAsync) };
};

Soren::Application* Soren::CreateApplication()
{
	return new Sandbox;
}
