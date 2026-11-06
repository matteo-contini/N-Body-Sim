import re
import glob

import matplotlib.pyplot as plt
import numpy as np

from matplotlib.animation import FuncAnimation, PillowWriter


def which_file_iam(file_path):

    pattern = DATA_DIR+r'step_(\d+).dat'

    match = re.match(pattern, file_path)

    if match:
        number = int(match.group(1))
    return number

DATA_DIR = 'bin/data/'
FILE_PATTERN = 'step_*.dat'
OUTPUT_GIF = 'Animated_N_body.gif'
STEP=3

log_file = np.loadtxt(DATA_DIR + 'simulation_log.dat', delimiter=' ', skiprows = 1)
time_stamp = np.loadtxt(DATA_DIR + 'time_stamp.dat', delimiter=' ', skiprows = 1)
N_file = int(log_file[3])
print(f"Expecting {N_file+1} files")
print("Reading the data directory...")
# file list of all step files in the data directory
file_list = glob.glob(DATA_DIR + FILE_PATTERN, recursive=True)
# needs to be sorted to ensure correct order
# we sort with a simple rule
print("Sorting the paths...")
file_list.sort(key = which_file_iam)
N_file_founds = len(file_list)

if (N_file_founds == N_file + 1):
    print(f"Perfect match! I found {N_file_founds} files!")
else:
    print(f"I found {N_file_founds} files! I was not expecting that but I will try to continue!") 

# we create a view on the selected files
if (N_file_founds<=1001):
    selected_files = file_list
    selected_time_stamp = time_stamp
else:
    # we select every STEP file
    selected_files = file_list[::STEP]
    selected_time_stamp = time_stamp[::STEP]

# now loading the first data file

data = np.loadtxt(selected_files[0],delimiter = ' ',skiprows = 1, usecols=(1,2,3))


fig = plt.figure(figsize=(10,10))
fig.set_facecolor('black')


ax = fig.add_subplot(projection='3d')
ax.set_facecolor('black')
ax.set_xlim(-5,5)
ax.set_ylim(-5,5)
ax.set_zlim(-5,5)
ax.grid(False)
ax.set_axis_off()
ax.xaxis.pane.fill = ax.yaxis.pane.fill = ax.zaxis.pane.fill = False

scatter = ax.scatter(data[:,0], data[:,1], data[:,2], color='white',s=2.5)
title = ax.set_title(f"t = {selected_time_stamp[0]}", color="white", fontsize=10)

print(f"Generating the GIF, elaborating {len(selected_files)} frames...")

def update(frame_idx):
    file_path = selected_files[frame_idx]
   
    data = np.loadtxt(file_path, delimiter = ' ', skiprows = 1, usecols = (1,2,3))
    
    scatter._offsets3d = (data[:, 0], data[:, 1], data[:, 2])
    
    ax.view_init(elev=20, azim=frame_idx * 0.8)
    
    title.set_text(f"t = {selected_time_stamp[frame_idx]}")
    return scatter, title

anim = FuncAnimation(fig, update, frames=len(selected_files), blit=False)
writer = PillowWriter(fps=25)
anim.save(OUTPUT_GIF, writer=writer)
print(f"Completed! Saved as {OUTPUT_GIF} .")