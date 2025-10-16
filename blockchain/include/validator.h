#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <vector>

class Validator {
public:
    std::string id;
    int stake;

    Validator(std::string i, int s);

    // Validator selection and validation methods
    static std::string selectValidator(const std::vector<Validator>& validators);
    static bool validateStake(const std::vector<Validator>& validators, const std::string& selectedId);
};

#endif