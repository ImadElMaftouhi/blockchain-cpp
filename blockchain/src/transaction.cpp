#include "transaction.h"
#include "utils.h"
#include <sstream>

Transaction::Transaction(std::string i, std::string s, std::string r, double a)
    : id(i), sender(s), receiver(r), amount(a) {}

std::string Transaction::toString() const {
    std::stringstream ss;
    ss << id << sender << receiver << amount;
    return ss.str();
}

bool Transaction::validate() const {
    return !id.empty() && !sender.empty() && !receiver.empty() && amount >= 0;
}