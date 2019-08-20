C++ object wrapper for TCP/IP socket
==

This code can work on Linux and Windows.

## Usage

- Step 0. Setup Makefile

    There are arguments in **Makefile**
    
    - `VIDEO_STREAM`: set `1` to compile the video server/client, which use opencv to open webcam.

- Step 1. Compile the program
    
    Compile with **Makefile** by using following command in terminal
    ```bash
    $ make
    ```

- Step 2. Execute the server

    - Open a server socket to wait client come by using following command in terminal

        ```bash
        $ ./server.out
        ```
        or 
        ```bash
        $ python server.py
        ```

    - In image data case, use following instead

        ```bash
        $ ./video_server.out
        ```

- Step 3. Execute the client

    - Open a client socket to connect server by using following command in terminal

        ```bash
        $ ./client.out
        ```

    - In image data case, use following instead

        ```bash
        $ ./video_client.out
        ```

## Introduction

### IP Address and Port Number

- The default IP and port are `"127.0.0.1"` and `8700` respectively.

- To set the different IP and port, use `SocketPair::setServerIP` and `SocketPair::setPortNum` function.

### Setup Bytes Length

- Before send/recieve the data, please setup the data size `SocketPair::SendLen` and `SocketPair::RecvLen`.

### Wait Received Data

- If the data with large size like image, you should check the all bytes are recieved when recieve the data from partner. To enable this functionality, please set `SocketPair::waitRecvComplete = true`.