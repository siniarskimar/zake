#ifndef CMDLINE_ABSTRACT_VALUE_H
#define CMDLINE_ABSTRACT_VALUE_H

#include <memory>
#include "./basic_value.hpp"
#include "./validator.hpp"

namespace cmdline {
    template<typename T>
    class abstract_value : public basic_value {
        public:

        abstract_value() {
            default_value(T());
        }

        abstract_value(const T& d) {
            default_value(d);
        }

        const std::type_info& type() const noexcept override {
            return typeid(T);
        }

        bool is_abstract() const noexcept override {
            return true;
        }

        abstract_value<T>* default_value(const T& v) {
            default_ = std::make_unique<T>(v);
            return this;
        }
        abstract_value<T>* implicit_value(bool implicit = true) {
            implicit_ = implicit;
            return this;
        }

        bool has_default() const noexcept {
            return default_ != nullptr;
        }

        bool is_implicit() const noexcept {
            return implicit_;
        }

        virtual const basic_validator* get_validator() const {
            return &validator_;
        }

        private:
        bool implicit_;
        std::unique_ptr<T> default_;
        validator<T> validator_;
    };

    template<typename T>
    abstract_value<T>* value(const T& v = T{}) {
        return new abstract_value<T>(v);
    }
}

#endif