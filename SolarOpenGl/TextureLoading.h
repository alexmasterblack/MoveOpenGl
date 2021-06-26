#pragma once

class TextureLoading {
private:
	unsigned int texture;
public:
	TextureLoading() = default;
	TextureLoading(const char* path);
	void Binding(int check = -1);
};