#include "Font.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/Core/Log.hpp"

namespace hg {

Font::Font(TTF_Font *font) : mFont(font) {
}

Font::~Font() {
    TTF_CloseFont(mFont);
    TTF_Quit();
}

hd::Image Font::renderLine(const std::string &text, const glm::vec4 &color) const {
    if (text.empty()) {
        HD_LOG_WARNING("Rendering empty text line to texture");
    }
    if (color.a == 0) {
        HD_LOG_WARNING("Rendering fully transparent text line to texture");
    }

    return mRenderLine(text, color);
}

hd::Image Font::renderText(const std::string &text, const glm::vec4 &color) const {
    if (text.empty()) {
        HD_LOG_WARNING("Rendering empty text to texture");
    }
    if (color.a == 0) {
        HD_LOG_WARNING("Rendering fully transparent text to texture");
    }

    std::vector<std::string> lines = hd::StringUtils::split(text, "\n", false);
    auto maxLine = std::max_element(lines.begin(), lines.end(), [&](const std::string &a, const std::string &b){
        return getTextWidth(a) < getTextWidth(b);
    });
    size_t width = getTextWidth(*maxLine);
    size_t height = lines.size()*getLineHeight();

    hd::Image img(nullptr, glm::ivec2(width, height), hd::ImageFormat::RGBA);
    for (uint32_t i = 0; i < lines.size(); i++) {
        mRenderLineTo(img, 0, i*getLineHeight(), lines[i], color);
    }
    return img;
}

void Font::setStyle(FontStyle style) {
	TTF_SetFontStyle(mFont, static_cast<int>(style));
}

void Font::setOutline(uint32_t outline) {
	TTF_SetFontOutline(mFont, static_cast<int>(outline));
}

void Font::setHinting(FontHinting hinting) {
	TTF_SetFontHinting(mFont, static_cast<int>(hinting));
}

void Font::setKerning(bool kerning) {
	TTF_SetFontKerning(mFont, kerning);
}

uint32_t Font::getMaxGlyphHeight() const {
    return static_cast<uint32_t>(TTF_FontHeight(mFont));
}

uint32_t Font::getLineHeight() const {
    return static_cast<uint32_t>(TTF_FontLineSkip(mFont));
}

uint32_t Font::getTextWidth(const std::string &text) const {
    int width;
	TTF_SizeUTF8(mFont, text.data(), &width, nullptr);
    return static_cast<uint32_t>(width);
}

FontPtr Font::createFromFile(const std::string &path, uint32_t size) {
    if (TTF_Init() < 0) {
        HD_LOG_ERROR("Failed to initialize SDL_ttf. Error: {}", TTF_GetError());
    }

    std::string fullPath = mGetFullPath(path);
    TTF_Font *font = TTF_OpenFont(fullPath.data(), static_cast<int>(size));
    if (!font) {
        HD_LOG_ERROR("Failed to load font '{}'. Errors: {}", path, TTF_GetError());
    }
    return std::make_shared<Font>(font);
}

hd::Image Font::mRenderLine(const std::string &text, const glm::vec4 &color) const {
    SDL_Color sdlColor;
    sdlColor.r = static_cast<uint8_t>(glm::clamp(color.r*255.0f, 0.0f, 255.0f));
    sdlColor.g = static_cast<uint8_t>(glm::clamp(color.g*255.0f, 0.0f, 255.0f));
    sdlColor.b = static_cast<uint8_t>(glm::clamp(color.b*255.0f, 0.0f, 255.0f));
    sdlColor.a = static_cast<uint8_t>(glm::clamp(color.a*255.0f, 0.0f, 255.0f));
    SDL_Surface *surface = TTF_RenderUTF8_Blended(mFont, text.data(), sdlColor);
    if (!surface) {
        TTF_CloseFont(mFont);
        HD_LOG_ERROR("Failed to render line '{}'. Error: {}", text, TTF_GetError());
    }
    if (surface->format->format != SDL_PIXELFORMAT_RGBA32) {
        SDL_Surface *old = surface;
        surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(old);
    }

    hd::Image img(surface->pixels, glm::ivec2(surface->w, surface->h), hd::ImageFormat::RGBA);
    SDL_FreeSurface(surface);

    /*glm::u8vec4 bg = static_cast<glm::u8vec4*>(img.getData())[0];
    glm::u8vec4 newBg = glm::u8vec4(0, 0, 0, 0);
    for (int i = 0; i < img.getSize().x*img.getSize().y; i++) {
        glm::u8vec4 &pixel = static_cast<glm::u8vec4*>(img.getData())[i];
        if (pixel.r == bg.r && pixel.g == bg.g && pixel.b == bg.b) {
            pixel = newBg;
        }
        else if (pixel.a == 0) {
            pixel = color;
        }
    }
    glm::u8vec4 stokeColor = glm::u8vec4(0, 0, 0, 160);
    for (int y = 0; y < img.getSize().y; y++) {
        for (int x = 0; x < img.getSize().x; x++) {
            glm::u8vec4 pixel = static_cast<glm::u8vec4*>(img.getData())[x + y*img.getSize().x];
            if (pixel.r == color.r && pixel.g == color.g && pixel.b == color.b) {
                for (int ny = -1; ny <= 1; ny++) {
                    for (int nx = -1; nx <= 1; nx++) {
                        glm::u8vec4 pixel2 = static_cast<glm::u8vec4*>(img.getData())[(x + nx) + (y + ny)*img.getSize().x];
                        if (pixel2.r != color.r || pixel2.g != color.g || pixel2.b != color.b) {
                            static_cast<glm::u8vec4*>(img.getData())[(x + nx) + (y + ny)*img.getSize().x] = stokeColor;
                        }
                    }
                }
            }
        }
    }*/
    return img;
}

void Font::mRenderLineTo(hd::Image &target, uint32_t uOffset, uint32_t vOffset, const std::string &text, const glm::vec4 &color) const {
    hd::Image img = mRenderLine(text, color);
    for (int y = 0; y < img.getSize().y; y++) {
        const glm::u8vec4 *line = static_cast<const glm::u8vec4*>(img.getData()) + y*img.getSize().x;
        int baseOffset = uOffset + vOffset*target.getSize().x;
        int offset = baseOffset + y*target.getSize().x;
        int size = img.getSize().x;
        memcpy(static_cast<glm::u8vec4*>(target.getData()) + static_cast<size_t>(offset), line, sizeof(glm::u8vec4)*static_cast<size_t>(size));
    }
}

std::string Font::mGetFullPath(const std::string &path) {
    return "./data/" + path;
}

}
