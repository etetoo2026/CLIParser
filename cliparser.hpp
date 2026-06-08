#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <functional>
#include <optional>

namespace cli {

struct Option {
    std::string name;
    char shortName;
    std::string description;
    std::string defaultValue;
    std::string typeName;
    bool isFlag   = false;
    bool required = false;
    bool positional = false;
};

class Parser {
public:
    Parser(std::string prog, std::string desc)
        : prog_(std::move(prog)), desc_(std::move(desc)) {}

    void add_flag(const std::string& name, char shortName, const std::string& desc) {
        options_.push_back({name, shortName, desc, "false", "bool", true});
    }

    template<typename T>
    void add_option(const std::string& name, char shortName, const std::string& desc,
                    std::optional<T> def = std::nullopt, bool required = false) {
        std::string defStr = def ? to_str(*def) : "";
        options_.push_back({name, shortName, desc, defStr, type_name<T>(), false, required, false});
    }

    void add_positional(const std::string& name, const std::string& desc, bool required = true) {
        options_.push_back({name, '\0', desc, "", "string", false, required, true});
    }

    void parse(int argc, char** argv) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "--help" || arg == "-h") { print_help(); exit(0); }

            if (arg.substr(0, 2) == "--") {
                std::string key = arg.substr(2);
                auto* opt = find(key);
                if (!opt) throw std::runtime_error("Unknown option: --" + key);
                if (opt->isFlag) { values_[key] = "true"; }
                else if (i + 1 < argc) { values_[key] = argv[++i]; }
                else throw std::runtime_error("Option --" + key + " requires a value");
            } else if (arg[0] == '-' && arg.size() == 2) {
                auto* opt = findShort(arg[1]);
                if (!opt) throw std::runtime_error(std::string("Unknown flag: ") + arg[1]);
                if (opt->isFlag) { values_[opt->name] = "true"; }
                else if (i + 1 < argc) { values_[opt->name] = argv[++i]; }
                else throw std::runtime_error(std::string("Option -") + arg[1] + " requires a value");
            } else {
                positionals_.push_back(arg);
            }
        }

        size_t posIdx = 0;
        for (auto& opt : options_) {
            if (opt.positional && posIdx < positionals_.size())
                values_[opt.name] = positionals_[posIdx++];
            if (!opt.defaultValue.empty() && values_.find(opt.name) == values_.end())
                values_[opt.name] = opt.defaultValue;
            if (opt.required && values_.find(opt.name) == values_.end())
                throw std::runtime_error("Required argument missing: " + opt.name);
        }
    }

    template<typename T>
    T get(const std::string& name) const {
        auto it = values_.find(name);
        if (it == values_.end()) return T{};
        return from_str<T>(it->second);
    }

    void print_help() const {
        std::cout << "Usage: " << prog_ << " [OPTIONS]";
        for (auto& o : options_) if (o.positional) std::cout << " <" << o.name << ">";
        std::cout << "\n\n" << desc_ << "\n\nOptions:\n";
        for (auto& o : options_) {
            if (o.positional) continue;
            std::string left = (o.shortName ? std::string("  -") + o.shortName + ", " : "      ");
            left += "--" + o.name;
            if (!o.isFlag) left += " <" + o.typeName + ">";
            std::cout << left;
            int pad = 30 - (int)left.size();
            if (pad > 0) std::cout << std::string(pad, ' ');
            std::cout << o.description;
            if (!o.defaultValue.empty()) std::cout << " (default: " << o.defaultValue << ")";
            std::cout << "\n";
        }
        std::cout << "      --help, -h              Show this help message\n";
    }

private:
    std::string prog_, desc_;
    std::vector<Option> options_;
    std::unordered_map<std::string, std::string> values_;
    std::vector<std::string> positionals_;

    Option* find(const std::string& name) {
        for (auto& o : options_) if (o.name == name) return &o;
        return nullptr;
    }
    Option* findShort(char c) {
        for (auto& o : options_) if (o.shortName == c) return &o;
        return nullptr;
    }

    template<typename T> static std::string to_str(const T& v) {
        std::ostringstream ss; ss << v; return ss.str();
    }
    template<typename T> static T from_str(const std::string& s) {
        std::istringstream ss(s); T v{}; ss >> v; return v;
    }
    template<typename T> static std::string type_name() { return "value"; }
};

template<> inline std::string Parser::from_str<std::string>(const std::string& s) { return s; }
template<> inline bool Parser::from_str<bool>(const std::string& s) { return s == "true" || s == "1"; }
template<> inline std::string Parser::type_name<int>()         { return "int"; }
template<> inline std::string Parser::type_name<float>()       { return "float"; }
template<> inline std::string Parser::type_name<std::string>() { return "str"; }

} // namespace cli
