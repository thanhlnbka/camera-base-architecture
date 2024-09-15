#ifndef DYNAMIC_PROPERTIES_HPP
#define DYNAMIC_PROPERTIES_HPP

#include <unordered_map>
#include <string>
#include <any>
#include <optional>
#include <mutex>

class DynamicProperties {
public:

    DynamicProperties() = default;


    DynamicProperties(const DynamicProperties&) = delete;
    DynamicProperties& operator=(const DynamicProperties&) = delete;


    DynamicProperties(DynamicProperties&&) noexcept = default;
    DynamicProperties& operator=(DynamicProperties&&) noexcept = default;


    ~DynamicProperties() = default;


    template<typename T>
    void set_property(const std::string& key, const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        properties[key] = value;
    }


    template<typename T>
    std::optional<T> get_property(const std::string& key) const {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = properties.find(key);
        if (it != properties.end()) {
            try {
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast&) {
                return std::nullopt;
            }
        }
        return std::nullopt;
    }


    std::vector<std::string> list_properties() const {
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<std::string> keys;
        for (const auto& pair : properties) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    std::shared_ptr<DynamicProperties> clone() const {
        auto clone = std::make_shared<DynamicProperties>();
        {
            std::lock_guard<std::mutex> lock(mtx);
            clone->properties = properties;
        }
        return clone;
    }

private:
    mutable std::mutex mtx;
    std::unordered_map<std::string, std::any> properties;
};

#endif
