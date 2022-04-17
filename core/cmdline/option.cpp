#include "./option.hpp"

namespace cmdline {
    option::option(
            const char shortName,
            const std::string& longName,
            const std::string description,
            std::unique_ptr<basic_value> value)
        : shortName_(shortName), longName_(longName), desc_(description), value_(std::move(value)) {}
}