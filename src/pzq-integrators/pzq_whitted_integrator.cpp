
#include "pzq_whitted_integrator.h"
#include "camera.h"

namespace pbrt {
Spectrum PzqWhittedIntegrator::Li(const RayDifferential &ray,
	const Scene &scene, Sampler &sampler,
	MemoryArena &arena, int depth) const {

	Spectrum L;
    SurfaceInteraction isect;
	if (!scene.Intersect(ray, &isect)) { 
		// no intersection of scene
		for (const auto &light : scene.lights) {
			L += light->Le(ray);
		}
		return L;
	}

	const Normal3f &n = isect.shading.n;
	Vector3f wo = isect.wo;
	// intersect something
	isect.ComputeScatteringFunctions(ray, arena);
	if (!isect.bsdf) { 
		// no bsdf, a non-scattering transparent surface?
		// construct new ray from intersect point
		return this->Li(isect.SpawnRay(ray.d), scene, sampler, arena, depth);
	}

	// isect may emit
	L += isect.Le(wo);

	// add contribution of lights
	for (const auto &light : scene.lights) {
		Vector3f wi;
		Float pdf;
		VisibilityTester visibility;
		Spectrum Li = light->Sample_Li(isect, sampler.Get2D(), &wi, &pdf, &visibility);
		if (Li.IsBlack() || pdf == 0) continue;
		Spectrum f = isect.bsdf->f(wo, wi);
		if (!f.IsBlack() && visibility.Unoccluded(scene)) {
			L += f * Li * AbsDot(wi, n) / pdf;
		}
	}
	// TODO: for specular and refraction
	return L;
}

PzqWhittedIntegrator* CreatePzqWhittedIntegrator(const ParamSet &params, std::shared_ptr<Sampler> sampler, std::shared_ptr<const Camera> camera) {
	Bounds2i pixelBounds = camera->film->GetSampleBounds();
	return new PzqWhittedIntegrator(camera, sampler, pixelBounds);
}

}