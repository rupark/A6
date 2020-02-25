#include "serialize.h"
#include "list.h"
#include "string.h"

public Node {
    // know own address
    // know HOST ADDRESS
    // table of other known client addresses

    // constructor creates socket...
        // sends update to server of this address
public:
    IPList* neighbors;
    String* master_addr;
    String* ip_addr;
    int port;

     Node(String* master_addr, String* ip_addr, int port) {
        this.master_addr = master_addr;
        this.ip_addr = ip_addr;
        this.port = port;
        this.neighbors = new IPList();
    }

    ~Node() {
        delete master_addr;
        delete ip_addr;
    }

    void send_reg() {

     }

     void send_data(Message m, IP_Port_Tuple* ipNport) {

     }

     // Data that is received on this port (Could be Update or Data)
     // Receives type Message -- has MsgKind field
     void handle_packet(Message* m) {

     }

     String* get_address() {
         return this->ip_addr;
     }

    virtual IPList* dump_table() {
         return this->neighbors;
     }
};