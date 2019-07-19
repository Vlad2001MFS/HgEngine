#include "JSONObject.hpp"

namespace glm {

void to_json(hg2d::JSONObject &json, const vec2 &data) {
    json["x"] = data.x;
    json["y"] = data.y;
}

void to_json(hg2d::JSONObject &json, const ivec2 &data) {
    json["x"] = data.x;
    json["y"] = data.y;
}

void from_json(const hg2d::JSONObject &json, vec2 &data) {
    data.x = json.at("x");
    data.y = json.at("y");
}

void from_json(const hg2d::JSONObject &json, ivec2 &data) {
    data.x = json.at("x");
    data.y = json.at("y");
}

}