#include <EngineHeaders.h>

class Window {
public:

	void create_framebuffer(int width, int height);

	void bind_framebuffer();

	void unbind_framebuffer();

	void rescale_framebuffer(float width, float height);

	void destroy_buffers();

	unsigned int GetTexture();

private:
	unsigned int FBO;
	unsigned int RBO;
	unsigned int frameBufferTexture;
};