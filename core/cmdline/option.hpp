#ifndef CMDLINE_OPTION_H
#define CMDLINE_OPTION_H

#include <memory>
#include <string>
#include "./basic_value.hpp"

namespace cmdline {
    class option {
        public:
        option(
            const char shortName,
            const std::string& longName,
            const std::string description = std::string(),
            std::unique_ptr<basic_value> value = nullptr);

        inline bool has_long_name() const noexcept {
            return longName_ != "";
        }
        inline bool has_description() const noexcept {
            return desc_ != "";
        }

        const char shortName_;
        const std::string longName_;
        const std::string desc_;
        std::unique_ptr<basic_value> value_;
    };
}

#endif