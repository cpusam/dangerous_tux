#include "renderer/renderer.hpp"

int Renderer::countID = 0;

Renderer::Renderer (  )
:id(countID++)
{
}

int Renderer::getId (  ) const
{
	return id;
}

unsigned int Renderer::getFlags() const {
	return flags;
}
