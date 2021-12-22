#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
private:
	unsigned int id;
	std::string type;
	std::string path;

public:
	Texture(std::string path, std::string type);
	~Texture();

	unsigned int GetID();
	std::string GetType();
	std::string GetPath();
};

#endif // TEXTURE_H