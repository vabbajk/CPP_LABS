#pragma once

#include <iterator>
#include <type_traits>
#include <cstddef>

namespace analytics {

template<typename Iterator, typename Predicate, typename ValueExtractor>
auto sum_if(Iterator first, Iterator last, Predicate predicate, ValueExtractor extractor)
    -> std::decay_t<std::invoke_result_t<ValueExtractor, typename std::iterator_traits<Iterator>::reference>> {
    using Category = typename std::iterator_traits<Iterator>::iterator_category;
    static_assert(std::is_base_of_v<std::input_iterator_tag, Category>,
                  "analytics::sum_if requires at least an input iterator");
    using Result = std::decay_t<std::invoke_result_t<ValueExtractor, typename std::iterator_traits<Iterator>::reference>>;
    Result total{};
    for (; first != last; ++first) {
        if (predicate(*first)) {
            total += extractor(*first);
        }
    }
    return total;
}

template<typename T>
class ValueAccumulator {
public:
    void add(const T& value) {
        total_ += value;
        ++count_;
    }

    T total() const {
        return total_;
    }

    double average() const {
        return count_ ? static_cast<double>(total_) / static_cast<double>(count_) : 0.0;
    }

    std::size_t count() const {
        return count_;
    }

private:
    T total_{};
    std::size_t count_{0};
};

}


