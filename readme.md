kslice
------

A command line utility and library for extracting keyframes from video and normalizing them for ML processing.

## Requirements

Given a video file and dimensions (e.g. 32x32, 64x64, 128x128), extract keyframes from the video, convert the frame into grayscale, split each frame into a grid of said dimensions, calculate median value of all the pixels of each cell of the grid and write the values to a CSV file together with the timestamp of the frame.

##### Example:

If a frame timestamp (in seconds) is 3.14 and the dimensions are 3x3 an example line might look like:

`    3.14,42,255,9,13,67,0,27,33,123  // timestamp + 9 values (3x3)`

Answer should be posted in a git repo.
