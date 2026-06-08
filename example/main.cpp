#include "../cliparser.hpp"
#include <iostream>

int main(int argc, char** argv) {
    cli::Parser p("myapp", "Example app using CLIParser");
    p.add_flag("verbose", 'v', "Enable verbose output");
    p.add_option<int>("count", 'n', "Number of items", std::optional<int>(10));
    p.add_option<std::string>("output", 'o', "Output file path");
    p.add_positional("input", "Input file path");

    try {
        p.parse(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        p.print_help();
        return 1;
    }

    bool verbose = p.get<bool>("verbose");
    int count    = p.get<int>("count");
    std::string input  = p.get<std::string>("input");
    std::string output = p.get<std::string>("output");

    if (verbose) std::cout << "[verbose] count=" << count << "\n";
    std::cout << "Input:  " << input  << "\n";
    std::cout << "Output: " << (output.empty() ? "(stdout)" : output) << "\n";
    std::cout << "Count:  " << count  << "\n";
    return 0;
}
