#pragma once
#include "Core.h"
#include "Base.h"

#include "Eigen/Core"

#include "Status/Status.h"

#include "Log/Log.h"
#include "Events/Event.h"

#include "LayerAsync.h"

#include "nlohmann/json.hpp"


namespace Soren {

	template <typename T>
	class FileOutput;
	template <typename T>
	class FileInput;

	namespace Neural {


		class Event;

		struct Linkable;
		template<typename TWeight>
		class Linker;

		template<typename TValue, typename TBias, typename TWeight>
		class Node;

		class BaseLayer {
			//template<typename TValue, typename TBias, typename TWeight>
			//friend class Layer;
		public:
			BaseLayer():mId{sId++} {};
			~BaseLayer() {};

			BaseLayer& pipeMatrix(BaseLayer& layer);

			BaseLayer& pipe(BaseLayer& layer);
			template<typename TValue>
			std::vector<TValue> output();
			template<typename TInputValue>
			BaseLayer& input(std::vector<TInputValue>& value);
			template<typename TInputValue>
			BaseLayer& input(std::vector<TInputValue>&& value);

			//template<typename TWeight>
			void link(BaseLayer& layer);

			inline size_t id() const { return mId; }

			// Special member functions
			#pragma region
			BaseLayer(const BaseLayer&)
				: mId(sId++) {};
			BaseLayer(BaseLayer&& arg) noexcept
				: mId(std::exchange(arg.mId, 0)) {};
			BaseLayer& operator=(const BaseLayer&) {
				//mId = sId++;
				return *this;
			};
			BaseLayer& operator=(BaseLayer&& arg) noexcept {
				mId = std::move(arg.mId);
				return *this;
			};
			#pragma endregion

		protected:

			inline static size_t sId = 1;
			size_t mId;
		};

		template<typename TValue = double, typename TBias = TValue, typename TWeight = TValue>
		class SOREN_API Layer : public EventEmmiter, public BaseLayer // Should be LayerEvent
		{

			template<typename TValue, typename TBias, typename TWeight>
			friend class Layer;
			template<typename TValue, typename TBias, typename TWeight>
			friend class Network;

		public:

			using LinkGenerator = std::function<Eigen::Vector<TWeight, Eigen::Dynamic>(size_t index, const Layer<TValue, TBias, TWeight>& inputLayer)>;
			using BiasGenerator = std::function<TBias(size_t index)>;
			using LayerActivation = std::function<Eigen::Vector<TValue, Eigen::Dynamic>(Eigen::Vector<TWeight, Eigen::Dynamic>)>;


			Layer(size_t size = 0)
			{
				Timer::Stopwatch stopwatch;
				// //SOREN_CORE_WARN("No Default Link Weight Limits Set");
				mActivated = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
				mWeighted = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
				mWeights = Eigen::Matrix<TWeight, Eigen::Dynamic, Eigen::Dynamic>::Identity(size, size);
				mInputs = Eigen::MatrixX<TValue>::Zero(size, 1);
				mBiases = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
				//Bind();

				//SOREN_CORE_TRACE("ID : {0} -> {1}", mId, __FUNCTION__);
				//SOREN_CORE_TRACE(stopwatch.Stop());

				LayerCreatedEvent event{};
				Emit(event);
			};
			Layer(size_t size, std::shared_ptr<Activation::ActivatorBase<TValue>> activation)
				: mActivation(activation)
			{
				Timer::Stopwatch stopwatch;

				mActivated = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
				mWeighted = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
				mWeights = Eigen::Matrix<TWeight, Eigen::Dynamic, Eigen::Dynamic>::Identity(size, size);
				mInputs = Eigen::MatrixX<TValue>::Zero(size, 1);
				mBiases = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
				
				// //SOREN_CORE_WARN("No Default Link Weight Limits Set");
				//Bind();

				//SOREN_CORE_TRACE("ID : {0} -> {1}", mId, __FUNCTION__);
				//SOREN_CORE_TRACE(stopwatch.Stop());

				LayerCreatedEvent event{};
				Emit(event);
			};
			//Layer(size_t size, std::shared_ptr<Activation::Activator<TValue>> layerActivation)
			//	:  mLayerActivation(layerActivation)
			//{
			//	Timer::Stopwatch stopwatch;

			//	mWeighted = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
			//	mWeights = Eigen::Matrix<TWeight, Eigen::Dynamic, Eigen::Dynamic>::Identity(size, size);
			//	mInputs = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
			//	mBiases = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);

			//	// //SOREN_CORE_WARN("No Default Link Weight Limits Set");
			//	//mInputs.reserve(mNodes.size());
			//	//Bind();

			//	//SOREN_CORE_TRACE("ID : {0} -> {1}", mId, __FUNCTION__);
			//	//SOREN_CORE_TRACE(stopwatch.Stop());

			//	LayerCreatedEvent event{};
			//	Emit(event);
			//};
			Layer(size_t size, std::shared_ptr<Activation::ActivatorBase<TValue>> activation, std::shared_ptr<Activation::Vector::ActivatorBase<TValue>> layerActivation)
				: mActivation(activation), mLayerActivation(layerActivation)
			{
				Timer::Stopwatch stopwatch;
				// //SOREN_CORE_WARN("No Default Link Weight Limits Set");
				mActivated = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
				mWeighted = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
				mWeights = Eigen::Matrix<TWeight, Eigen::Dynamic, Eigen::Dynamic>::Identity(size, size);
				mInputs = Eigen::MatrixX<TValue>::Zero(size, 1);
				mBiases = Eigen::Vector<TValue, Eigen::Dynamic>::Zero(size);
				//mInputs.reserve(mNodes.size());
				//Bind();

				//SOREN_CORE_TRACE("ID : {0} -> {1}", mId, __FUNCTION__);
				//SOREN_CORE_TRACE(stopwatch.Stop());

				LayerCreatedEvent event{};
				Emit(event);
			};
			~Layer() {
				LayerDeletedEvent event{};
				Emit(event);
			};

			// Links
			inline const Eigen::Vector<TValue, Eigen::Dynamic>& values() const { return mWeighted; }
			inline const Eigen::MatrixX<TWeight>& weights() const { return mWeights; }
			inline Eigen::MatrixX<TWeight>& weights() { return mWeights; }
			inline const Eigen::MatrixX<TWeight>& biases() const { return mBiases; }
			inline Eigen::VectorX<TBias>& biases() { return mBiases; }
			inline const Eigen::MatrixX<TValue>& layerActivations() const { return mLayerActivations; }
			inline Eigen::MatrixX<TValue>& layerActivations() { return mLayerActivations; }
			inline const Eigen::MatrixX<TValue>& activated() const { return mActivated; }
			inline Eigen::MatrixX<TValue>& activated() { return mActivated; }
			inline const Eigen::MatrixX<TValue>& weighted() const { return mWeighted; }
			inline Eigen::MatrixX<TValue>& weighted() { return mWeighted; }
			inline const Eigen::MatrixX<TValue>& inputs() const { return mInputs; }
			inline Eigen::MatrixX<TValue>& inputs() { return mInputs; }
			inline const std::shared_ptr<Activation::Vector::ActivatorBase<TValue>>& layerActivation() const { return mLayerActivation; }
			inline std::shared_ptr<Activation::Vector::ActivatorBase<TValue>>& layerActivation() { return mLayerActivation; }
			inline const std::shared_ptr<Activation::ActivatorBase<TValue>>& activation() const { return mActivation; }
			inline std::shared_ptr<Activation::ActivatorBase<TValue>>& activation() { return mActivation; }
			inline size_t size() const { return mWeighted.rows(); }
			inline bool running() const { return mRunning; }
			inline bool locked() const { return mLocked; }

			void clear() {
				if (mLocked) return;

				LayerClearedEvent event{};
				Emit(event);

				// Reset Activation function if stored by Link
			};

			void halt() {

				//const running = m_Running;
				mRunning = FALSE;

				LayerDisabledEvent event{};
				Emit(event);

			};
			void resume() {
				mRunning = TRUE;

				LayerEnabledEvent event{};
				Emit(event);
			};

			void reset() {
				LayerResetEvent event();
				Emit(event);
			};

			inline void value(TValue value) { if (mLocked) return; }
			inline void bias(TBias bias) { if (mLocked) return; }
			inline void lock(bool lock) { mLocked = lock; }
			inline void lock() { mLocked = true; }
			inline void unlock() { mLocked = false; }

			inline void setLinkGenerator(LinkGenerator linkGenerator) { if (mLocked) return; mLinkGenerator = linkGenerator; }
			inline void setBiasGenerator(std::function<TBias(void)> biasGenerator) { if (mLocked) return; mBiasGenerator = biasGenerator; }

			template<typename TLinkValue = TValue , typename TLinkBias = TBias,typename TLinkWeight=TWeight>
			inline void link(Layer<TLinkValue, TLinkBias, TLinkWeight>& layer) {
				layer.generateLinks(*this);
			}
			
			template<typename TLinkValue = TValue , typename TLinkBias = TBias,typename TLinkWeight=TWeight>
			inline void link(Layer<TLinkValue, TLinkBias, TLinkWeight>& layer, LinkGenerator linkGenerator) {
				layer.generateLinks(*this, linkGenerator);
			}

			// Basic
			// Receive non-weighted input
			// basically output() from previous layer
			#pragma region
			
			template<typename TInputValue = TValue,typename TInputBias = TBias,typename TInputWeight = TWeight>
			inline Layer& input(Layer<TInputValue,TBias, TWeight>& layer) {
				//Timer::Stopwatch stopwatch;
				input(layer.output());
				//SOREN_CORE_TRACE("ID : {0} -> {1}", mId, __FUNCTION__);
				//SOREN_CORE_TRACE(stopwatch.Stop());
				return (*this);
			}
			inline Layer& input(std::vector<TValue>& input) {
				Timer::Stopwatch stopwatch;
				/*if (input.size() != (size_t)mWeighted.rows()) {
					throw std::invalid_argument("The array length provided is not equal to the number of nodes");
				}*/

				//Async::Layer::InputVector
				//oneapi::tbb::simple_partitioner partitioner;
				//std::mutex lock;

				mInputs.noalias() = Eigen::Map<Eigen::Vector<TValue, Eigen::Dynamic>, Eigen::Unaligned>(input.data(), input.size());

				//SOREN_CORE_TRACE("ID : {0} -> {1}", mId, __FUNCTION__);
				//SOREN_CORE_TRACE(stopwatch.Stop());

				return (*this);
			}
			inline Layer& input(const std::vector<TValue>&& input) {
				//Timer::Stopwatch stopwatch;
				/*if (input.size() != (size_t)mWeighted.rows()) {
					throw std::invalid_argument("The array length provided is not equal to the number of nodes");
				}*/

				mInputs.noalias() = Eigen::Map<Eigen::Vector<TValue, Eigen::Dynamic>, Eigen::Unaligned>(input.data(), input.size());

				//SOREN_CORE_TRACE("ID : {0} -> {1}", mId, __FUNCTION__);
				//SOREN_CORE_TRACE(stopwatch.Stop());

				return (*this);
			}
			//template<typename TInputValue = TValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Layer& input(const Eigen::Ref<const Eigen::MatrixX<TValue>>& input) {
				//Timer::Stopwatch stopwatch;
				/*if (input.rows() == mWeighted.rows()) {
					input = input.transpose();
				}
				if (input.rows() != mWeighted.rows()) {
					throw std::invalid_argument("The array length provided is not equal to the number of nodes");
				}*/

				mInputs.noalias() = input;

				//SOREN_CORE_TRACE("ID : {0} -> {1}", mId, __FUNCTION__);
				//SOREN_CORE_TRACE(stopwatch.Stop());

				return (*this);
			}
			//template<typename TInputValue = TValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Layer& input(const Eigen::MatrixX<TValue>&& input) {
				//Timer::Stopwatch stopwatch;
				/*if (input.rows() == mWeighted.rows()) {
					input = input.transpose();
				}
				if (input.rows() != mWeighted.rows()) {
					throw std::invalid_argument("The array length provided is not equal to the number of nodes");
				}*/

				mInputs.noalias() = input;

				//SOREN_CORE_TRACE("ID : {0} -> {1}", mId, __FUNCTION__);
				//SOREN_CORE_TRACE(stopwatch.Stop());

				return (*this);
			}

			inline const Eigen::MatrixX<TValue> output() const {
				mActivated.noalias() = (mWeighted.colwise() + mBiases).unaryExpr([&](TValue input) {
					return (mActivation)->operator()(input);
					});
				return (mLayerActivation)->operator()(mActivated);
				//return (*mLayerActivation)(mWeighted.unaryExpr(*mActivation) + mBiases);
			}

			Layer<TValue, TBias, TWeight>& resolve() {
				//Timer::Stopwatch stopwatch;
				//oneapi::tbb::simple_partitioner partitioner;

				if (mInputs.rows() != mWeights.cols()) {
					mInputs.conservativeResizeLike(Eigen::Vector<TValue, Eigen::Dynamic>(mWeights.cols()).setZero());
				}

				mWeighted.noalias() = mWeights * mInputs;
				//SOREN_CORE_INFO("Resolve");
				//SOREN_CORE_INFO(mInputs);
				//SOREN_CORE_INFO(mWeights);
				//SOREN_CORE_INFO(mWeighted);


				return *this;
			}

			template <typename TPipeValue = double, typename TPipeBias = double, typename TPipeWeight = double>
			Layer<TPipeValue, TPipeBias, TPipeWeight>& pipe(Layer<TPipeValue, TPipeBias, TPipeWeight>& layer) {
				layer.input(this->resolve());
				return layer;
			}


			template<typename TInputValue = TValue, typename TInputBias = TBias, typename TInputWeight = TWeight>
			inline Layer& backPropagate(const Eigen::Ref<const Eigen::Vector<TValue, Eigen::Dynamic>>& DLossDLayerActivation, Layer<TInputValue, TBias, TWeight>& input) { // DLoss/DOutput
				// DLossDActivated
				mDLossDActivated.noalias() = ((mLayerActivation)->prime(mActivated) /* DLayerActivation/DActivated */) * DLossDLayerActivation;

				// DActivated/DValue
				mDActivatedDValue.noalias() = (mWeighted + mBiases).unaryExpr([&](auto val) { return (mActivation)->prime(val); });

				// DLoss/DWeight
				mDLossDWeight = (mDActivatedDValue * mInputs.transpose() /* DValue/DWeight */)/* DActivated/DWeight */.array().colwise() * mDLossDActivated.array();

				if (input.mWeights.cols() || input.mWeights.rows()) {
					// DOutput/DInput
					mDActivatedDInput = mWeights.array().colwise() /* DValue/DInput */ * mDActivatedDValue.array();

					// DLoss/DInput
					mDLossDInput.noalias() = (mDActivatedDInput.transpose() * mDLossDActivated);
				}

				// Change Weights
				
				mWeights += mDLossDWeight;

				return (*this);
			}

			/*struct Gradients {
				Eigen::VectorX<double> DLossDInput{};
				Eigen::MatrixX<double> DLossDWeight{};
			};*/

			/*Gradients gradients(Eigen::VectorX<double> DLossDOutput) {
				Timer::Stopwatch stopwatch;
				
			}*/

			/*Layer& optimize(Eigen::MatrixX<double>& gradients) {
			
			}*/

			#pragma endregion

			//template <typename TValue>
			void generateLinks(Layer<TValue, TBias, TWeight>& input) {
				generateLinks(input, mLinkGenerator);
			}
			void generateLinks(Layer<TValue, TBias, TWeight>& input, LinkGenerator linkGenerator) {
				//oneapi::tbb::simple_partitioner partitioner;

				size_t size = mWeighted.size();
				mWeights = Eigen::Matrix<TWeight, Eigen::Dynamic, Eigen::Dynamic>(size, input.size());
				for (unsigned int i = 0; i < size; i++) {
					mWeights.row(i) = linkGenerator(i, input);
				}
			}


			// Special member functions
			#pragma region
			Layer(const Layer& arg)
				: BaseLayer(arg),
				mWeighted(arg.mWeighted),
				mBiases(arg.mBiases),
				mWeights(arg.mWeights),
				//mId(sId++),
				mRunning(arg.mRunning),
				mLocked(arg.mLocked),
				mActivation(arg.mActivation),
				mLayerActivation(arg.mLayerActivation),
				mLinkGenerator(arg.mLinkGenerator),
				mBiasGenerator(arg.mBiasGenerator)
			{
				LayerCopiedEvent event{ arg.mId, mId };
				Emit(event);
			};
			Layer(Layer<TValue, TBias, TWeight>&& arg) noexcept
				: BaseLayer(std::move(arg)),
				mWeighted(std::exchange(arg.mWeighted, {})),
				mBiases(std::exchange(arg.mBiases, {})),
				mWeights(std::exchange(arg.mWeights, {})),
				//mId(std::exchange(arg.mId, 0)),
				mRunning(std::exchange(arg.mRunning, false)),
				mLocked(std::exchange(arg.mLocked, true)),
				mActivation(std::exchange(arg.mActivation, std::make_shared<
					Activation::Activator<TValue>>(Activation::Identity<TValue>)
				)),
				mLayerActivation(std::exchange(arg.mLayerActivation, std::make_shared<
					Activation::Vector::Activator<TValue>>(Activation::Vector::Identity<TValue>)
				)),
				mLinkGenerator(std::exchange(arg.mLinkGenerator, mDefaultLinkGenerator)),
				mBiasGenerator(std::exchange(arg.mBiasGenerator, mDefaultBiasGenerator))
			{
				LayerMovedEvent event{mId};
				Emit(event);
			};
			Layer& operator=(const Layer& arg) {
				BaseLayer::operator=(arg);
				//if (*this == arg) return *this;

				mRunning = arg.mRunning;
				mWeighted = arg.mWeighted;
				mBiases = arg.mBiases;
				mWeights = arg.mWeights;
				mLocked = arg.mLocked;
				mActivation = arg.mActivation;
				mLayerActivation = arg.mLayerActivation;
				mLinkGenerator = arg.mLinkGenerator;
				mBiasGenerator = arg.mBiasGenerator;
				//mId = sId++;

				LayerCopyAssignedEvent event{};
				Emit(event);

				return *this;
			};
			Layer& operator=(Layer&& arg) noexcept {
				BaseLayer::operator=(std::move(arg));

				mWeighted = std::move(arg.mWeighted);
				mBiases = std::move(arg.mBiases);
				mWeights = std::move(arg.mWeights);
				mRunning = std::move(arg.mRunning);
				mLocked = std::move(arg.mLocked);
				mActivation = std::move(arg.mActivation);
				mLayerActivation = std::move(arg.mLayerActivation);
				mLinkGenerator = std::move(arg.mLinkGenerator);
				mBiasGenerator = std::move(arg.mBiasGenerator);


				//mId = std::move(arg.mId);

				LayerMoveAssignedEvent event{};
				Emit(event);
				return *this;
			};
			#pragma endregion

			// Operators
			template<typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
			friend inline Layer& operator>>(std::vector<T>& lhs, Layer& rhs) {
				return rhs.input(lhs);
			}
			friend inline Layer& operator>>(std::map<size_t, TValue>& lhs, Layer& rhs) {
				return rhs.input(lhs);
			}
			template<typename TLinkLayerValue = TValue,typename TLinkLayerBias = TBias, typename TLinkLayerWeight = TWeight>
			inline Layer<TLinkLayerValue, TLinkLayerBias, TLinkLayerWeight>& operator>>(Layer<TLinkLayerValue, TLinkLayerBias, TLinkLayerWeight>& rhs) {
				return pipe(rhs);
			}

			// Conversion

			// Iterator

			/*typedef typename std::map<size_t, Node<TValue,TBias,TWeight>>::iterator iterator;

			iterator begin() {
				return mNodes.begin();
			}
			iterator end() {
				return mNodes.end();
			}
			const iterator begin() const {
				return mNodes.begin();
			}
			const iterator end() const {
				return mNodes.end();
			}*/

		protected:

			//size_t mId;

		protected:

			// Events
			#pragma region
			//inline void Bind() { SetEventCallbackFunction(BIND_EVENT_FN(Link::OnEvent)); }

			void OnEvent(const Soren::Event& e) {
				if (!mRunning) return;
				EventDispatcher dispatcher(e);

				//dispatcher.Dispatch<NetworkCreatedEvent>(BIND_EVENT_FN(Layer::Init)); <- Might be virtuals
				//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN());
				////SOREN_CORE_TRACE(e);
				return;

			};
			#pragma endregion

			bool mRunning = true; // So as to allow links to be disabled and enabled
			bool mLocked = false;


			std::shared_ptr<Activation::ActivatorBase<TValue>> mActivation{ 
				std::make_shared<Activation::Activator<TValue>>
				(Activation::Identity<TValue>)
			};
			//inline static LayerActivation mDefaultLayerActivation = [](auto vec) { return vec; };
			std::shared_ptr<Activation::Vector::ActivatorBase<TValue>> mLayerActivation{ 
				std::make_shared<Activation::Vector::Activator<TValue>>
				(Activation::Vector::Identity<TValue>)
			};

			// Backprop
			Eigen::MatrixX<TValue> mLayerActivations;
			mutable Eigen::MatrixX<TValue> mActivated;
			Eigen::MatrixX<TValue> mInputs;
			Eigen::MatrixX<TValue> mWeighted;


			Eigen::Vector<TValue, Eigen::Dynamic> mDActivatedDValue;
			Eigen::Matrix<TValue, Eigen::Dynamic, Eigen::Dynamic> mDActivatedDInput;
			Eigen::Matrix<TValue, Eigen::Dynamic, Eigen::Dynamic> mDLossDWeight;
			Eigen::Vector<TValue, Eigen::Dynamic> mDLossDActivated;
			Eigen::Vector<TValue, Eigen::Dynamic> mDLossDInput;

			//Eigen::Vector<TValue, Eigen::Dynamic> mInputs;

			//Eigen::Vector<TValue, Eigen::Dynamic> mWeighted;
			Eigen::Vector<TBias, Eigen::Dynamic> mBiases;
			Eigen::MatrixX<TWeight> mWeights;

			inline static LinkGenerator mDefaultLinkGenerator = [](size_t index, auto& input) -> Eigen::Vector<TWeight, Eigen::Dynamic> {
				index;
				return Eigen::Vector<TWeight, Eigen::Dynamic>(input.size()).setRandom();
			};

			LinkGenerator mLinkGenerator = mDefaultLinkGenerator;

			inline static BiasGenerator mDefaultBiasGenerator = [](size_t index) -> TBias { index;  return (TBias)0; };

			BiasGenerator mBiasGenerator = mDefaultBiasGenerator;
		};


	} // namespace Neural
} // namespace Soren

