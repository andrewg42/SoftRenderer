#include "viewer/GL_Context.h"

int main() {
	GL_Context &gl = GL_Context::instance();
	gl.run();
	return 0;
}
