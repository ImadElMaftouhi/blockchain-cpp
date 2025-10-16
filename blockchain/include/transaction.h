#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
public:
    std::string id;
    std::string sender;
    std::string receiver;
    double amount;

    Transaction(std::string i, std::string s, std::string r, double a);
    std::string toString() const;
    bool validate() const;
};

#endif