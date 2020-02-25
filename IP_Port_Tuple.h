class IP_Port_Tuple {
public:
    // server
    int port;
    String* ip_address;

    // Cannot be a network without Registration Server.
    IP_Port_Tuple(String* ip_address, int port) {
        this->port = port;
        this->ip_address = ip_address;
    }

    ~IP_Port_Tuple() {
        delete ip_address;
    }

    int getPort() {
        if (port != 0) {
            return this->port;
        } else {
            exit(1);
        }
    }

    String* getIP() {
        return this->ip_address;
    }

};