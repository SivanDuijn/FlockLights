from vector import Vector3D

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

  x = correctEdgeInfinityPerceptionOnAxis(ourPos.x, otherBoid.pos.x, sizeBox.x, percR)
  y = correctEdgeInfinityPerceptionOnAxis(ourPos.y, otherBoid.pos.y, sizeBox.y, percR)
  z = correctEdgeInfinityPerceptionOnAxis(ourPos.z, otherBoid.pos.z, sizeBox.z, percR)
  pos = Vector3D(x, y, z)
  if pos == ourPos:
    return otherBoid
  else:
    b = otherBoid.copy()
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