#include "Object.hpp"

namespace hg {

TypeInfo::TypeInfo(const std::string &typeName, const TypeInfo *baseTypeInfo) : mTypeName(typeName) {
    mTypeHash = hd::StringHash(typeName);
    mBaseClassInfo = baseTypeInfo;
}

bool TypeInfo::isTypeOf(const hd::StringHash &typeHash) const {
    const TypeInfo *currentTypeInfo = this;
    while (currentTypeInfo) {
        if (currentTypeInfo->getTypeHash() == typeHash) {
            return true;
        }
        currentTypeInfo = currentTypeInfo->getBaseTypeInfo();
    }
    return false;
}

bool TypeInfo::isTypeOf(const TypeInfo *typeInfo) const {
    if (!typeInfo) {
        return false;
    }

    const TypeInfo *currentTypeInfo = this;
    while (currentTypeInfo) {
        if (currentTypeInfo == typeInfo || currentTypeInfo->getTypeHash() == typeInfo->getTypeHash()) {
            return true;
        }
        currentTypeInfo = currentTypeInfo->getBaseTypeInfo();
    }
    return false;
}

const hd::StringHash &TypeInfo::getTypeHash() const {
    return mTypeHash;
}

const std::string &TypeInfo::getTypeName() const {
    return mTypeName;
}

const TypeInfo *TypeInfo::getBaseTypeInfo() const {
    return mBaseClassInfo;
}

const TypeInfo *Object::getTypeInfoStatic() {
    return nullptr;
}

bool Object::isInstanceOf(const hd::StringHash &typeHash) const {
    return getTypeInfo()->isTypeOf(typeHash);
}

bool Object::isInstanceOf(const TypeInfo* typeInfo) const {
    return getTypeInfo()->isTypeOf(typeInfo);
}

Object *hg::Factory::createObject(const std::string &name) {
    return createObject(hd::StringHash(name));
}

Object *Factory::createObject(const hd::StringHash &typeHash) {
    auto it = mCreateFunctions.find(typeHash);
    if (it != mCreateFunctions.end()) {
        HD_LOG_INFO("Object '{}' created by factory", typeHash.getString());
        return it->second();
    }
    else {
        HD_LOG_WARNING("Object '{}' not registered at factory", typeHash.getString());
        return nullptr;
    }
}

}
