#ifndef CMDLINE_TYPED_VALUE_H
#define CMDLINE_TYPED_VALUE_H

#include <any>
#include "./basic_value.hpp"

namespace cmdline {
    class typed_value : public basic_value {
        public:
        typed_value() = default;
        typed_value(std::any v)
            : v_(v) {}

        const std::type_info& type() const noexcept override {
            return v_.type();
        }

        virtual const basic_validator* get_validator() const override {
            return nullptr;
        }

        template<typename T>
        T as() const {
            return std::any_cast<T>(v_);
        }

        template<typename T>
        bool is() const {
            return type() == typeid(T);
        }

        private:
        std::any v_;
    };

}


#endif