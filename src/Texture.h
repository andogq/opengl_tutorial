#pragma once

#include <string>
#include <GL/glew.h>

#include "error.h"
#include "vendor/stb_image/stb_image.h"

class Texture {
    private:
        unsigned int renderer_id;
        std::string file_path;
        unsigned char* local_buffer;
        int width;
        int height;
        int bytes_per_pixel;

    public:
        Texture(const std::string& path);
        ~Texture();
        
        void bind(unsigned int slot = 0) const;
        void unbind() const;

        int get_width() const;
        int get_height() const;
};