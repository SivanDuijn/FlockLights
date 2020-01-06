import numpy as np

def limit(v, lim):
  l = np.linalg.norm(v)
  if l > lim:
    return setMag(v, lim)
  else:
    return v

def setMag(v1, mag):
  return v1 * (mag / np.linalg.norm(v1))

def mag(v):
  return np.linalg.norm(v)

def dist(v1, v2):
  return np.linalg.norm(v1 - v2)

def normalize(v):
  return v / np.linalg.norm(v)