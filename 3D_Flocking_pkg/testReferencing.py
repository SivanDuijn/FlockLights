import numpy as np
from boid import Boid
import copy

b = Boid(np.random.random_sample(3) * 100)
print(id(b))
b2 = copy.copy(b)
print(id(b2))
print()
print(id(b.pos))
print(id(b2.pos))

b2.pos = np.array((12, 13, 14), dtype=float)
print(id(b2.pos))



# a = np.array((1,2,3), dtype=float)

# print(id(a))
# lst = []
# lst.append(a)
# print(id(lst[0]))
# b = np.copy(a)
# print(id(b))
# b[0] = 4
# print(a)
