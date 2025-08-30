# LaTeX Task Generator

A hybrid C++/Python system for auto-generating practice problem sets from Codeforces in LaTeX format.

## Features

- **Tag-Based Selection:** Specify topics (e.g., `graphs`, `greedy`) to get a curated list of problems.
- **Randomized Pick:** Randomly selects a user-defined number of problems from the available pool.
- **Automatic Content Fetching:** Retrieves problem statements directly from Codeforces.
- **LaTeX Output:** Generates a clean, ready-to-compile `.tex` file.

## How It Works

1.  The C++ client requests problems from the **Codeforces API** based on input tags.
2.  It selects random problems from the JSON response.
3.  Using **gRPC**, it sends each problem's URL to a Python service.
4.  The Python service fetches the HTML page, parses it, and extracts key components (title, statement, examples).
5.  The parsed data is serialized using **Protocol Buffers** and sent back to the C++ client.
6.  The client converts the text into LaTeX-friendly format and writes the final output file.

## Tech Stack

- **Languages:** C++17, Python 3
- **Communication:** gRPC
- **Data Serialization:** Protocol Buffers
- **Web API:** Codeforces API (HTTP/JSON)
- **HTML Parsing:** BeautifulSoup4 (Python)
- **JSON Processing:** nlohmann/json
- **HTTP Client:** libcurl

## Build & Installation

### Prerequisites
- C++17 compatible compiler (GCC, Clang)
- CMake (version 3.10+)
- Python 3.x with pip
- gRPC, protobuf, curl, nlohmann/json

### Python Dependencies
Install required Python packages:
```bash
pip install -r py_requirements.txt
./generate_python_proto.sh
```

### Building the C++ Client

1.  **Create build directory and run CMake:**
    ```bash
    mkdir build && cd build
    cmake ..
    ```
2.  **Compile the project:**
    ```bash
    make -j$(nproc)
    ```
    Or just `make`

### Running the System

1.  **Start the Python gRPC Server** (in a separate terminal):
    ```bash
    python3 ./server/py_server.py
    ```

2.  **Run the C++ Client** (in another terminal from the project root or `build/` directory):
    ```bash
    # From project root (if binary is in build/bin/)
    ./build/bin/codeforces_client [tags]
    
    # Example with specific tags:
    ./build/bin/codeforcses_client dp math graphs
    ```

## Usage Example

```bash
# Terminal 1 - Start the server
$ python3 server/py_server.py
Server started on port 50051...

# Terminal 2 - Generate a problem set
$ ./build/bin/codef dp math
Response: OK
```
```