#pragma once

#include "Graphics/Post/PostEffect.h"

class BloomEffect : public PostEffect
{
public:
	// initialize 
	// override Init
	void Init(unsigned width, unsigned height) override;

	// applies effects
	// passes previous framebuffer
	void ApplyEffect(PostEffect* buffer) override;

	// getter
	float GetDownScale() const;
	float GetThreshold() const;
	unsigned GetPass() const;

	// setter
	void SetDownScale(float downScale);
	void SetThreshold(float threshold);
	void SetPass(unsigned pass);

private:
	float m_downScale = 2.0f;
	float m_threshold = 0.01f;
	unsigned m_pass = 10;
	glm::vec2 dir;
};