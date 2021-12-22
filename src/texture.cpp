#include <texture.hpp>

#include <GLAD/glad.h>
#include <stb_image.h>

#include <stdio.h>

Texture::Texture(std::string path, std::string type) {
	int height, width, nChannels;
	unsigned int texture, format;

	unsigned char *imageData = stbi_load(path.c_str(), &width, &height, &nChannels, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (!imageData) {
		printf("Failed to load image data from path: %s\n", path);
		return;
	}

	switch (nChannels) {
		case 1: {
			format = GL_RED;
			break;
		}
		case 3: {
			format = GL_RGB;
			break;
		}
		case 4: {
			format = GL_RGBA;
			break;
		}
		default: {
			printf("Unsupported image channels: %d for image at %s\n", nChannels, path);
			stbi_image_free(imageData);
			return;
		}
	}

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		width,
		height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		imageData
	);

	int wrap_param = (format == GL_RGBA) ? GL_CLAMP_TO_EDGE : GL_REPEAT;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);

	this->id = texture;
	this->type = type;
	this->path = path;

	return;
}

Texture::~Texture() {
	glDeleteTextures(1, &this->id);
}

unsigned int Texture::GetID() {
	return this->id;
}

std::string Texture::GetType() {
	return this->type;
}

std::string Texture::GetPath() {
	return this->path;
}