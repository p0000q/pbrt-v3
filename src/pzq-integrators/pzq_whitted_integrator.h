#pragma once

#include "pbrt.h"
#include "integrator.h"
#include "scene.h"

namespace pbrt {
class PzqWhittedIntegrator : public SamplerIntegrator {
  public:
	  PzqWhittedIntegrator(std::shared_ptr<const Camera> camera,
		  std::shared_ptr<Sampler> sampler,
		  const Bounds2i& pixelBounds)
	  : SamplerIntegrator(camera, sampler, pixelBounds) {

	}

	  virtual Spectrum Li(const RayDifferential &ray, const Scene &scene,
                      Sampler &sampler, MemoryArena &arena, int depth) const override;

};

PzqWhittedIntegrator* CreatePzqWhittedIntegrator(const ParamSet &params, std::shared_ptr<Sampler> sampler, std::shared_ptr<const Camera> camera);
}
