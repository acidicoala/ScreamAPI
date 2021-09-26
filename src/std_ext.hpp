#pragma once

/**
 * Provides extensions for various standard library containers.
 * Composition is preferred to inheritance.
 */
namespace std_ext {
    /**
     * A convenience wrapper over std::vector
     */
    template<typename T>
    struct list {
        using Predicate = std::function<bool(const T&)>;

        std::vector<T> vector{};

        T& operator[](int index) {
            return vector.at(index);
        }

        bool any(Predicate predicate) const {
            for (auto& element: vector) {
                if (predicate(element)) {
                    return true;
                }
            }

            return false;
        }

        auto add(const T& element) {
            vector.push_back(element);
        }

        auto add_if_every(const T& element, Predicate predicate) {
            if (this->every(predicate)) {
                vector.push_back(element);
            }
        }

        auto operator+=(const T& element) {
            this->add(element);
        }

        auto add_all(const std::vector<T>& another) {
            vector.insert(vector.end(), another.begin(), another.end());
        }

        auto operator+=(const std::vector<T>& another) {
            this->add_all(another);
        }

        auto count() const {
            return static_cast<uint32_t>(vector.size());
        }

        bool contain(const T& element) const {
            return any([&](const T& member) { return member == element; });
        }

        bool contain(Predicate predicate) const {
            return any(predicate);
        }

        bool every(Predicate predicate) const {
            for (auto& element: vector) {
                if (not predicate(element)) {
                    return false;
                }
            }

            return true;
        }

        auto raw() const {
            return vector.data();
        }
    };

}