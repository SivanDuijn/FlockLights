#!/usr/bin/env python

import numpy as np
from boid import Boid

amount = 100
boxSize = np.array((100, 100, 100))

boids = []

# b = Boid(np.random.random_sample(3) * 100)
# print(b.pos)
# print(b.vel)

for x in range(amount):
  boids.append(Boid(np.random.random_sample(3) * 100))


# boids[x].calcFlockForce(boids, 30, 40, 4, boxSize, 2)

counter = 0

while True:

  for boid in boids:
    boid.calcFlockForce(boids, 30, 40, 4, boxSize, 2)

  for boid in boids:
    boid.update(40, 4, 1, boxSize)

  print(counter)
  counter += 1