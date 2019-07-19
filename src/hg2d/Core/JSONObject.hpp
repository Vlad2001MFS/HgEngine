#pragma once
#include "../../3rd/include/json/json.hpp"
#include "3rd/include/glm/glm.hpp"

namespace hg2d {

using JSONObject = nlohmann::json;

}

namespace glm {

void to_json(hg2d::JSONObject &json, const vec2 &data);
void to_json(hg2d::JSONObject &json, const ivec2 &data);

void from_json(const hg2d::JSONObject &json, vec2 &data);
void from_json(const hg2d::JSONObject &json, ivec2 &data);

}