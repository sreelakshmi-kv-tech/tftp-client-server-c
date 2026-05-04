# TFTP Client-Server Implementation (C)

## 📌 Abstract
Trivial File Transfer Protocol (TFTP) is a simple file transfer protocol defined in RFC 1350. This project implements a client-server file transfer system using socket programming in C.

## 🚀 Features
- Supports Read Request (RRQ) and Write Request (WRQ)
- File transfer in 512-byte data blocks
- Acknowledgment (ACK) mechanism for reliability
- Error handling (file not found, access violation, disk full, etc.)
- Retransmission for packet loss handling
- Supports modes: netascii and octet (binary)

## 🛠️ Technologies Used
- C Programming
- Socket Programming
- UDP Protocol
- Linux System Calls

## ⚙️ System Design
- Client sends RRQ/WRQ requests to server
- Server handles requests using separate processes (fork)
- Data packets transferred with sequence numbers
- Transfer ends when final data packet (<512 bytes) is acknowledged

## 📡 Packet Types
- 1: RRQ (Read Request)
- 2: WRQ (Write Request)
- 3: DATA
- 4: ACK
- 5: ERROR

## ❗ Error Handling
- File not found
- Access violation
- Disk full
- Illegal operation
- Unknown transfer ID
- File already exists

## ▶️ How to Run

### Compile:
