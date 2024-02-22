import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection


file = 'Input.txt'

x_coords = []
y_coords = []
z_coords = []

with open(file, 'r') as f:
    for line in f:
        x, y, z = map(float, line.strip().split())
        x_coords.append(x)
        y_coords.append(y)
        z_coords.append(z)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x_coords, y_coords, z_coords, c=None, marker=None)

ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")
ax.set_title("3D Scatter Plot of Points")
plt.savefig("3dplot.png")
#1plt.show();

file="myoutput.txt"

vertices=[]
with open(file,"r") as f:
    for line in f:
        x,y,z=map(float, line.strip().split("\t"))
        vertices.append((x,y,z))

triangles=[vertices[i:i+3] for i in range(0,len(vertices),3)]


#fig=plt.figure(figsize=(12,10))
#ax=fig.add_subplot(111,projection='3d')

for i in range(len(triangles)):
    single_triangle=triangles[i]
    poly3d_single = [[vertex[:3] for vertex in single_triangle]]
    ax.add_collection3d(Poly3DCollection(poly3d_single, facecolors='cyan', linewidths=1, edgecolors='black', alpha=0.5))


ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.grid(True)



#ax.set_xlim(min(v[0] for v in single_triangle), max(v[0] for v in single_triangle))
#ax.set_ylim(min(v[1] for v in single_triangle), max(v[1] for v in single_triangle))
#ax.set_zlim(min(v[2] for v in single_triangle), max(v[2] for v in single_triangle))

ax.set_xlim(0,1000)
ax.set_ylim(0,1000)
ax.set_zlim(0,1000)

#plt.savefig("triangle.png")
plt.show()

