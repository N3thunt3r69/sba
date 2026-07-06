module;
#include <cstdint>
#include <atomic>
#include <array>
#include <mutex>
#include <memory>
#include <unordered_map>

export module sba.util.container;

export namespace SBA::Util {

	template <typename T>
	class PVector {
	private:
		static constexpr size_t SEGMENT_SIZE = (1 << 16);
		static constexpr size_t MAX_SEGMENTS = (1 << 16);

		using Segment = std::array<T, SEGMENT_SIZE>;
		std::atomic<uint32_t> size_;
		std::atomic<Segment*> segments_[MAX_SEGMENTS];
		std::mutex alloc_;

	public:
		PVector() : size_(0) {
			for (size_t i = 0; i < MAX_SEGMENTS; ++i)
				segments_[i].store(nullptr, std::memory_order_relaxed);
		}

		~PVector() {
			for (size_t i = 0; i < MAX_SEGMENTS; ++i)
				delete segments_[i].load(std::memory_order_relaxed);
		}

		uint32_t push_back(const T& val) {
			uint32_t idx = size_.fetch_add(1, std::memory_order_relaxed);
			uint32_t segment_idx = idx / SEGMENT_SIZE;
			uint32_t offset = idx % SEGMENT_SIZE;

			auto* segment = segments_[segment_idx].load(
				std::memory_order_acquire
			);
			if (!segment) {
				std::lock_guard<std::mutex> lock(alloc_);
				segment = segments_[segment_idx].load(
					std::memory_order_relaxed
				);
				if (!segment) {
					segment = new Segment();
					segments_[segment_idx].store(
						segment,
						std::memory_order_release
					);
				}
			}

			(*segment)[offset] = val;
			return idx;
		}

		T operator[](uint32_t idx) const {
			uint32_t segment_idx = idx / SEGMENT_SIZE;
			uint32_t offset = idx % SEGMENT_SIZE;
			auto* segment = segments_[segment_idx].load(
				std::memory_order_relaxed
			);
			return (*segment)[offset];
		}

		uint32_t size() const {
			return size_.load(std::memory_order_relaxed);
		}
	};

	template <typename Key,
			  typename Value,
			  typename Hash = std::hash<Key>,
			  typename KeyEqual = std::equal_to<Key>>
	class PMap {
	private:
		static constexpr size_t NUM_SHARDS = (1 << 7);

		struct Shard {
			std::mutex mutex;
			std::unordered_map<Key, Value, Hash, KeyEqual> map;
		};

		std::unique_ptr<Shard[]> shards;

	public:
		PMap() : shards(new Shard[NUM_SHARDS]) {}

		PMap(Hash h, KeyEqual eq) : shards(new Shard[NUM_SHARDS]) {
			for (size_t i = 0; i < NUM_SHARDS; ++i)
				shards[i].map = std::unordered_map
								<Key, Value, Hash, KeyEqual>(0, h, eq);
		}

		Value get_or_insert(const Key& key, const Value& val) {
			size_t h = Hash{}(key);
			size_t idx = h % NUM_SHARDS;
			auto& shard = shards[idx];
			std::lock_guard<std::mutex> lock(shard.mutex);
			auto it = shard.map.find(key);
			if (it != shard.map.end())
				return it->second;
			shard.map.insert({key, val});
			return val;
		}
	};

}
