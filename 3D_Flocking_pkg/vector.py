# 3D vector!!

import numpy as np

class Vector3D:
  def __init__(self, x, y, z):
    self.__npArr__ = np.array([x, y, z])
  
  @classmethod
  def createNull(cls):
    return cls(0., 0., 0.)
  @classmethod 
  def createFromArray(cls, array):
    return cls(array[0], array[1], array[2])
  @classmethod  
  def createRandom(cls, maxVal):
    npArr = np.random.random_sample(3) * maxVal
    return cls.createFromArray(npArr)
    
  def copy(self):
    return Vector3D.createFromArray(self.getArray())

  def normalize(self):
    l = self.length()
    self /= l
    return self
  def normalized(self):
    copy = self.copy()
    copy.normalize()
    return copy

  def length(self):
    return np.linalg.norm(self.__npArr__)
  def setMag(self, mag):
    self.normalize()
    self *= mag
    return self

  def limit(self, mag):
    if self.length() > mag:
      self.setMag(mag)
    return self


  def dist(self, other):
    return (self - other).length()

  def getArray(self):
    return self.__npArr__
  def __add__(self, other):
    copy = self.copy()
    copy += other
    return copy
  def __iadd__(self, other):
    self.__npArr__ += other.getArray()
    return self
  def __sub__(self, other):
    copy = self.copy()
    copy -= other
    return copy
  def __isub__(self, other):
    self.__npArr__ -= other.getArray()
    return self
  def __mul__(self, value):
    copy = self.copy()
    copy *= value
    return copy
  def __imul__(self, value):
    self.__npArr__ *= value
    return self
  def __truediv__(self, value):
    copy = self.copy()
    copy /= value
    return copy
  def __itruediv__(self, value):
    self.__npArr__ = np.true_divide(self.__npArr__, value)
    return self

  def __eq__(self, other):
    return np.array_equal(self.__npArr__, other.getArray())
  
  def __str__(self):
    return str(self.__npArr__)

  @property
  def x(self):
    return self.__npArr__[0]
  @x.setter
  def x(self, value):
    self.__npArr__[0] = value
  @property
  def y(self):
    return self.__npArr__[1]
  @y.setter
  def y(self, value):
    self.__npArr__[1] = value
  @property
  def z(self):
    return self.__npArr__[2]
  @z.setter
  def z(self, value):
    self.__npArr__[2] = value
  


  