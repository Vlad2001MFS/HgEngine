#pragma once
#include "hd/Core/StringHash.hpp"
#include "hd/Core/Color.hpp"
#include "nlohmann/json.hpp"
#include "glm/glm.hpp"

namespace hg2d {

using JSONObject = nlohmann::json;

}

namespace hd {

void from_json(const hg2d::JSONObject &json, StringHash &data);
void from_json(const hg2d::JSONObject &json, Color4 &data);

void to_json(hg2d::JSONObject &json, const StringHash &data);
void to_json(hg2d::JSONObject &json, const Color4 &data);

}

namespace glm {

void to_json(hg2d::JSONObject &json, const vec2 &data);
void to_json(hg2d::JSONObject &json, const ivec2 &data);

void from_json(const hg2d::JSONObject &json, vec2 &data);
void from_json(const hg2d::JSONObject &json, ivec2 &data);

}