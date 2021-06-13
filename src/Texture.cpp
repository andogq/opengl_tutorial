#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path):
    renderer_id(0),
    file_path(path),
    local_buffer(nullptr),
    width(0),
    height(0),
    bytes_per_pixel(0) {
    // Set the image to be flipped and load it
    stbi_set_flip_vertically_on_load(1);
    this->local_buffer = stbi_load(path.c_str(), &this->width, &this->height, &this->bytes_per_pixel, 4);
        
    // Initialise the texture
    gl_call(glGenTextures(1, &this->renderer_id));
    this->bind();

    // Set the parameters
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Load the tecture
    gl_call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->local_buffer));

    this->unbind();

    if (this->local_buffer) stbi_image_free(this->local_buffer);
}

Texture::~Texture() {
    gl_call(glDeleteTextures(1, &this->renderer_id));
}

void Texture::bind(unsigned int slot) const {
    gl_call(glActiveTexture(GL_TEXTURE0 + slot));
    gl_call(glBindTexture(GL_TEXTURE_2D, this->renderer_id));
}

void Texture::unbind() const {
    gl_call(glBindTexture(GL_TEXTURE_2D, 0));
}

int Texture::get_width() const {
    return this->width;
}

int Texture::get_height() const {
    return this->height;
}