#pragma once
#include "hd/Core/Common.hpp"
#include "hd/Core/Log.hpp"
#include "hd/Core/StringUtils.hpp"
#include "nameof/nameof.hpp"
#include <map>
#include <functional>

#define HG2D_REGISTER_OBJECT(name, type) HD_RUN_BEFORE_MAIN() { hg2d::Factory::get().registerObject<type>(name); }
#define HG2D_CREATE_OBJECT(name, type) static_cast<type*>(hg2d::Factory::get().createObject(name))

namespace hg2d {

class Factory : public hd::Singleton<Factory> {
public:
    template<typename T>
    void registerObject(const std::string &name) {
        uint64_t typeHash = hd::StringUtils::getHash(name);
        if (!mCtors.count(typeHash)) {
            mCtors.insert(std::make_pair(typeHash, std::make_pair(NAMEOF_TYPE(T), []() { return static_cast<void*>(new T()); })));
        }
        else {
            LOG_F(WARNING, "Object '{}' with name '{}' already registered at factory", NAMEOF_TYPE(T), name);
        }
    }

    void *createObject(const std::string &name);

private:
    std::unordered_map<uint64_t, std::pair<std::string, std::function<void*()>>> mCtors;
};

inline Factory &getFactory() {
    return Factory::get();
}

}