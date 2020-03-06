#include "JSONObject.hpp"

namespace hd {

void from_json(const hg::JSONObject &json, StringHash &data) {
    data = StringHash(json.get<uint64_t>());
}

void from_json(const hg::JSONObject &json, Color4 &data) {
    data.r = json["r"].get<uint8_t>();
    data.g = json["g"].get<uint8_t>();
    data.b = json["b"].get<uint8_t>();
    data.a = json["a"].get<uint8_t>();
}

void to_json(hg::JSONObject &json, const StringHash &data) {
    json = data.getHash();
}

void to_json(hg::JSONObject &json, const Color4 &data) {
    json["r"] = data.r;
    json["g"] = data.g;
    json["b"] = data.b;
    json["a"] = data.a;
}

}

namespace glm {

void to_json(hg::JSONObject &json, const vec2 &data) {
    json["x"] = data.x;
    json["y"] = data.y;
}

void to_json(hg::JSONObject &json, const ivec2 &data) {
    json["x"] = data.x;
    json["y"] = data.y;
}

void from_json(const hg::JSONObject &json, vec2 &data) {
    data.x = json.at("x");
    data.y = json.at("y");
}

void from_json(const hg::JSONObject &json, ivec2 &data) {
    data.x = json.at("x");
    data.y = json.at("y");
}

}
