//
// Created by kramt on 2/21/2020.
//

class Server {

    // Know its own ip address (master)
    // know all the other ip address of clients

    // send update clients of update address table returns true if updated addresses sent
    virtual bool send_update();

    // receive update from new client's to add to address table returns true if update received
    virtual bool recv_update();

    // receive data
    virtual Object* recv_data();

    // send data to specific address returns true if data sent
    virtual bool send_data(String* ip_addr, Object* data);

    // returns master address (this address)
    virtual String* get_master_addr();

    // returns all the addresses of the clients saved in the address table
    virtual Object** dump_table();

};