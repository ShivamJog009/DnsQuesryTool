#ifndef DNSRESOLVER_H
#define DNSRESOLVER_H

#include <vector>
#include <string>

/**
 * @class DnsResolver
 * @brief A class for resolving domain names and performing reverse lookups.
 *
 * This class provides functionality to resolve hostnames to IP addresses,
 * perform reverse DNS lookups, and batch resolve multiple hostnames.
 */
class DnsResolver {
private:
    bool initialized; ///< Indicates whether the resolver has been initialized.

public:
    /**
     * @brief Constructor for DnsResolver.
     * Initializes any necessary resources.
     */
    DnsResolver();

    /**
     * @brief Destructor for DnsResolver.
     * Cleans up allocated resources.
     */
    ~DnsResolver();

    /**
     * @brief Resolves a given hostname to a list of IP addresses.
     * @param hostname The domain name to resolve.
     * @return A vector of resolved IP addresses.
     */
    std::vector<std::string> resolve(const std::string& hostname);

    /**
     * @brief Performs a reverse DNS lookup to get the hostname for an IP address.
     * @param ipAddress The IP address to look up.
     * @return The corresponding hostname, or an empty string if not found.
     */
    std::string reverseLookup(const std::string& ipAddress);

    /**
     * @brief Performs a reverse lookup with additional verification.
     * @param ipAddress The IP address to verify.
     * @return The resolved hostname if verification is successful, otherwise an empty string.
     */
    std::string reverseLookupWithVerification(const std::string& ipAddress);

    /**
     * @brief Resolves a batch of hostnames to their corresponding IP addresses.
     * @param hostnames A vector of hostnames to resolve.
     * @return A vector of vectors containing IP addresses for each hostname.
     */
    std::vector<std::vector<std::string>> resolveBatch(const std::vector<std::string>& hostnames);

    /**
     * @brief Performs a reverse DNS lookup for a given IP address.
     * @param ipAddress The IP address to resolve.
     * @return The resolved hostname, or an empty string if not found.
     */
    std::string reverseResolve(const std::string& ipAddress);
};

#endif
