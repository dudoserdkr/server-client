# Server-Client
---

C/C++ course project: A simple chat room application.

## Build
---


To build the project, run the following command:

bash

Copy code

`$ bash make.sh`

The compiled executables `client` and `server` will be located in the `build/` directory.

## Usage
***

### Start the Server

To start the server, use:

bash

Copy code

`$ ./server <IP>:<PORT> BACKLOG`

- `<IP>`: The IP address on which the server will listen.
- `<PORT>`: The port number on which the server will listen.
- `BACKLOG`: The maximum number of clients the server can accept.

### Start the Client

To start a client, use:

bash

Copy code

`$ ./client <IP>:<PORT>`

- `<IP>`: The IP address of the server.
- `<PORT>`: The port number of the server.

### Sending Messages

After connecting, you can send messages to the server by typing them into the terminal where the **client** is running. All messages will be broadcast to other connected clients, appearing like this:

markdown

Copy code

`[Client 1]: Hello World`

### Changing Your Name

If you want to change your display name, type the command:

bash

Copy code

`/name New_Name`

After this command, your subsequent messages will be visible as:

markdown

Copy code

`[New_Name]: Hello World`

---

**Notes:**

- Make sure the server is running before starting any clients.
- All clients connected to the server will receive messages sent by any client.
- Use unique names to identify yourself easily in the chat room.
