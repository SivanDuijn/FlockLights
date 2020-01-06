import numpy as np
import copy

def correctEdgeAxis(axisValue, minn, maxx):
  if axisValue > maxx:
    axisValue = minn
  elif axisValue < minn:
    axisValue = maxx
  return axisValue

def correctEdgeOverflowPerceptionR(ourPos, otherBoid, sizeBox, percR):
  # if a boid is close to an edge, than a boid on the other side of the
  # screen could still be inside the perception radius
  # because boids will get teleported to the other side of the sceen when they fly out of the screen
  # here we return a teleported copy, if a boid on the other side is in range

  x = correctEdgeInfinityPerceptionOnAxis(ourPos[0], otherBoid.pos[0], sizeBox[0], percR)
  y = correctEdgeInfinityPerceptionOnAxis(ourPos[1], otherBoid.pos[1], sizeBox[1], percR)
  z = correctEdgeInfinityPerceptionOnAxis(ourPos[2], otherBoid.pos[2], sizeBox[2], percR)
  pos = np.array((x, y, z), dtype=float)
  if np.array_equal(pos, ourPos):
    return otherBoid
  else:
    b = copy.copy(otherBoid)
    b.pos = pos
    return b

def correctEdgeInfinityPerceptionOnAxis(thisAxis, toCheckAxis, sizeBoxAxis, percR):
  if thisAxis < toCheckAxis:
    overflow = percR - thisAxis
    if toCheckAxis > sizeBoxAxis - overflow:
      toCheckAxis -= sizeBoxAxis
  
  elif sizeBoxAxis - thisAxis < percR:
    overflow = percR - (sizeBoxAxis - thisAxis)
    if toCheckAxis < overflow:
      toCheckAxis += sizeBoxAxis

  return toCheckAxis