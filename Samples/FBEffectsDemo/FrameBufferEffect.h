
#ifndef FRAMEBUFFER_EFFECT_H_
#define FRAMEBUFFER_EFFECT_H_

#include <Nc/Graphics/Core/GL/FrameBuffer.h>
#include <Nc/Graphics/Effect/Effect.h>
#include <Nc/Graphics/Camera/Camera2d.h>
#include <Nc/Graphics/Object/Sprite.h>
#include <Nc/Graphics/Material/ProgrammableMaterial.h>

namespace Nc
{
  namespace Graphic
  {
    class FrameBufferEffect : public Effect
      {
      public:
	FrameBufferEffect();
	~FrameBufferEffect();

	static const char       *ClassName()		{return "FrameBufferEffect";}
	virtual ISceneNode      *Clone() const		{return new FrameBufferEffect(*this);}

	virtual void            Render(SceneGraph *scene);
	virtual void		Update(float elapsedTime);

	void			TriggerPostProcessActivation()	{_postProcessActivated = (_postProcessActivated) ? false : true;}

      private:
	void			InitFbo1();

      private:
	GL::FrameBuffer		_fboPass1;
	Camera2d		*_camera2d;
	Sprite			*_sprite;

	bool			_postProcessActivated;

	ProgrammableMaterial	*_postProcessMaterial;
	GL::Uniform<float,2>	*_sketchJitter;
	float			_sketchJitterSpeed;
	float			_timeToNextJitter;
      };
  }
}

#endif