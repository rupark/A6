class IP_Port_Tuple {
public:
    // server
    String* port;
    String* ip_address;

    // Cannot be a network without Registration Server.
    IP_Port_Tuple(String* port, String* ip_address) {
        this->port = port;
        this->ip_address = ip_address;
    }

    ~IP_Port_Tuple() {
        delete port;
        delete ip_address;
    }

    String* getPort() {
        if (port != nullptr) {
            return this->port;
        } else {
            exit(1);
        }
    }

    String* getIP() {
        return this->ip_address;
    }

};