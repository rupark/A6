#include "string.h"
#include "object.h"
#include <stdio.h>

enum class MsgKind { Ack, Nack, Put,
    Reply,  Get, WaitAndGet, Status,
    Kill,   Register,  Directory };

class Message : public Object {
    public:
        MsgKind kind_;  // the message kind
        size_t sender_; // the index of the sender node
        size_t target_; // the index of the receiver node
        //size_t id_;     // an id t unique within the node

        virtual String* serialize();
};

class Ack : public Message {
public:

    Ack(int sender, int target) {
        this->kind_ = Ack;
        this->sender_ = sender;
        this->target_ = target;
    }

    String* serialize() {
        char str[256] = ""; /* In fact not necessary as snprintf() adds the
                         0-terminator. */
        return new String(str);
    }

};

class Status : public Message {
public:
    String* msg_; // owned

    Status(int sender, int target, String* msg) {
        this->kind_ = Status;
        this->sender_ = sender;
        this->target_ = target;
        this->msg_ = msg;
    }

    String* serialize() {
        char str[256] = ""; /* In fact not necessary as snprintf() adds the
                         0-terminator. */
        snprintf(str, sizeof str, "%zu ", this->kind_);
        snprintf(str, sizeof str, "%zu ", this->sender_);
        snprintf(str, sizeof str, "%zu ", this->target_);
        snprintf(str, sizeof msg_, "%s", msg_->c_str());
        return new String(str);
    }
};

class Register : public Message {
public:
    sockaddr_in client;
    size_t port;

    Register(int sender, int target, sockaddr_in client, size_t port) {
        this->kind_ = Register;
        this->sender_ = sender;
        this->target_ = target;
        this->client = client;
        this->port = port;
    }

    String* serialize() {
        char str[256] = ""; /* In fact not necessary as snprintf() adds the
                         0-terminator. */
        snprintf(str, sizeof str, "%zu ", this->kind_);
        snprintf(str, sizeof str, "%zu ", this->sender_);
        snprintf(str, sizeof str, "%zu ", this->target_);
        snprintf(str, sizeof client, "%zu ", client);
        snprintf(str, sizeof port, "%zu", port);
        return new String(str);
    }
};

class Directory : public Message {
    size_t nodes;
    size_t * ports;  // owned
    String ** addresses;  // owned; strings owned

    Register(int sender, int target, size_t nodes, size_t * ports, String ** addresses) {
        this->kind_ = Register;
        this->sender_ = sender;
        this->target_ = target;
        this->nodes = nodes;
        this->ports = ports;
        this->addresses = addresses;
    }

    String* serialize() {
        char str[256] = ""; /* In fact not necessary as snprintf() adds the
                         0-terminator. */

        snprintf(str, sizeof str, "%zu ", this->kind_);
        snprintf(str, sizeof str, "%zu ", this->sender_);
        snprintf(str, sizeof str, "%zu ", this->target_);
        snprintf(str, sizeof str, "%zu ", this->nodes);
        for (int i = 0; i < nodes; i++) {
            snprintf(str, sizeof ports[i], "%zu ", ports[i]);
        }
        for (int i = 0; i < nodes; i++) {
            snprintf(str, sizeof ports[i], "%s ", addresses[i]->c_str());
        }
        return new String(str);
    }

};