#ifndef CMDLINE_BASIC_VALIDATOR_H
#define CMDLINE_BASIC_VALIDATOR_H

#include <string>
#include <any>

namespace cmdline {
    class basic_validator {
        public:
        virtual ~basic_validator() = default;
        virtual std::any validate(const std::string& str) const = 0;
    };
}

#endif