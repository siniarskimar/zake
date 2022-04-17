#include "./validator.hpp"
#include <regex>

namespace cmdline {
    template<>
    std::any validator<std::string>::validate(const std::string& str) const {
        return std::make_any<std::string>(str);
    }

    template<>
    std::any validator<int>::validate(const std::string& str) const {
        static const std::regex pattern("[0-9]+");
        std::smatch m;
        if(!std::regex_match(str, m, pattern)) {
            return std::make_any<std::nullptr_t>();
        }
        return std::make_any<int>(std::atoi(str.c_str()));
    }

    template<>
    std::any validator<float>::validate(const std::string& str) const {
        static const std::regex pattern("-?[0-9]+([\\.][0-9]+)?");
        std::smatch m;
        if(!std::regex_match(str, m, pattern)) {
            return std::make_any<std::nullptr_t>();
        }
        return std::make_any<float>(std::atof(str.c_str()));
    }

    template<>
    std::any validator<double>::validate(const std::string& str) const {
        static const std::regex pattern("-?[0-9]+([\\.][0-9]+)?");
        std::smatch m;
        if(!std::regex_match(str, m, pattern)) {
            return std::make_any<std::nullptr_t>();
        }
        return std::make_any<double>(std::atof(str.c_str()));
    }
    
}