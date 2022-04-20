#ifndef CMDLINE_OPTIONS_H
#define CMDLINE_OPTIONS_H

#include <vector>
#include <map>
#include <memory>
#include <string>

#include "./basic_value.hpp"
#include "./parse_result.hpp"
#include "./option.hpp"

namespace cmdline {
    class options {
        private:
        struct option_adder {
            option_adder(options* optDesc, const std::string& group);
            option_adder& operator()(
                const std::string& name,
                const std::string& desc = std::string(),
                basic_value* value = nullptr);
            private:
            options* optDesc_;
            const std::string group_;
        };

        public:
        options(const std::string& programName, const std::string& shortDesc = std::string());

        /**
         * @brief Starts a new option add chain.
         * 
         * @param group Option group name
         * @return option_adder 
         */
        option_adder add_options(const std::string& group = std::string("")) {
            return option_adder(this, group);
        }

        /**
         * @brief Adds a new option
         * 
         * Adds a new option to group.
         * 
         * If the group name is left blank ("") the option
         * will be added to the default group "Options".
         * 
         * If a given group doesn't exist, it will be created
         * upon calling this function.
         * 
         * @param group Option group name
         * @param name Option name
         * @param desc Option description
         * @param value Option default value, if any
         */
        void add_option(
            const std::string& group,
            const std::string& name,
            const std::string& desc,
            basic_value* value = nullptr);
        
        void set_positional_options(const std::initializer_list<std::string>&);

        const parse_result parse(int argc, const char* argv[]);

        std::string format_help();

        private:
        std::shared_ptr<option> find_option(const std::string& longName);
        std::shared_ptr<option> find_short_option(const char& name);

        private:
        std::string programName_;
        std::string programDescription_;
        std::vector<std::shared_ptr<option>> options_;
        std::vector<std::shared_ptr<option>> positionals_;
        std::map<std::string, std::vector<std::shared_ptr<option>>> groupMap_;
    };
}

#endif