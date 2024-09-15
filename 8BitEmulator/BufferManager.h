#include <glad/glad.h>
#include "Grid.h"
// TODO: Create template for object with VAO/VBO that implements Interface (to be sure passed type implements VAO/VBO)

namespace Renderer {
	class __declspec(dllimport) BufferManager {
	public:
		//unsigned int VAO, VBO;

		Grid& grid;

		//Template here
		BufferManager(Grid& grid);
		~BufferManager();

		void Bind();
		void Unbind() const;

		void SetBufferData(size_t size, const void* data, GLenum usage = GL_STATIC_DRAW) const;
		void SetVertexAttribPointer(unsigned int index, int size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const;
		void SetGridStandard();
	};
}