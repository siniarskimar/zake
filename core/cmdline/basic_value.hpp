#ifndef CMDLINE_BASIC_VALUE_H
#define CMDLINE_BASIC_VALUE_H

#include "./basic_validator.hpp"

namespace cmdline {
    class basic_value {
        public:
        virtual ~basic_value() = default;
        virtual const std::type_info& type() const noexcept = 0;
        virtual const basic_validator* get_validator() const {
            return nullptr;
        };
        virtual bool is_abstract() const noexcept {
            return false;
        };
    };
}

#endif