import numpy as np
from vector import Vector3D
import boidUtils

class Boid:

  def __init__(self, pos):
    self.pos = pos
    self.flockForceToApply = Vector3D.createNull()
    self.vel = Vector3D.createRandom(3)
  
  def copy(self):
    copy = Boid(self.pos.copy())
    copy.vel = self.vel.copy()
    return copy

  def update(self, maxSpeed, maxForce, secondsPast, boxSize):
    self.flockForceToApply.limit(maxForce)
    # add force to current velocity
    self.vel += self.flockForceToApply * secondsPast
    self.vel.limit(maxSpeed)
    # and add the velocity to current position
    self.pos += self.vel * secondsPast
    # correct if the boid went past an edge
    self.pos.x = boidUtils.correctEdgeAxis(self.pos.x, 0, boxSize.x);
    self.pos.y = boidUtils.correctEdgeAxis(self.pos.y, 0, boxSize.y);
    self.pos.z = boidUtils.correctEdgeAxis(self.pos.z, 0, boxSize.z);

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
    bs = np.array((), dtype=Boid)
    for i in range(len(boids)):
      boid = boidUtils.correctEdgeOverflowPerceptionR(self.pos, boids[i], sizeBox, percR)
      if self is not boid and self.pos.dist(boid.pos) <= percR:
        # print(boids[i])
        bs = np.append(bs, boid)
        # print(bs[len(bs) - 1])
        # print("")
    
    return bs

  def calcSeparation(self, bsInView, percR, maxForce):
    if len(bsInView) == 0:
      return Vector3D.createNull()

    sep = Vector3D.createNull()
    for i in range(len(bsInView)):
      pushForce = self.pos - bsInView[i].pos
      l = pushForce.length()
      pushForce.normalize()
      pushForce *= 1 - (l / percR)
      sep += pushForce
    
    sep /= len(bsInView)
    sep *= maxForce
    return sep

  def calcAlignment(self, bsInView, maxSpeed, maxForce):
    if len(bsInView) == 0:
      return Vector3D.createNull()

    averageHeading = self.vel.normalized()
    for i in range(len(bsInView)):
      if bsInView[i].vel.length() > 0:
        averageHeading += bsInView[i].vel.normalized()
    
    averageHeading /= len(bsInView) + 1
    return self.steerForceFromVector(averageHeading, maxSpeed, maxForce)

  def calcCohesion(self, bsInView, maxSpeed, maxForce):
    if len(bsInView) == 0:
      return Vector3D.createNull()

    centerOfMass = self.pos.copy()
    for i in range(len(bsInView)):
      centerOfMass += bsInView[i].pos
    
    centerOfMass /= len(bsInView) + 1
    return self.steerTowards(centerOfMass, maxSpeed, maxForce)

  def applyDestinationSteer(self, dest, maxSpeed, maxForce):
    seek = self.steerTowards(dest.copy(), maxSpeed, maxForce)
    seek *= .3
    self.flockForceToApply += seek

  def steerForceFromVector(self, desired, maxSpeed, maxForce):
    desired.setMag(maxSpeed)
    desired -= self.vel
    desired *= maxForce / maxSpeed
    return desired
  def steerTowards(self, dest, maxSpeed, maxForce):
    if self.pos == dest:
      return Vector3D.createNull() 
    dest -= self.pos
    return self.steerForceFromVector(dest, maxSpeed, maxForce)
  
