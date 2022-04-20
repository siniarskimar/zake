#include "./options.hpp"
#include <algorithm>
#include <deque>
#include <cctype>
#include <exception>
#include <stdexcept>

namespace cmdline {
    options::options(
        const std::string& programName,
        const std::string& shortDesc)
    : programName_(programName), programDescription_(shortDesc) {}

    void options::add_option(
            const std::string& group,
            const std::string& name,
            const std::string& desc,
            basic_value* value) {
        std::string g = "Options";
        if(group != "") {
            g = group;
        }
        char shortName = '\0';
        auto it = name.find_first_of(',');
        if(it != std::string::npos) {
            if(name[0] != ',') {
                shortName = name[0];
            } else {
                shortName = '-';
            }
        } else {
            shortName = name[0];
        }
        std::string longName = "";
        it = name.find_last_of(',');
        if(it != std::string::npos) {
            longName = name.substr(it + 1, name.size() - it - 1);
        } else {
            longName = name;
        }
        std::unique_ptr<basic_value> v;
        v.reset(value);
        auto opt = std::make_shared<option>(shortName, longName, desc, std::move(v));
        options_.push_back(opt);
        groupMap_[g].emplace_back(opt);
    }

    void options::set_positional_options(const std::initializer_list<std::string>& il) {
        // Clear vector describing positionals
        positionals_.erase(positionals_.cbegin(), positionals_.cend());

        for(const auto& n : il) {
            // Find the option described
            auto opt = find_option(n);
            if(opt != nullptr) {
                // Add it to the positionals only if it's not a positional yet
                auto it = std::find(positionals_.begin(), positionals_.end(), opt);
                if(it == positionals_.end()) {
                    positionals_.emplace_back(opt);
                }
            }
        }
    }

    std::shared_ptr<option> options::find_option(const std::string& name) {
        auto it = std::find_if(options_.begin(), options_.end(),
                [name](std::shared_ptr<option> elem){
                    return elem->longName_ == name;
                });
        if(it != options_.end()) {
            return *it;
        }
        return nullptr;
    }

    std::shared_ptr<option> options::find_short_option(const char& name) {
        auto it = std::find_if(options_.begin(), options_.end(),
                [name](std::shared_ptr<option> elem){
                    return elem->shortName_ == name;
                });
        if(it != options_.end()) {
            return *it;
        }
        return nullptr;
    }

    const parse_result options::parse(int argc, const char* argv[]) {
        enum {POSITIONAL, VALUE, OPT_SHORT, OPT_LONG} state = POSITIONAL;
        parse_result result;
        std::deque<std::string> queue;
        std::shared_ptr<option> resOpt = nullptr;
        int i = 1;
        int positionalIndex = 0;
        bool terminated = false;

        while(i < argc || !queue.empty() || state == VALUE) {
            if(queue.empty() && i < argc) {
                queue.push_back(argv[i]);
                i++;
            }

            switch(state) {
                case POSITIONAL: {
                    auto& arg = queue.front();
                    if(arg[0] == '-' && !terminated) {
                        if(arg.size() <= 2 && arg[1] == '-') {
                            state = POSITIONAL;
                            terminated = true;
                            break;
                        }
                        if(arg[1] == '-') {
                            state = OPT_LONG;
                        } else {
                            state = OPT_SHORT;
                        }
                        break;
                    }

                    if(positionalIndex < positionals_.size()) {
                        resOpt = positionals_[positionalIndex];
                        positionalIndex++;
                        state = VALUE;
                        break;
                    }
                    queue.pop_front();
                    break;
                }
                case OPT_LONG: {
                    auto& arg = queue.front();

                    // check if we have to deal with equals sign
                    // if so, split argument into two
                    auto equalsIter = arg.find_first_of('=');
                    if(equalsIter != std::string::npos) {
                        auto opt = arg.substr(0, equalsIter);
                        auto val = arg.substr(equalsIter, equalsIter - arg.size());
                        queue.pop_front();
                        queue.push_back(opt);
                        queue.push_back(val);
                        break;
                    }

                    std::string name = arg.substr(2, arg.size() - 2);
                    queue.pop_front();

                    auto opt = find_option(name);
                    if(opt != nullptr) {
                        resOpt = opt;
                        if(opt->value_ == nullptr) {
                            result.insert(name, typed_value());
                            state = POSITIONAL;
                        } else {
                            state = VALUE;
                        }
                    }
                    break;
                }
                case OPT_SHORT: {
                    auto& arg = queue.front();
                    std::string name = arg.substr(1, arg.size() - 1);
                    queue.pop_front();

                    if(name.size() == 1) {
                        auto opt = find_short_option(name[0]);
                        if(opt != nullptr) {
                            resOpt = opt;
                            if(opt->value_ == nullptr) {
                                result.insert(opt->longName_, typed_value());
                                state = POSITIONAL;
                            } else {
                                state = VALUE;
                            }
                        }
                        break;
                    }

                    // make sure option with a value is last
                    std::shared_ptr<option> vopt = nullptr;
                    for(char sn : name) {
                        auto opt = find_short_option(sn);
                        if(opt != nullptr) {
                            resOpt = opt;
                            if(opt->value_ == nullptr) {
                                result.insert(opt->longName_, typed_value());
                                state = POSITIONAL;
                            } else {
                                if(vopt != nullptr) {
                                    throw std::runtime_error("two short options with values can not be joined together");
                                }
                                vopt = opt;
                            }
                        }
                    }

                    if(vopt != nullptr) {
                        resOpt = vopt;
                        state = VALUE;
                    }

                    break;
                }
                case VALUE: {
                    auto& arg = queue.front();
                    if(resOpt->value_ == nullptr) {
                        // Something is fudged up
                        throw std::runtime_error("internal logic error");
                    }
                    auto v = resOpt->value_->get_validator()->validate(arg);
                    auto defaultV = resOpt->value_->get_default_value();

                    if(v.type() == typeid(std::nullptr_t)) {
                        if(defaultV.type() != typeid(std::nullptr_t)) {
                            v = defaultV;
                        } else {
                            std::string error = "Couldn't parse argument value '" + arg + "'";
                            throw std::runtime_error(error);
                        }
                    }
                    result.insert(resOpt->longName_, typed_value(v));
                    state = POSITIONAL;
                    break;
                }
            }
        }

        return result;
    }

    std::string options::format_help() {
        return "This should be help text";
    }

    options::option_adder::option_adder(options* optDesc, const std::string& group)
        : optDesc_(optDesc), group_(group) {}
    
    options::option_adder& options::option_adder::operator()(
                const std::string& name,
                const std::string& desc,
                basic_value* value)
    {
        optDesc_->add_option(group_, name, desc, value);
        return *this;
    }
}