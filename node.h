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
    //Directory stuff
    //TODO put this in a directory?
    //the first one is the server
    size_t nodes;
    size_t * ports;  // owned
    String ** addresses;  // owned; strings owned

    //Personal Stuff
    String* server_addr;
    String* ip_addr;
    int port;

    int sock_listen;
    int sock_send:

     Node(String* server_addr, String* ip_addr, int port) {
        this.server_addr = server_addr;
        this.ip_addr = ip_addr;
        this.port = port;
        this.neighbors = new IPList();
        this.sock_listen = init(this->ip_addr->cstr_, this->port);
    }

    ~Node() {
        delete server_addr;
        delete ip_addr;
    }

    void send_reg() {
        // TODO Create Message {REGISTER}
        send_data(new Register(this.), new IP_Port_Tuple(server_addr, port))
    }

     void send_data(Message m, IP_Port_Tuple* ipNport) {
         sock_send = init(ipNport->getIP(), ipNport->getPort());
         send(sock_send , m, sizeof(m) , 0 );
         close(sock_send);
     }

     // Receives type Message -- has MsgKind field
     void handle_packet(char* buffer) {
         Register* r = dynamic_cast<Register>(m);
         char* args = strtok(buffer, " ");
        switch (args[0]) {
            case "Register":
                //TODO error?
                break;
            case "Ack":
                //TODO
                break;
            case "Status":
                //TODO
                break;
            case "Directory":
                size_t nodes = atoi(args[3]);
                for (int i = 4; i < 4 + nodes; i++) {
                    addresses[i - 4] == args[i];
                }
                for (int i = 4 + nodes; i < 4 + nodes + nodes; i++) {
                    ports[i - (4 + nodes)] == args[i];
                }
                break;
        }
     }

     String* get_address() {
         return this->ip_addr;
     }

     int init(const char* ip_address, int port) {

         struct sockaddr_in server_addr;
         char buffer[1024] = {0};
         printf("In client\n");
         printf("creating socket\n");
         int tmpSocket = 0;
         if ((tmpSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
         {
             printf("\n Socket creation error \n");
             return -1;
         }
         serv_addr.sin_family = AF_INET;
         serv_addr.sin_port = htons(port);
         // Convert IPv4 and IPv6 addresses from text to binary form
         if(inet_pton(AF_INET, ip_address, &serv_addr.sin_addr)<=0)
         {
             printf("\nInvalid address/ Address not supported");
             printf("%s", ip_addr);
             printf("\n");
             return -1;
         }
         if (connect(tmpSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
         {
             printf("\nConnection Failed \n");
             return -1;
         }
         return tmpSocket;
     }
};