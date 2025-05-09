# CGRA 354: Assignment 3 - Report

# Part 1: Animation / Boids (15 points)

## Boids Flocking Algorithm Implementation

### Overview
This part of the project implements a classic boids flocking simulation based on Craig Reynolds' algorithm. A single flock of boids is simulated with emergent group behavior resulting from simple individual rules.

<br/>
<img src="preview/Part_1.png" alt="Core" width="500" height="auto">
<br/>

---

### Implemented Behaviors

Each boid in the simulation independently applies five behaviors each frame:

1. **Avoidance**  
   Boids steer away from nearby neighbors to avoid collisions. A vector is computed by summing the inverse direction to each close boid, scaled by proximity.

2. **Alignment**  
   Boids adjust their direction to align with the average heading of nearby flockmates. This encourages group movement in a unified direction.

3. **Cohesion**  
   Boids steer toward the average position of local neighbors, which keeps the flock together and prevents scattering.

4. **Confinement**  
   Boids are kept within a bounding volume by applying a steering force when they approach the boundary limits. This ensures they remain in the visible area.

5. **Sensible Speed**  
   Boids are constrained to move within a minimum and maximum speed range. After all steering forces are applied, the velocity is clamped to remain within this range to mimic natural flight behavior.

---

### User Interaction: ImGUI Controls

To allow interactive tuning and experimentation, the following parameters are exposed through **ImGUI sliders** in the simulation interface:

- **Avoidance Weight** - Strength of the avoidance force.
- **Alignment Weight** - Strength of alignment with neighbors.
- **Cohesion Weight** - Strength of steering toward the group's center.
- **Local Radius** - Distance within which a boid senses neighbors.

These controls allow real-time adjustment of the flocking dynamics.

---

### Testing and Observations

- Boids initialized in random positions/directions inside bounds.
- Increasing `local radius` tightens group formation.
- Disabling `cohesion` causes boids to scatter.
- Confinement successfully prevents escape from scene bounds.

---

### Conclusion

A complete and configurable boids simulation has been implemented. The five core behaviors were realized with adjustable weighting through ImGUI. The simulation exhibits lifelike flocking behavior that adapts to user input.

> [!NOTE]
> TEST.

# Completion (5 points)

## Two Flocks and a Predator

### Overview
This part of the simulation extends the original boids system by introducing:
- Two distinct flocks of boids, each with different colors and flocking behavior.
- A predator that hunts the boids and causes them to scatter when nearby.

<br/>
<img src="preview/Part_2.png" alt="Completion" width="500" height="auto">
<br/>


---

### Multiple Flocks

The simulation includes two separate boid flocks:

- **Color Distinction**: Boids in each flock are visually distinguished using different colors.
- **Behavioral Logic**:
  - Boids **avoid** all nearby boids regardless of flock.
  - Boids **align** and **cohere** only with boids from **their own flock**.
  - This encourages flock separation while still allowing emergent group movement.

---

### Predator Behavior

A predator entity is added to the simulation with a set of custom behaviors:

- **Boid Avoidance of Predator**:
  - Each boid perceives the predator within a specific **scare radius**.
  - When the predator is within range, boids apply a **repulsion force** to flee.
  - The strength and direction of this force scale with proximity.

- **Predator Hunting Logic**:
  - The predator does **not align or cohere** with any flock.
  - It searches for the **nearest boid** within its **vision range** and chases it.
  - If a closer boid enters its vision, it **switches targets dynamically**.
  - The predator uses a **seek behavior** to chase boids based on predicted movement.

---

### Design Considerations

Several parameters were introduced to fine-tune predator-prey dynamics:

- **Boid perception radius of predator** - how far away a boid can "sense" danger.
- **Predator vision range** - how far the predator can see potential prey.
- **Target switching logic** - whether the predator locks on or switches targets dynamically.
- **Chase strategy** - whether the predator simply follows or uses smarter prediction.

---

### Observations

- When the predator enters a flock, the boids scatter in panic, creating visually interesting separation behavior.
- Boids dynamically re-group once the predator is no longer nearby.
- The predator successfully catches boids when its speed and vision range are well-tuned.
- Different strategies for target acquisition (e.g., nearest boid vs. fixed target) produce noticeably different hunting efficiency.

---

### Conclusion

This part of the assignment demonstrates how flocking and predation can coexist in a dynamic system. By layering additional behavior rules and tuning perception ranges, complex predator-prey interactions emerge naturally from simple rules.

# Challenge (2 points)

## Obstacle Avoidance

### Overview

In this final extension of the boids simulation, large static obstacles are introduced to the environment. These obstacles differ from other boids in that they are significantly larger and cannot be avoided using the standard boid avoidance mechanism.

<br/>
<img src="preview/Part_3.png" alt="Completion" width="500" height="auto">
<br/>

---

### Implementation Details

- **Obstacle Representation**:
  - Obstacles are implemented as large **spheres** placed within the scene bounds.
  - At least **three obstacles** are added with varied positions and sizes to demonstrate the avoidance behavior.

- **Custom Avoidance Logic**:
  - A specialized **obstacle avoidance force** was added to each boid's steering calculation.
  - The force is activated when a boid comes within a defined **obstacle detection radius**.
  - The avoidance vector is calculated based on the boid's direction relative to the nearest point on the obstacle's surface, scaled by proximity.
  - This ensures the boid steers around large objects rather than into them.

---

### Design Considerations

- **Avoidance Radius**: The detection radius is larger than for standard boid avoidance, allowing early response to large obstacles.
- **Smooth Steering**: The obstacle force is smoothly blended with other boid behaviors to avoid jitter or unnatural turns.
- **Scene Integration**:
  - The obstacles are visually rendered as scaled spheres using `cgra::drawSphere()` and basic transformation matrices.
  - Colors distinguish the obstacles from the boids for clarity.

---

### Observations

- Boids effectively steer around large obstacles rather than crashing into them.
- Behavior adapts dynamically - when a boid approaches an obstacle, it curves around and rejoins the flock once past the danger zone.
- Placement of obstacles influences flock structure and movement paths, creating natural-looking flow redirection.

---

### Conclusion

This challenge demonstrates how large environmental objects can be integrated into a boids system using a custom avoidance mechanism. The resulting simulation exhibits richer and more realistic group movement, especially in constrained or complex environments.

