C++ object wrapper for TCP/IP socket
==

This code can work on Linux and Windows.

## Usage

- Step 1. Compile the program
    
    Compile with **Makefile** by using following command in terminal
    ```
    $ make
    ```

- Step 2. Execute the server

    Open a server socket to wait client come by using following command in terminal
    ```
    $ ./server.out
    ```

- Step 3. Execute the client

    Open a client socket to connect server by using following command in terminal
    ```
    $ ./client.out
    ```

## Introduction

### IP Address and Port Number

- The default IP and port are `"127.0.0.1"` and `8700` respectively.

- To set the different IP and port, use `SocketPair::setServerIP` and `SocketPair::setPortNum` function.

### Setup Bytes Length

- Before send/recieve the data, please setup the data size `SocketPair::SendLen` and `SocketPair::RecvLen`.

### Wait Received Data