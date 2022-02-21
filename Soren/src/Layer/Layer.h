#pragma once
#include "Core.h"
#include "Base.h"

#include "Eigen/Core"

#include "Status/Status.h"

#include "Log/Log.h"
#include "Events/Event.h"

#include "CallbackFunctions.h"

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

		class BaseLayer: public Linkable {
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

			inline size_t id() const override { return mId; }

			// Special member functions
			#pragma region
			BaseLayer(const BaseLayer&)
				: mId(sId++) {};
			BaseLayer(BaseLayer&& arg) noexcept
				: mId(std::exchange(arg.mId, 0)) {};
			BaseLayer& operator=(const BaseLayer&) {
				mId = sId++;
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

		public:

			Layer(size_t size = 0, TBias bias = 0, TValue value = 0)
				: mBias{bias}, mValue{ value }
			{
				// SOREN_CORE_WARN("No Default Link Weight Limits Set");

				for (size_t i = 0; i < size; i++) {
					//auto pNode = new Node<TValue, TBias, TWeight>({ bias, value });
					auto pNode = std::make_shared<Node<TValue, TBias, TWeight>>(bias, value);
					mNodes.emplace(std::make_pair<size_t, std::shared_ptr<Node<TValue, TBias, TWeight>>>(pNode ->id(), std::move(pNode)));
				}

				//Bind();

				LayerCreatedEvent event{};
				Emit(event);
			};
			Layer(size_t size, std::function<TValue(const TValue)> activation, TBias bias = 0, TValue value = 0)
				: mActivation(activation), mBias(bias), mValue(value)
			{
				// SOREN_CORE_WARN("No Default Link Weight Limits Set");
				for (size_t i = 0; i < size; i++) {
					auto pNode = std::make_shared<Node<TValue, TBias, TWeight>>( mActivation, bias, value );
					mNodes.emplace(std::make_pair<size_t,std::shared_ptr<Node<TValue,TBias,TWeight>>>(pNode->id(), std::move(pNode)));
				}
				//Bind();

				LayerCreatedEvent event{};
				Emit(event);
			};
			Layer(size_t size, std::function<std::vector<TValue>(std::vector<TValue>)> layerActivation, TBias bias = 0, TValue value = 0)
				:  mLayerActivation(layerActivation), mBias(bias), mValue(value)
			{
				// SOREN_CORE_WARN("No Default Link Weight Limits Set");
				for (size_t i = 0; i < size; i++) {
					auto pNode = std::make_shared<Node<TValue, TBias, TWeight>>( bias, value );
					mNodes.emplace(std::make_pair<size_t,std::shared_ptr<Node<TValue,TBias,TWeight>>>(pNode->id(), std::move(pNode)));
				}
				//Bind();

				LayerCreatedEvent event{};
				Emit(event);
			};
			Layer(size_t size, std::function<std::vector<TValue>(std::vector<TValue>)> layerActivation, std::function<TValue(const TValue)> activation, TBias bias = 0, TValue value = 0)
				: mActivation(activation), mLayerActivation(layerActivation), mBias(bias), mValue(value)
			{
				// SOREN_CORE_WARN("No Default Link Weight Limits Set");
				for (size_t i = 0; i < size; i++) {
					auto pNode = std::make_shared<Node<TValue, TBias, TWeight>>( mActivation, bias, value );
					mNodes.emplace(std::make_pair<size_t,std::shared_ptr<Node<TValue,TBias,TWeight>>>(pNode->id(), std::move(pNode)));
				}
				//Bind();

				LayerCreatedEvent event{};
				Emit(event);
			};
			~Layer() {
				LayerDeletedEvent event{};
				Emit(event);
			};

			// Links
			inline TValue value() const { return mValue; }
			inline size_t size() const { return mNodes.size(); }
			inline TBias bias() const { return mBias; }
			inline const std::map<size_t,std::shared_ptr<Node<TValue, TBias, TWeight>>>& nodes() const { return mNodes; }
			inline bool running() const { return mRunning; }
			inline bool locked() const { return mLocked; }

			void clear() {
				if (mLocked) return;

				const weight = mBias;
				mBias = NULL;

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
				LayerResetEvent event(mBias);
				Emit(event);
			};

			inline void value(TValue value) { if (mLocked) return; mValue = value; }
			inline void bias(TBias bias) { if (mLocked) return; mBias = bias; }
			inline void lock(bool lock) { mLocked = lock; }
			inline void lock() { mLocked = true; }
			inline void unlock() { mLocked = false; }

			inline void setLinkGenerator(std::function<std::vector<size_t>(std::shared_ptr<Node<TValue, TBias, TWeight>> current, Layer<TValue, TBias, TWeight>& inputLayer)> linkGenerator) { if (mLocked) return; mLinkGenerator = linkGenerator; }
			inline void setWeightGenerator(std::function<TWeight(void)> weightGenerator) { if (mLocked) return; mWeightGenerator = weightGenerator; }

			// Use Async to create many links at once

			template<typename TLinkValue = TValue , typename TLinkBias = TBias,typename TLinkWeight=TWeight>
			inline void link(Layer<TLinkValue, TLinkBias, TLinkWeight>& layer) {
				for (auto [node, linkNodes] : layer.generateLinks(*this)) {
					for (size_t linkNodeId : linkNodes) {
						if (auto pos = mNodes.find(linkNodeId); pos != mNodes.end())
							mLinker.link(node, pos->second, mWeightGenerator());
					}
				}
				
			}
			template<typename TLinkValue = TValue , typename TLinkBias = TBias,typename TLinkWeight=TWeight>
			inline void link(Layer<TLinkValue, TLinkBias, TLinkWeight>& layer,TWeight weight) {
				for (auto [node, linkNodes] : layer.generateLinks(*this)) {
					for (size_t linkNodeId : linkNodes) {
						if (auto pos = mNodes.find(linkNodeId); pos != mNodes.end())
							mLinker.link(node, pos->second, weight);
					}
				}
			}
			template<typename TLinkValue = TValue , typename TLinkBias = TBias,typename TLinkWeight=TWeight>
			inline void link(Layer<TLinkValue, TLinkBias, TLinkWeight>& layer, std::function<std::vector<size_t>(std::shared_ptr<Node<TValue, TBias, TWeight>> current, Layer<TValue, TBias, TWeight>& inputLayer)> linkGenerator,TWeight weight = 0) {
				for (auto [node, linkNodes] : layer.generateLinks(*this, linkGenerator)) {
					for (size_t linkNodeId : linkNodes) {
						if (auto pos = mNodes.find(linkNodeId); pos != mNodes.end())
							mLinker.link(node, pos->second, weight);
					}
				}
			}
			template<typename TLinkValue = TValue , typename TLinkBias = TBias,typename TLinkWeight = TWeight>
			inline void link(Layer<TLinkValue, TLinkBias, TLinkWeight>& layer,std::function<TWeight(void)> weightGenerator) {
				for (auto [node, linkNodes] : layer.generateLinks(*this)) {
					for (size_t linkNodeId : linkNodes) {
						if (auto pos = mNodes.find(linkNodeId); pos != mNodes.end())
							mLinker.link(node, pos->second, weightGenerator);
					}
				}
			}
			template<typename TLinkValue = TValue , typename TLinkBias = TBias,typename TLinkWeight = TWeight>
			inline void link(Layer<TLinkValue, TLinkBias, TLinkWeight>& layer, std::function<std::vector<size_t>(std::shared_ptr<Node<TValue, TBias, TWeight>> current, Layer<TValue, TBias, TWeight>& inputLayer)> linkGenerator, std::function<TWeight(void)> weightGenerator) {
				for (auto [node, linkNodes] : layer.generateLinks(*this, linkGenerator)) {
					for (size_t linkNodeId : linkNodes) {
						if (auto pos = mNodes.find(linkNodeId); pos != mNodes.end())
							mLinker.link(node, pos->second, weightGenerator);
					}
				}
			}

			// Eigen
			#pragma region
			template<typename TInputValue = TValue>
			inline void inputMatrix(Eigen::VectorX<TInputValue> value) {
				auto pos = mNodes.begin();
				for (size_t iNodes = 0; iNodes < mNodes.size(); iNodes++, pos++) {
					pos->second->input(value(iNodes));
				}
			}

			inline Eigen::VectorX<typename std::common_type<TValue, TBias>::type> outputMatrix() const {

				Eigen::VectorX<TBias> vector(mNodes.size());
				vector.setZero();
				auto pos = mNodes.begin();

				for (size_t iNodes = 0; iNodes < mNodes.size(); iNodes++, pos++) {
					vector(iNodes) = pos->second->output();
				}
				return vector;
			}

			template <typename TLinkValue = double, typename TLinkBias = double, typename TLinkWeight = double>
			Eigen::MatrixX<TWeight> linkMatrix(const Layer<TLinkValue, TLinkBias,TLinkWeight>& layer) {
				Eigen::MatrixX<TWeight> matrix(layer.mNodes.size(), mNodes.size());
				matrix.setZero();
				auto pos = mNodes.begin();
				for (size_t iNodes = 0; iNodes < mNodes.size(); iNodes++, pos++) {
					for (auto& [linkedNode, link] : mLinker.links().at(pos->second)) {
						auto linkedNodePos = layer.mNodes.find(linkedNode.get()->id());
						//auto linkedNodePos = std::find_if(layer.mNodes.begin(), layer.mNodes.end(), [linkedNode](auto& [id,node]) {return node.id() == linkedNode.get().id(); });
						if (linkedNodePos != layer.mNodes.end()) {
							matrix(std::distance(layer.mNodes.begin(), linkedNodePos), iNodes) = link.weight();
						}
					}
				}
				return matrix;
			}

			Eigen::VectorX<TValue> valueMatrix() const {
				Eigen::VectorX<TBias> vector(mNodes.size());
				vector.setZero();
				auto pos = mNodes.begin();

				for (size_t iNodes = 0; iNodes < mNodes.size(); iNodes++, pos++) {
					vector(iNodes) = pos->second.value();
				}
				return vector;
			}

			Eigen::VectorX<TBias> biasMatrix() const {
				Eigen::VectorX<TBias> vector(mNodes.size());
				vector.setZero();
				auto pos = mNodes.begin();

				for (size_t iNodes = 0; iNodes < mNodes.size(); iNodes++, pos++) {
					vector(iNodes) = pos->second.bias();
				}
				return vector;
			}

			template <typename TResolveValue = double, typename TResolveBias = double, typename TResolveWeight = double>
			Eigen::VectorX<typename std::common_type<TValue, TBias, TWeight>::type> resolveMatrix(Layer<TResolveValue, TResolveBias, TResolveWeight>& layer) {
				return linkMatrix(layer)* outputMatrix();
			}

			template <typename TPipeValue = double, typename TPipeBias = double, typename TPipeWeight = double>
			Layer<TPipeValue, TPipeBias, TPipeWeight>& pipeMatrix(Layer<TPipeValue, TPipeBias, TPipeWeight>& layer) {
				layer.inputMatrix(resolveMatrix(layer));
				return layer;
			}
			#pragma endregion

			// Basic	
			#pragma region
			template<typename TInputValue = TValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Layer& input(std::vector<TInputValue>& value) {
				auto activatedValues = mLayerActivation(std::vector<TValue>(value.begin(), value.end()));
				auto pos = mNodes.begin();
				for (size_t iNodes = 0; iNodes < mNodes.size() && pos != mNodes.end(); iNodes++, pos++) {
					pos->second->input(activatedValues[iNodes]);
				}
				return (*this);
			}
			template<typename TInputValue = TValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Layer& input(std::vector<TInputValue>&& value) {
				auto activatedValues = mLayerActivation(std::vector<TValue>(value.begin(), value.end()));
				auto pos = mNodes.begin();
				for (size_t iNodes = 0; iNodes < mNodes.size() && pos != mNodes.end(); iNodes++, pos++) {
					pos->second->input(activatedValues[iNodes]);
				}
				return (*this);
			}
			template<typename TInputValue = TValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Layer& input(std::map<size_t, TInputValue>& value) {
				std::vector<TValue> valueVec;
				std::vector<size_t> idVec;
				valueVec.reserve(value.size());
				idVec.reserve(value.size());
				for (auto [id, val] : value) {
					idVec.push_back(id);
					valueVec.push_back(val);
				}
				auto activatedVec = mLayerActivation(valueVec);
				std::map<size_t, TValue> activatedMap{};
				for (size_t iValue = 0; iValue < activatedVec.size(); iValue++) {
					activatedMap[idVec[iValue]] = activatedVec[iValue];
				}

				for (auto [id, activatedValue] : activatedMap) {
					rhs.mNodes.at(id).input(activatedValue);
				}
				return *this;
			}
			template<typename TInputValue = TValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Layer& input(std::map<size_t, TInputValue>&& value) {
				std::vector<TValue> valueVec;
				std::vector<size_t> idVec;
				valueVec.reserve(value.size());
				idVec.reserve(value.size());
				for (auto [id, val] : value) {
					idVec.push_back(id);
					valueVec.push_back(val);
				}
				auto activatedVec = mLayerActivation(valueVec);
				std::map<size_t, TValue> activatedMap{};
				for (size_t iValue = 0; iValue < activatedVec.size(); iValue++) {
					activatedMap[idVec[iValue]] = activatedVec[iValue];
				}

				for (auto [id, activatedValue] : activatedMap) {
					mNodes.at(id)->input(activatedValue);
				}
				return *this;
			}

			inline std::vector<typename std::common_type<TValue, TBias>::type> output() const {

				std::vector<TBias> vector;
				auto pos = mNodes.begin();

				for (size_t iNodes = 0; iNodes < mNodes.size(); iNodes++, pos++) {
					vector.push_back(pos->second->output());
				}
				return vector;
			}

			template<typename TLinkLayerValue = TValue, typename TLinkLayerBias = TBias, typename TLinkLayerWeight = TWeight>
			std::map<size_t, TLinkLayerValue> resolveMap(Layer<TLinkLayerValue, TLinkLayerBias, TLinkLayerWeight>& layer) {

				std::map<size_t, TLinkLayerValue> data;

				for (auto& [linkNode, links] : mLinker.links()) {
					if (layer.nodes().find(linkNode.get()->id()) != layer.nodes().end()) {
						for (auto& [node, link] : links) {
							data[linkNode.get()->id()] += static_cast<TLinkLayerValue>(link.pipe<TValue>(dynamic_cast<Node<TValue, TBias, TWeight>*>(node.get())->output<TValue, TBias>()));
						}
					}
				}

				return data;
			}

			template<typename TLinkLayerValue = TValue, typename TLinkLayerBias = TBias, typename TLinkLayerWeight = TWeight>
			std::vector<TLinkLayerValue> resolve(Layer<TLinkLayerValue, TLinkLayerBias, TLinkLayerWeight>& layer) {

				std::map<size_t, TLinkLayerValue> data;
				std::vector<TLinkLayerValue> dataVec;

				for (auto& [node, links] : mLinker.links()) {
					for (auto& [linkedNode, link] : links) {
						data[linkedNode.get().id()] += static_cast<TLinkLayerValue>(link.pipe<TValue>(dynamic_cast<Node<TValue, TBias, TWeight>*>((node.get()))->output()));
					}
				}

				for (auto& [node, links] : rhs.mLinker.links()) {
					for (auto& [linkedNode, link] : links) {
						auto pos = mNodes.find(linkedNode.get().id());
						if (pos != mNodes.end()) {
							data[node.get().id()] += static_cast<TLinkLayerValue>(link.pipe<TLinkLayerValue>(dynamic_cast<Node<TLinkLayerValue, TLinkLayerBias, TLinkLayerWeight>*>(&(linkedNode.get()))->output()));
						}
					}
				}

				dataVec.reserve(data.size());

				for (auto& [id, value] : data) {
					dataVec.push_back(value);
				}
				return dataVec;
			}

			template <typename TPipeValue = double, typename TPipeBias = double, typename TPipeWeight = double>
			Layer<TPipeValue, TPipeBias, TPipeWeight>& pipe(Layer<TPipeValue, TPipeBias, TPipeWeight>& layer) {
				layer.input(resolveMap(layer));
				return layer;
			}
			#pragma endregion

			//template <typename TValue>
			std::map<std::shared_ptr<Node<TValue, TBias, TWeight>>, std::vector<size_t>> generateLinks(Layer<TValue, TBias, TWeight>& input) {
				std::map<std::shared_ptr<Node<TValue, TBias,TWeight>>, std::vector<size_t>> links{};
				for (auto& [id, node] : mNodes) {
					links.insert(std::make_pair(node, mLinkGenerator(node, input)));
				}
				return links;
			}
			std::map<std::shared_ptr<Node<TValue, TBias, TWeight>>, std::vector<size_t>> generateLinks(Layer<TValue, TBias, TWeight>& input, std::function<std::vector<size_t>(std::shared_ptr<Node<TValue, TBias, TWeight>> current, Layer<TValue, TBias, TWeight> inputLayer)> linkGenerator) {
				std::map<std::shared_ptr<Node<TValue, TBias,TWeight>>, std::vector<size_t>> links{};
				for (auto& [id, node] : mNodes) {
					links.insert(std::make_pair(node, linkGenerator(node, input)));
				}
				return links;
			}


			// Special member functions
			#pragma region
			Layer(const Layer& arg)
				: BaseLayer(arg),
				mValue(arg.mValue),
				mBias(arg.mBias),
				mNodes(arg.mNodes),
				mLinker(arg.mLinker),
				//mId(sId++),
				mRunning(arg.mRunning),
				mLocked(arg.mLocked),
				mActivation(arg.mActivation),
				mLayerActivation(arg.mLayerActivation),
				mLinkGenerator(arg.mLinkGenerator),
				mWeightGenerator(arg.mWeightGenerator)
			{
				LayerCopiedEvent event{ arg.mId, mId };
				Emit(event);
			};
			Layer(Layer<TValue, TBias, TWeight>&& arg) noexcept
				: BaseLayer(std::move(arg)),
				mValue(std::exchange(arg.mValue, static_cast<TValue>(0))),
				mBias(std::exchange(arg.mBias, static_cast<TBias>(0))),
				mNodes(std::exchange(arg.mNodes, {})),
				mLinker(std::exchange(arg.mLinker, {})),
				//mId(std::exchange(arg.mId, 0)),
				mRunning(std::exchange(arg.mRunning, false)),
				mLocked(std::exchange(arg.mLocked, true)),
				mActivation(std::exchange(arg.mActivation, Activation::Identity<TValue>)),
				mLayerActivation(std::exchange(arg.mLayerActivation, [](auto vec) {return vec; })),
				mLinkGenerator(std::exchange(arg.mLinkGenerator, arg.mDefaultLinkGenerator)),
				mWeightGenerator(std::exchange(arg.mWeightGenerator, []() -> TWeight {return 0; }))
			{
				LayerMovedEvent event{mId};
				Emit(event);
			};
			Layer& operator=(const Layer& arg) {
				BaseLayer::operator=(arg)
				//if (*this == arg) return *this;

				mValue = arg.mValue;
				mBias = arg.mBias;
				mNodes = arg.mNodes;
				mLinker = arg.mLinker;
				mRunning = arg.mRunning;
				mLocked = arg.mLocked;
				mActivation = arg.mActivation;
				mLayerActivation = arg.mLayerActivation;
				mLinkGenerator = arg.mLinkGenerator;
				mWeightGenerator = arg.mWeightGenerator;
				//mId = sId++;

				LayerCopyAssignedEvent event{};
				Emit(event);

				return *this;
			};
			Layer& operator=(Layer&& arg) noexcept {
				BaseLayer::operator=(std::move(arg))

				mValue = std::move(arg.mValue);
				mBias = std::move(arg.mBias);
				mNodes = std::move(arg.mNodes);
				mLinker = std::move(arg.mLinker)
				mRunning = std::move(arg.mRunning);
				mLocked = std::move(arg.mLocked);
				mActivation = std::move(arg.mActivation);
				mLayerActivation = std::move(arg.mLayerActivation);
				mLinkGenerator = std::move(arg.mLinkGenerator);
				mWeightGenerator = std::move(arg.mWeightGenerator);
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

			operator Eigen::VectorX<TValue>() const {
				Eigen::VectorX<TValue> vector(mNodes.size());
				auto pos = mNodes.begin();
				for (size_t iNode = 0; iNode < mNodes.size();iNode++, pos++) {
					vector(iNode) = (*pos).second.value();
				}
				return vector;
			}

			// Iterator

			typedef typename std::map<size_t, Node<TValue,TBias,TWeight>>::iterator iterator;

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
			}

		protected:

			//size_t mId;

		private:

			// Events
			#pragma region
			//inline void Bind() { SetEventCallbackFunction(BIND_EVENT_FN(Link::OnEvent)); }

			void OnEvent(const Soren::Event& e) {
				if (!mRunning) return;
				EventDispatcher dispatcher(e);

				//dispatcher.Dispatch<NetworkCreatedEvent>(BIND_EVENT_FN(Layer::Init)); <- Might be virtuals
				//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN());
				//SOREN_CORE_TRACE(e);
				return;

			};
			#pragma endregion

			Linker<TWeight> mLinker{};

			bool mRunning = true; // So as to allow links to be disabled and enabled
			bool mLocked = false;

			std::map<size_t, std::shared_ptr<Node<TValue, TBias, TWeight>>> mNodes{};

			std::function<TValue(const TValue)> mActivation = Activation::Identity<TValue>;
			std::function<std::vector<TValue>(const std::vector<TValue>)> mLayerActivation = []( auto vec) {return vec; };

			std::function<std::vector<size_t>(std::shared_ptr<Node<TValue, TBias, TWeight>> current, Layer<TValue, TBias, TWeight>& inputLayer)> mDefaultLinkGenerator = [](auto node, auto inputLayer) -> std::vector<size_t> {
				std::vector<size_t> ids;
				ids.reserve(inputLayer.mNodes.size());
				for (auto& [id, inputNode] : inputLayer.mNodes) {
					ids.push_back(id);
				}
				return ids;
			};

			std::function<std::vector<size_t>(std::shared_ptr<Node<TValue, TBias, TWeight>> current, Layer<TValue, TBias, TWeight>& inputLayer)> mLinkGenerator = mDefaultLinkGenerator;

			std::function<TWeight(void)> mWeightGenerator = []() -> TWeight {return 0; };
			TValue mValue = 0;
			TBias mBias = 0;
		};


	} // namespace Neural
} // namespace Soren

