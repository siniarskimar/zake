#ifndef CMDLINE_PARSE_RESULT_H
#define CMDLINE_PARSE_RESULT_H

#include <string>
#include <map>
#include "./typed_value.hpp"

namespace cmdline {
    class parse_result {
        public:
        bool has(const std::string& optionName) const;
        const typed_value& get(const std::string& optionName) const;
        const typed_value& operator[](const std::string& optionName) const;

        void insert(const std::string& name, const typed_value& value);
        void insert(const std::string& name, typed_value&& value);

        private:
        std::map<std::string, const typed_value> map_;
    };
}

#endif