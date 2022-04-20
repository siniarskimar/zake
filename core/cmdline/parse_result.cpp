#include "./parse_result.hpp"

namespace cmdline {
    bool parse_result::has(const std::string& optionName) const {
        return map_.count(optionName) > 0;
    }

    const typed_value& parse_result::get(const std::string& optionName) const {
        return map_.at(optionName);
    }

    const typed_value& parse_result::operator[](const std::string& optionName) const {
        return get(optionName);
    }

    void parse_result::insert(const std::string& name, const typed_value& value) {
        map_.emplace(name, value);
    }

    void parse_result::insert(const std::string& name, typed_value&& value) {
        map_.emplace(name, std::move(value));
    }
}