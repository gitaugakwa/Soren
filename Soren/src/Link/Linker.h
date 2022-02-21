#pragma once
#include <variant>
#include <stdexcept>

#include "Core.h"
#include "Base.h"

#include "Status/Status.h"

#include "Log/Log.h"
#include "Events/Event.h"

#include "CallbackFunctions.h"

#include "nlohmann/json.hpp"

namespace Soren {
	namespace Neural {
		struct Linkable {
			template<typename TInput>
			void input(TInput input);
			template<typename TValue, typename TBias>
			typename std::common_type<TValue, TBias>::type output() const;

			virtual size_t id() const {
				return mId;
			};

			//virtual void output() = 0;
			template<typename TInput>
			Linkable& input(TInput);

			virtual bool operator ==(const Linkable& rhs) const noexcept {
				return mId == rhs.mId;
			}
			virtual bool operator <(const Linkable& rhs) const noexcept {
				return mId < rhs.mId;
			}

		protected: 
			size_t mId;
		};

		struct LinkerLink {
			template<typename TValue>
			TValue pipe(TValue);
		};
	}
}


namespace std {
	template<>
	struct less<std::reference_wrapper<Soren::Neural::Linkable>> {
		bool operator() (const std::reference_wrapper < Soren::Neural::Linkable>& lhs, const std::reference_wrapper < Soren::Neural::Linkable>& rhs) const {
			return lhs.get().id() < rhs.get().id();

		}
	};
}


namespace Soren {

	template <typename T>
	class FileOutput;
	template <typename T>
	class FileInput;

	namespace Neural {

		template<typename TWeight>
		class Link;

		class Event;

		template<typename TWeight = double>
		class SOREN_API Linker
		{
		public:
			Linker() {

			}
			~Linker() {

			}

			const std::map<std::shared_ptr<Linkable>, std::map<std::shared_ptr<Linkable>, Link<TWeight>>>& links() const { return mLinks; }

			LinkerLink& link(std::shared_ptr<Linkable> linkable1, std::shared_ptr<Linkable> linkable2, TWeight weight) {
				if (mLinks.find(linkable2) != mLinks.end() && mLinks[linkable2].find(linkable1) != mLinks[linkable2].end()) {
					return mLinks[linkable2][linkable1];
				}
				return mLinks[linkable1][linkable2] = Link<TWeight>(weight);
			}

			LinkerLink& get(std::shared_ptr<Linkable> linkable1, std::shared_ptr<Linkable> linkable2) {
				if (mLinks.find(linkable1) != mLinks.end() && mLinks[linkable1].find(linkable2) != mLinks[linkable1].end()) {

					return mLinks[linkable1][linkable2];

				}
				if (mLinks.find(linkable2) != mLinks.end() && mLinks[linkable2].find(linkable1) != mLinks[linkable2].end()) {

					return mLinks[linkable2][linkable1];
				}
				throw std::invalid_argument("No Link exists between linkables");
			}

			// Special member functions
			#pragma region
			Linker(const Linker& arg)
				: mLinks(arg.mLinks) {};
			Linker(Linker<TWeight>&& arg) noexcept
				: mLinks(std::exchange(arg.mLinks, {})) {};
			Linker& operator=(const Linker& arg) {
				mLinks = arg.mLinks;
				return *this;	
			};
			Linker& operator=(Linker&& arg) noexcept {

				mLinks = std::move(arg.mLinks);
				return *this;
			};
			#pragma endregion

		private:

			std::map<std::shared_ptr<Linkable>, std::map<std::shared_ptr<Linkable>, Link<TWeight>>> mLinks{};

		};

	} // namespace Neural
} // namespace Soren


