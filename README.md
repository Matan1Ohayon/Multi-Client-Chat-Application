### 💬 Multi-Client Chat Application - C++ Socket Programming
A console-based chat system built in C++ using TCP sockets.  
The project implements a real-time client–server model where multiple clients can connect and send messages to each other.

---

### ✨ Features
- 🔌 **TCP Client–Server Architecture** - central server managing all connections  
- 👥 **Multiple Clients** - each client handled in its own detached thread  
- 🧵 **Multithreading** - smooth parallel communication using `std::thread`  
- 💬 **Direct Messaging** - send messages to specific users using the `send <name> <message>` command  
- 📜 **User Listing** - view connected users with `list`  
- 🚫 **Error Handling** - duplicate names, user not found, undefined commands  
- 🛑 **Graceful Disconnects** - clients can exit without affecting others  
- ⚙️ **Admin Exit Command** - server can shut down cleanly using `exit`

---

### 🛠️ Tech Stack
- C++  
- Linux Socket API (`socket`, `bind`, `listen`, `accept`, `recv`, `send`)  
- Multithreading (`std::thread`)  
- Data structures (`map`, `vector`, `istringstream`)  
- GCC / g++ compiler  

---

### ▶️ How to Run

#### 1️⃣ Compile the Server
```bash
g++ server.cpp -o server -pthread
```
Run:
```bash
./server
```
#### 2️⃣ Compile the Client
```bash
g++ client.cpp -o client
```
Run:
```bash
./client
```
You can run multiple client windows at the same time - all connect to:
```bash
127.0.0.1 : 4444
```

---

### 💡 Available Commands (Client Side)

| Command | Description |
|--------|-------------|
| `send <name> <message>` | Sends a direct message to another connected client |
| `list` | Shows all connected clients except yourself |
| `exit` | Disconnects from the server |

---

### 📁 Project Structure
- server.cpp            # Server logic: threads, clients map, routing messages
- client.cpp            # Client logic: connecting, sending commands, receiving messages
- std_lib_facilities.h  # Utilities header used by the project


---

### 🚀 What This Project Demonstrates
- Understanding of TCP/IP networking  
- Practical socket programming in C++  
- Real-time message routing between clients  
- Safe multithreading with detached threads  
- Error handling and command parsing  
- Building and structuring small networked systems  

---

### 📬 Contact
- 🌐 Portfolio: https://matans-portfolio.vercel.app/  
- 💼 LinkedIn: www.linkedin.com/in/matan-ohayon-4101b6276  
- 📧 Email: matan1ohayon@gmail.com



