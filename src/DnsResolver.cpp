// DnsResolver.cpp
// Implementation of the DnsResolver class

#include "DnsResolver.h"
#include <future>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <winsock2.h>
#include <ws2tcpip.h>

/**
 * @brief Logs error messages to a file with a timestamp.
 * @param message The error message to log.
 */
void logError(const std::string& message) {
    std::ofstream logFile("dns_resolver.log", std::ios::app);
    if (logFile) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        logFile << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "] " << message << std::endl;
    }
}

/**
 * @brief Constructor that initializes WinSock for DNS resolution.
 */
DnsResolver::DnsResolver() : initialized(false) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
        initialized = true;
    } else {
        logError("Error: WSAStartup failed.");
    }
}

/**
 * @brief Destructor that cleans up WinSock resources.
 */
DnsResolver::~DnsResolver() {
    if (initialized) {
        WSACleanup();
    }
}

/**
 * @brief Resolves a hostname to a list of IP addresses.
 * @param hostname The domain name to resolve.
 * @return A vector of resolved IP addresses.
 */
std::vector<std::string> DnsResolver::resolve(const std::string& hostname) {
    std::vector<std::string> ipAddresses;
    if (!initialized) {
        logError("Error: WinSock not initialized.");
        return ipAddresses;
    }
    if (hostname.empty()) {
        logError("Error: Hostname is empty.");
        return ipAddresses;
    }

    struct addrinfo hints{}, *res = nullptr, *p = nullptr;
    char ipStr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(hostname.c_str(), nullptr, &hints, &res);
    if (status != 0) {
        logError("Error: " + std::string(gai_strerror(status)) + " (" + hostname + ")");
        return ipAddresses;
    }

    for (p = res; p != nullptr; p = p->ai_next) {
        void* addr;
        if (p->ai_family == AF_INET) {
            addr = &((struct sockaddr_in*)p->ai_addr)->sin_addr;
        } else if (p->ai_family == AF_INET6) {
            addr = &((struct sockaddr_in6*)p->ai_addr)->sin6_addr;
        } else {
            logError("Warning: Unknown address family.");
            continue;
        }

        if (!inet_ntop(p->ai_family, addr, ipStr, sizeof(ipStr))) {
            logError("Error: Failed to convert IP address.");
            continue;
        }
        ipAddresses.push_back(ipStr);
    }

    if (res) {
        freeaddrinfo(res);
    }

    if (ipAddresses.empty()) {
        logError("Error: No IP addresses resolved for " + hostname);
    }
    return ipAddresses;
}

/**
 * @brief Resolves a batch of hostnames using multithreading.
 * @param hostnames A vector of hostnames to resolve.
 * @return A vector of vectors containing resolved IP addresses for each hostname.
 */
std::vector<std::vector<std::string>> DnsResolver::resolveBatch(const std::vector<std::string>& hostnames) {
    if (hostnames.empty()) {
        logError("Error: No hostnames provided for batch resolution.");
        return {};
    }

    const size_t numThreads = std::thread::hardware_concurrency();
    std::vector<std::vector<std::string>> results(hostnames.size());
    std::mutex queueMutex;
    std::condition_variable cv;
    std::queue<size_t> tasks;
    std::atomic<bool> done{false};

    for (size_t i = 0; i < hostnames.size(); ++i) {
        tasks.push(i);
    }

    auto worker = [&]() {
        while (true) {
            size_t index;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                cv.wait(lock, [&] { return !tasks.empty() || done; });
                if (tasks.empty()) return;
                index = tasks.front();
                tasks.pop();
            }
            results[index] = resolve(hostnames[index]);
        }
    };

    std::vector<std::thread> workers;
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(worker);
    }

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        done = true;
    }
    cv.notify_all();

    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    return results;
}

/**
 * @brief Performs a reverse DNS lookup for a given IP address.
 * @param ipAddress The IP address to resolve.
 * @return The resolved hostname, or an empty string if not found.
 */
std::string DnsResolver::reverseResolve(const std::string& ipAddress) {
    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;
    char host[NI_MAXHOST];

    if (inet_pton(AF_INET, ipAddress.c_str(), &sa.sin_addr)) {
        sa.sin_family = AF_INET;
        if (getnameinfo((struct sockaddr*)&sa, sizeof(sa), host, NI_MAXHOST, nullptr, 0, NI_NAMEREQD) == 0) {
            return std::string(host);
        }
    } else if (inet_pton(AF_INET6, ipAddress.c_str(), &sa6.sin6_addr)) {
        sa6.sin6_family = AF_INET6;
        if (getnameinfo((struct sockaddr*)&sa6, sizeof(sa6), host, NI_MAXHOST, nullptr, 0, NI_NAMEREQD) == 0) {
            return std::string(host);
        }
    }

    logError("Error: Could not resolve IP " + ipAddress);
    return "";
}
