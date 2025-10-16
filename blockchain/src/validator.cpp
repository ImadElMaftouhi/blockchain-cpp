#include "validator.h"
#include "utils.h"
#include <random>
#include <sstream>

Validator::Validator(std::string i, int s) : id(i), stake(s) {}

std::string Validator::selectValidator(const std::vector<Validator>& validators) {
    int totalStake = 0;
    for (const auto& v : validators) totalStake += v.stake;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, totalStake - 1);
    int random = dis(gen);
    int cumulative = 0;
    for (const auto& v : validators) {
        cumulative += v.stake;
        if (random < cumulative) return v.id;
    }
    return validators.back().id;
}

bool Validator::validateStake(const std::vector<Validator>& validators, const std::string& selectedId) {
    for (const auto& v : validators) {
        if (v.id == selectedId && v.stake > 0) return true;
    }
    return false;
}