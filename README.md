# Multi-Client-Chat-Application
This project implements a multi-client chat application using C++ with socket programming. It features a server and client architecture, enabling real-time communication between multiple clients in a local network. The system is built for simplicity and scalability, ensuring proper message handling and synchronization.

# Key Features
1. Server:

* Listens for incoming client connections on a predefined port (127.0.0.1:4444).
* Manages multiple clients concurrently using threads.
* Maintains a mapping of connected client names to their respective socket connections.
* Supports the following functionalities:
  Direct Messaging: Clients can send private messages to other connected users.
  Client List: Clients can request a list of all connected users.
  Graceful Disconnection: Handles client disconnection requests and server shutdown commands efficiently.

2. Client:

* Connects to the server and registers with a unique name.
* Allows real-time messaging between connected users.
* Displays incoming messages with proper formatting and synchronization using a mutex.
* Detects and handles server errors (e.g., name conflicts or server shutdown) gracefully.
* Enables users to type "exit" to disconnect.

3. Concurrency:

* The server uses threads to manage multiple client connections simultaneously.
* The client employs threading for listening to incoming messages while sending new ones.

4. Error Handling:

* Ensures that client names are unique. If a duplicate name is detected, the server notifies the client and disconnects.
* Provides meaningful error messages for undefined commands or invalid client requests.
* Notifies all clients upon server shutdown.
