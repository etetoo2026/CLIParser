#include "../cliparser.hpp"
#include <cassert>
#include <iostream>

void test_flag() {
    char* args[] = {(char*)"prog", (char*)"-v"};
    cli::Parser p("prog", "test");
    p.add_flag("verbose", 'v', "verbose");
    p.parse(2, args);
    assert(p.get<bool>("verbose") == true);
    std::cout << "PASS test_flag\n";
}

void test_option_default() {
    char* args[] = {(char*)"prog"};
    cli::Parser p("prog", "test");
    p.add_option<int>("count", 'n', "count", std::optional<int>(42));
    p.parse(1, args);
    assert(p.get<int>("count") == 42);
    std::cout << "PASS test_option_default\n";
}

void test_option_override() {
    char* args[] = {(char*)"prog", (char*)"--count", (char*)"99"};
    cli::Parser p("prog", "test");
    p.add_option<int>("count", 'n', "count", std::optional<int>(1));
    p.parse(3, args);
    assert(p.get<int>("count") == 99);
    std::cout << "PASS test_option_override\n";
}

void test_positional() {
    char* args[] = {(char*)"prog", (char*)"input.txt"};
    cli::Parser p("prog", "test");
    p.add_positional("input", "input file");
    p.parse(2, args);
    assert(p.get<std::string>("input") == "input.txt");
    std::cout << "PASS test_positional\n";
}

void test_short_option() {
    char* args[] = {(char*)"prog", (char*)"-n", (char*)"7"};
    cli::Parser p("prog", "test");
    p.add_option<int>("count", 'n', "count");
    p.parse(3, args);
    assert(p.get<int>("count") == 7);
    std::cout << "PASS test_short_option\n";
}

int main() {
    test_flag();
    test_option_default();
    test_option_override();
    test_positional();
    test_short_option();
    std::cout << "All tests passed.\n";
    return 0;
}
