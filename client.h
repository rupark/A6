
public Client {
    // know own address
    // know HOST ADDRESS
    // table of other known client addresses

    // constructor creates socket...
        // sends update to server of this address

    // receive data
    virtual bool recv_data();

    // send data to specific location
    virtual bool send_data(String* ip_addr, size_t port);

    // receive update from host of new client address
    virtual bool recv_update();

    // send update to host of own address
    virtual bool send_data(String* ip_addr, Object* data);

    virtual Object** dump_table();
};