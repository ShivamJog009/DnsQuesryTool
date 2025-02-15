// main.cpp
// This file implements the command-line interface for the DNS Resolver application.
// It provides options for resolving domain names to IP addresses and performing reverse lookups.

#include "DnsResolver.h"
#include <iostream>
#include <fstream>
#include <vector>

/**
 * @brief Prints the usage instructions for the DNS Resolver CLI.
 */
void printUsage() {
    std::cout << "Usage:\n"
              << "  --resolve <hostname>          Resolve a single hostname to IP addresses\n"
              << "  --batch <file>                Resolve multiple hostnames from a file\n"
              << "  --reverse <ip_address>        Perform a reverse DNS lookup\n"
              << "  --help                        Show this message\n";
}

/**
 * @brief Entry point of the DNS Resolver CLI application.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Exit status code.
 */
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }

    DnsResolver resolver; // Initialize the DNS resolver
    std::string option = argv[1];

    if (option == "--resolve") {
        // Resolving a single hostname
        std::string hostname = argv[2];
        auto results = resolver.resolve(hostname);

        if (results.empty()) {
            std::cerr << "No IP addresses found for " << hostname << "\n";
            return 1;
        }

        std::cout << "Resolved IPs for " << hostname << ":\n";
        for (const auto& ip : results) {
            std::cout << "  " << ip << "\n";
        }
    }
    else if (option == "--batch") {
        // Resolving multiple hostnames from a file
        std::ifstream file(argv[2]);
        if (!file) {
            std::cerr << "Error: Cannot open file " << argv[2] << "\n";
            return 1;
        }

        std::vector<std::string> hostnames;
        std::string hostname;
        while (std::getline(file, hostname)) {
            if (!hostname.empty()) {
                hostnames.push_back(hostname);
            }
        }

        auto results = resolver.resolveBatch(hostnames);
        for (size_t i = 0; i < hostnames.size(); ++i) {
            std::cout << "Resolved IPs for " << hostnames[i] << ":\n";
            if (results[i].empty()) {
                std::cout << "  (No IP addresses found)\n";
            } else {
                for (const auto& ip : results[i]) {
                    std::cout << "  " << ip << "\n";
                }
            }
        }
    }
    else if (option == "--reverse") {
        // Performing a reverse DNS lookup
        std::string ipAddress = argv[2];
        std::string hostname = resolver.reverseResolve(ipAddress);

        if (hostname.empty()) {
            std::cerr << "No hostname found for IP address: " << ipAddress << "\n";
            return 1;
        }

        std::cout << "Resolved hostname for " << ipAddress << ": " << hostname << "\n";
    }
    else if (option == "--help") {
        printUsage();
    }
    else {
        std::cerr << "Unknown option: " << option << "\n";
        printUsage();
        return 1;
    }

    return 0;
}
