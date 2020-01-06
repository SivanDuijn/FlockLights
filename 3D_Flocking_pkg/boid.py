import numpy as np
import boidUtils as bu
import vectorUtils as vu

class Boid:

  def __init__(self, pos):
    self.pos = pos
    self.vel = np.random.random_sample(3) * 3
  
  def copyForOverflow(self):
    copy = Boid(self.pos.copy())
    copy.vel = self.vel.copy()
    return copy

  def update(self, maxSpeed, maxForce, secondsPast, boxSize):
    self.flockForceToApply = vu.limit(self.flockForceToApply, maxForce)
    # add force to current velocity
    self.vel += self.flockForceToApply * secondsPast
    self.vel = vu.limit(self.vel, maxSpeed)
    # and add the velocity to current position
    self.pos += self.vel * secondsPast
    # correct if the boid went past an edge
    self.pos[0] = bu.correctEdgeAxis(self.pos[0], 0, boxSize[0])
    self.pos[1] = bu.correctEdgeAxis(self.pos[1], 0, boxSize[1])
    self.pos[2] = bu.correctEdgeAxis(self.pos[2], 0, boxSize[2])

  def calcFlockForce(self, boids, percR, maxSpeed, maxForce, sizeBox, serpMult):
    bsInView = self.calcBoidsInView(boids, percR, sizeBox)
    s = self.calcSeparation(bsInView, percR, maxForce)
    c = self.calcCohesion(bsInView, maxSpeed, maxForce)
    a = self.calcAlignment(bsInView, maxSpeed, maxForce)
    # print(s)
    # print(c)
    # print(a)
    # apply modifiers
    s *= serpMult
    c *= .4
    a *= .2
    # add up and assign to flockForceToApply !!! flockForceToApply is reset, call applyDestinationSteer after this!
    self.flockForceToApply = s + c + a

  def calcBoidsInView(self, boids, percR, sizeBox):
    bs = []
    for b in boids:
      boid = bu.correctEdgeOverflowPerceptionR(self.pos, b, sizeBox, percR)
      if self is not boid and vu.dist(self.pos, boid.pos) <= percR:
        # print(boids[i])
        bs.append(boid)
        # print(bs[len(bs) - 1])
        # print("")
    
    return bs

  def calcSeparation(self, bsInView, percR, maxForce):
    if len(bsInView) == 0:
      return np.array((0, 0, 0), dtype=float)

    sep = np.array((0, 0, 0), dtype=float)
    for bInView in bsInView:
      pushForce = self.pos - bInView.pos
      l = vu.mag(pushForce)
      pushForce = vu.normalize(pushForce)
      pushForce *= 1 - (l / percR)
      sep += pushForce
    
    sep /= len(bsInView)
    sep *= maxForce
    return sep

  def calcAlignment(self, bsInView, maxSpeed, maxForce):
    if len(bsInView) == 0:
      return np.array((0, 0, 0), dtype=float)

    averageHeading = vu.normalize(self.vel)
    for bInView in bsInView:
      if vu.mag(bInView.vel) > 0:
        averageHeading += vu.normalize(bInView.vel)
    
    averageHeading /= len(bsInView) + 1
    return self.steerForceFromVector(averageHeading, maxSpeed, maxForce)

  def calcCohesion(self, bsInView, maxSpeed, maxForce):
    if len(bsInView) == 0:
      return np.array((0, 0, 0), dtype=float)

    centerOfMass = np.copy(self.pos)
    for bInView in bsInView:
      centerOfMass += bInView.pos
    
    centerOfMass /= len(bsInView) + 1
    return self.steerTowards(centerOfMass, maxSpeed, maxForce)

  def applyDestinationSteer(self, dest, maxSpeed, maxForce):
    seek = self.steerTowards(dest.copy(), maxSpeed, maxForce)
    seek *= .3
    self.flockForceToApply += seek

  def steerForceFromVector(self, desired, maxSpeed, maxForce):
    desired = vu.setMag(desired, maxSpeed)
    desired -= self.vel
    desired *= maxForce / maxSpeed
    return desired
  def steerTowards(self, dest, maxSpeed, maxForce):
    if np.array_equal(self.pos, dest):
      return np.array((0, 0, 0), dtype=float)
    dest -= self.pos
    return self.steerForceFromVector(dest, maxSpeed, maxForce)
  
