#pragma once

namespace kslice
{
    constexpr const char* const help =
            R"(
kslice: Finds the I-frames (a.k.a. Key Frames) in a video and reduces
these to an NxM grid of feature values.

Options:
   --help     - Print this help message
   --input    - The input video file
   --output   - The output csv file (if blank output will be to stout)
   --x-size   - The width of the output feature sets (default 32)
   --y-size   - The height of the output feature sets (default 32)

prerequisite: kslice makes a call to ffprobe. This must be in your
system's PATH in order for kslice to work.

The output it is in the form T,A,A,A,B,B,B,C,C,C

For example, if a frame timestamp (in seconds) is 3.14 and the dimensions
are 3x3 an example line might look like:

     3.14,42,255,9,13,67,0,27,33,123  // timestamp + 9 values (3x3)

Each row in the csv file represents one I-Frame


example usage: kslice --input="./files/my.mp4" --output="./files/out.csv" --x-size=32 --y-size=32
)";
}