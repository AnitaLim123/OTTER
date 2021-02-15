#include "BloomEffect.h"


void BloomEffect::Init(unsigned width, unsigned height)
{
	int index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);
	index++;

	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(unsigned(width / m_downScale), unsigned(height / m_downScale));
	index++;

	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(unsigned(width / m_downScale), unsigned(height / m_downScale));
	index++;

	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(width, height);
	index++;

	// loading shaders
	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;

	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/Bloom_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;

	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/BlurHorizontal_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;

	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/BlurVertical_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;

	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/BloomComposite_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;



	dir = glm::vec2(1.0f / width, 1.0f / height);
}

void BloomEffect::ApplyEffect(PostEffect* buffer)
{
	// draw previous buffer to first target
	BindShader(0);

	buffer->BindColorAsTexture(0, 0, 0);

	_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();

	// performs high pass on the first target
	BindShader(1);
	_shaders[1]->SetUniform("u_threshold", m_threshold);

	BindColorAsTexture(0, 0, 0);

	_buffers[1]->RenderToFSQ();

	UnbindTexture(0);

	UnbindShader();


	// compute the vert and horz blur
	for (unsigned i = 0; i < m_pass; i++)
	{
		// horizontal
		BindShader(2);
		_shaders[2]->SetUniform("u_dir", dir.x);

		BindColorAsTexture(1, 0, 0);

		_buffers[2]->RenderToFSQ();

		UnbindTexture(0);

		UnbindShader();

		// vertical
		BindShader(3);
		_shaders[3]->SetUniform("u_dir", dir.y);

		BindColorAsTexture(2, 0, 0);

		_buffers[1]->RenderToFSQ();

		UnbindTexture(0);

		UnbindShader();
	}


	// composite
	BindShader(4);

	buffer->BindColorAsTexture(0, 0, 0);
	BindColorAsTexture(1, 0, 1);

	_buffers[0]->RenderToFSQ();

	UnbindTexture(1);
	UnbindTexture(0);

	UnbindShader();
}

float BloomEffect::GetDownScale() const
{
	return m_downScale;
}

float BloomEffect::GetThreshold() const
{
	return m_threshold;
}

unsigned BloomEffect::GetPass() const
{
	return m_pass;
}

void BloomEffect::SetDownScale(float downScale)
{
	m_downScale = downScale;
}

void BloomEffect::SetThreshold(float threshold)
{
	m_threshold = threshold;
}

void BloomEffect::SetPass(unsigned pass)
{
	m_pass = pass;
}