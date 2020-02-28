#include "Factory.hpp"

void *hg2d::Factory::createObject(const std::string &name) {
    auto it = mCtors.find(hd::StringUtils::getHash(name));
    if (it != mCtors.end()) {
        LOG_F(INFO, "Object '{}' with name '{}' created by factory", it->second.first, name);
        return it->second.second();
    }
    else {
        LOG_F(WARNING, "Object '{}' with name '{}' not registered at factory", it->second.first, name);
        return nullptr;
    }
}
