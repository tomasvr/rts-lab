#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

x = range(1,11)
y = [190646, 255969, 298631, 311960, 341293, 363958, 385289, 395948, 426617, 449282]

for i in range(10):
    y[i] = y[i] / 976600 * 100

plt.xticks(np.arange(1, 11, 1))
plt.xlabel("Number of tasks")
plt.ylabel("Overhead per time unit")
plt.title("Relation between number of tasks and overhead")

plt.bar(x,y)
plt.show()

