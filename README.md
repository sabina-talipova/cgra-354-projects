# Assignment 4 - Ray Tracing Renderer

This project implements a CPU-based ray tracer that supports rendering scenes composed of multiple geometric primitives and light types, using various lighting and reflection models.

---

## Features

- **Ray generation** via camera parameters (position, orientation, FOV, resolution)
- **Intersection testing** with spheres, planes, disks, triangles
- **Multiple light types**: Directional and Point Lights
- **Lighting models**:
  - Ambient (indirect diffuse)
  - Lambertian diffuse
  - Blinn-Phong specular
- **Recursive reflections**
- **Textured materials**
- **Stochastic path tracing**

---

## Core Implementation

<br/>
<img src="preview/img_core_01.png" alt="Core img 1" width="500" height="auto">
<br/>

### 1. `Camera::generateRay`
- Implemented to compute rays from camera position using pixel coordinates.
- Converts pixel to normalized device coordinates, accounts for aspect ratio and FOV.
- Ray direction is transformed from camera space to world space using camera rotation.

### 2. `Sphere::intersect`
- Ray-sphere intersection computed using the quadratic formula.
- Handles cases for no hit, one hit (tangent), and two hits.
- Returns closest valid intersection, stores hit position, normal, and material.

### 3. `DirectionalLight` and `PointLight`
- Implemented occlusion checks using shadow rays.
- Directional light:
  - Tests shadows by casting rays in the opposite direction.
  - Returns constant irradiance.
- Point light:
  - Tests visibility toward light position.
  - Returns irradiance inversely proportional to squared distance.

<br/>
<img src="preview/img_core_02.png" alt="Core img 2" width="500" height="auto">
<br/>

### 4. `CorePathTracer::sampleRay`
- Implements:
  - **Ambient (indirect diffuse)**: constant color for global illumination fallback.
  - **Lambertian diffuse**: light intensity modulated by angle between normal and light.
  - **Blinn-Phong specular**: uses half-vector and shininess for highlights.

<br/>
<img src="preview/img_core_03.png" alt="Core img 3" width="500" height="auto">
<br/>
<img src="preview/img_core_04.png" alt="Core img 4" width="500" height="auto">
<br/>
<img src="preview/img_core_05.png" alt="Core img 5" width="500" height="auto">
<br/>
---

## Completion

<br/>
<img src="preview/img_comp_01.png" alt="Completion img 3" width="500" height="auto">
<br/>

### 1. Shape Test Scene
- Extended the scene to include:
  - **Plane**
  - **Disk**
  - **Triangle**
- Verified that the ray tracer handles intersections with each shape correctly.

### 2. `CompletionPathTracer::sampleRay`
- Adds recursive specular reflections.
- Reflects rays using surface normals and `reflect()` function.
- Samples secondary rays to accumulate reflected light contribution.
- Reflection weight is based on material shininess.

<br/>
<img src="preview/img_comp_02.png" alt="Completion img 3" width="500" height="auto">
<br/>
<img src="preview/img_comp_03.png" alt="Completion img 3" width="500" height="auto">
<br/>
<img src="preview/img_comp_04.png" alt="Completion img 3" width="500" height="auto">
<br/>
<img src="preview/img_comp_05.png" alt="Completion img 3" width="500" height="auto">
<br/>

---

## 🔬 Challenge (3 points)

### 1. Textured Materials
- Implemented `Texture` class for image-based texture sampling.
- Added `TexturedMaterial` class that uses UV coordinates from hit info.
- Texture color modulates diffuse reflection.
- Supports common formats (PNG, JPG, etc.) using `stb_image`.

### 2. `ChallengePathTracer::sampleRay`
- Replaces fixed reflection models with **stochastic sampling**:
  - **Diffuse**: samples random direction in hemisphere around normal.
  - **Specular**: samples direction from Phong lobe using exponent based on shininess.
- Enables soft lighting and global illumination effects.
- Handles multiple bounces with Russian roulette termination or max depth.

<img src="preview/img_chall_01.png" alt="Completion img 3" width="500" height="auto">
<br/>
<img src="preview/img_chall_02.png" alt="Completion img 3" width="500" height="auto">
<br/>