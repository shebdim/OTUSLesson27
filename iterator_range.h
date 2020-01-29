#pragma once

#include <algorithm>

template <typename It>
class IteratorRange {
public:
    IteratorRange(It first, It last) : first(first), last(last) {}

    It begin() const {
        return first;
    }

    It end() const {
        return last;
    }

    It& begin() {
        return first;
    }

    It& end() {
        return last;
    }

    [[nodiscard]] std::size_t size() const {
        return last - first;
    }

    [[nodiscard]] bool empty() const {
        return last == first;
    }

private:
    It first, last;
};

template <typename Container>
auto Head(Container& c, int top) {
    return IteratorRange(begin(c), begin(c) + std::min<std::size_t>(std::max(top, 0), c.size()));
}

template <typename Cont>
auto asRange(const Cont& container) {
  return IteratorRange{std::begin(container), std::end(container)};
}

template <typename Cont>
bool isAllEmpty(const Cont& cont) {
    for (auto range : cont) {
        if (!range.empty()) return false;
    }
    return true;
}

