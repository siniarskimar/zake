#ifndef CMDLINE_VALIDATOR_H
#define CMDLINE_VALIDATOR_H

#include <string>
#include <any>
#include "./basic_validator.hpp"

namespace cmdline {

    template<typename T>
    class validator : public basic_validator {
        public:
        std::any validate(const std::string& str) const {
            static_assert(true, "Missing a validator specialization");
            return std::make_any<std::nullptr_t>();
        }
    };
}

#endif