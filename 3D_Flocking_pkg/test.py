#!/usr/bin/env python

from vector import Vector3D
import numpy as np
from boid import Boid

amount = 100

boids = np.empty(amount, dtype=Boid)

for x in range(amount):
  boids[x] = Boid(Vector3D.createRandom(100))

boxSize = Vector3D(100, 100, 100)

boids[x].calcFlockForce(boids, 30, 40, 4, boxSize, 2)

counter = 0

while True:

  for x in range(amount):
    boids[x].calcFlockForce(boids, 30, 40, 4, boxSize, 2)

  for x in range(amount):
    boids[x].update(40, 4, 1, boxSize)

  print(counter)
  counter += 1