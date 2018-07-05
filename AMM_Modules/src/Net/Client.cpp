#include "Client.h"

void Client::SetName(std::string &name) {
    if (name.size() > MAX_NAME_LENGTH) {
        name.resize(MAX_NAME_LENGTH);
    }
    this->name = name;
}

void Client::SetUUID(std::string &uuid) {
    this->uuid = uuid;
}

void Client::SetId(unsigned long id) {
    this->id = id;
}

void Client::SetKeepHistory(bool historyflag) {
    this->keepHistory = historyflag;
}
