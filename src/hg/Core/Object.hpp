#pragma once
#include "hd/Core/Common.hpp"
#include "hd/Core/Log.hpp"
#include "hd/Core/StringHash.hpp"
#include "../../nameof/nameof.hpp"
#include <functional>

#define HG_CREATE_OBJECT(nameOrHash, type) hg::Factory::get().createObject(nameOrHash)->as<type>()
#define HG_OBJECT(typeName, baseTypeName) \
    public: \
        using ClassName = typeName; \
        using BaseClassName = baseTypeName; \
        virtual const hd::StringHash &getTypeHash() const override { return getTypeInfoStatic()->getTypeHash(); } \
        virtual const std::string &getTypeName() const override { return getTypeInfoStatic()->getTypeName(); } \
        virtual const hg::TypeInfo *getTypeInfo() const override { return getTypeInfoStatic(); } \
        static const hd::StringHash &getTypeHashStatic() { return getTypeInfoStatic()->getTypeHash(); } \
        static const std::string &getTypeNameStatic() { return getTypeInfoStatic()->getTypeName(); } \
        static const hg::TypeInfo *getTypeInfoStatic() { \
            static const hg::TypeInfo typeInfoStatic(std::string(NAMEOF_TYPE(typeName)), BaseClassName::getTypeInfoStatic()); \
            return &typeInfoStatic; \
        } \
        static void __attribute__((constructor)) HD_CONCAT(__register_object_, HD_CONCAT(typeName, HD_CONCAT(_, baseTypeName)))() { \
            hg::Factory::get().registerObject<typeName>(); \
        }

namespace hg {

class TypeInfo {
public:
    TypeInfo(const std::string &typeName, const TypeInfo *baseTypeInfo);

    bool isTypeOf(const hd::StringHash &typeHash) const;
    bool isTypeOf(const TypeInfo *typeInfo) const;

    template<typename T>
    bool isTypeOf() const {
        return isTypeOf(T::getTypeInfoStatic());
    }

    const hd::StringHash &getTypeHash() const;
    const std::string &getTypeName() const;
    const TypeInfo *getBaseTypeInfo() const;

private:
    hd::StringHash mTypeHash;
    std::string mTypeName;
    const TypeInfo* mBaseClassInfo;
};

class Object : public hd::Noncopyable {
public:
    Object() = default;
    virtual ~Object() = default;

    virtual const hd::StringHash &getTypeHash() const = 0;
    virtual const std::string &getTypeName() const = 0;
    virtual const TypeInfo *getTypeInfo() const = 0;

    static const TypeInfo *getTypeInfoStatic();
    bool isInstanceOf(const hd::StringHash &typeHash) const;
    bool isInstanceOf(const TypeInfo *typeInfo) const;

    template<typename T>
    bool isInstanceOf() const {
        return isInstanceOf(T::getTypeInfoStatic());
    }

    template<typename T>
    T *as() {
        T *ptr = isInstanceOf<T>() ? static_cast<T*>(this) : nullptr;
        if (!ptr) {
            HD_LOG_WARNING("Failed to cast object '{}' to '{}'", getTypeName(), T::getTypeNameStatic());
        }
        return ptr;
    }

    template<typename T>
    const T *as() const {
        T *ptr = isInstanceOf<T>() ? static_cast<T*>(this) : nullptr;
        if (!ptr) {
            HD_LOG_WARNING("Failed to cast object '{}' to '{}'", getTypeName(), T::getTypeNameStatic());
        }
        return ptr;
    }
};

class Factory : public hd::Singleton<Factory> {
public:
    template<typename T>
    void registerObject() {
        if (!mCreateFunctions.count(T::getTypeHashStatic())) {
            mCreateFunctions.insert(std::make_pair(T::getTypeHashStatic(), []() -> Object* { return new T(); }));
            HD_LOG_INFO("Object '{}' registered at factory", T::getTypeNameStatic());
        }
    }

    Object *createObject(const std::string &name);
    Object *createObject(const hd::StringHash &typeHash);

private:
    std::unordered_map<hd::StringHash, std::function<Object*()>> mCreateFunctions;
};

inline Factory &getFactory() {
    return Factory::get();
}

}
