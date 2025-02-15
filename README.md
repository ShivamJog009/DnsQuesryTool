# DnsResolver

## Description
DnsResolver is a command-line tool for resolving hostnames to IP addresses and performing reverse DNS lookups. It provides both single hostname resolution and batch resolution from a file. The tool also supports reverse lookups to retrieve the hostname associated with an IP address.

## Features
- Resolve a single hostname to its IP addresses.
- Perform batch resolution from a list of hostnames in a file.
- Reverse lookup: Find the hostname associated with an IP address.
- Simple command-line interface for ease of use.

## Usage
After building the project, you can use the application with the following commands:

### Resolve a Single Hostname
```sh
DnsResolver --resolve <hostname>
```
Example:
```sh
DnsResolver --resolve example.com
```

### Batch Resolution from a File
```sh
DnsResolver --batch <file>
```
Example:
```sh
DnsResolver --batch hostnames.txt
```
Format of `hostnames.txt` (each hostname on a new line):
```
example.com
google.com
```

### Reverse Lookup (Find hostname from an IP address)
```sh
DnsResolver --reverse <ip-address>
```
Example:
```sh
DnsResolver --reverse 8.8.8.8
```

### Help
```sh
DnsResolver --help
```

## Building the Project
To build the project, use the following steps:

1. Navigate to the project directory.
2. Run the build command:
   ```sh
   g++ -o DnsResolver main.cpp DnsResolver.cpp -std=c++11 -Wall
   ```
3. The executable `DnsResolver` (or `DnsResolver.exe` on Windows) will be generated.

## Deployment
When deploying the project to GitHub:
- Upload all source files (`.cpp`, `.h` files) to the repository.
- Exclude the `build/` folder if it was used during compilation.
- Include a `bin/` folder to store the compiled executable (`DnsResolver.exe` for Windows or `DnsResolver` for Linux/macOS).
- Add this `README.md` file for documentation.

## License
This project is open-source and available under the MIT License.

