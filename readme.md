# kslice

#### Continuous Integration Status

Continuous integration testing is running on CircleCI. Click [here](https://circleci.com/gh/webern/kslice) to go to the project.

  * `master` [![CircleCI](https://circleci.com/gh/webern/kslice/tree/master.svg?style=svg)](https://circleci.com/gh/webern/kslice/tree/master)
  * `develop` [![CircleCI](https://circleci.com/gh/webern/kslice/tree/develop.svg?style=svg)](https://circleci.com/gh/webern/kslice/tree/develop)

## Description

`kslice` is a command line utility for extracting i-frames (also known as key frames) from video and normalizing them (by grayscale pixel averaging) to a grid (e.g. 32x32, 64x64, etc.) This is the format often used as input into neural nets and other classification algorithms.

Given a video file and dimensions (e.g. 32x32, 64x64, 128x128), `kslice` extracts i-frames from the video, converts the frames to grayscale, splits each frame into a grid of said dimensions, calculates the median value of all the pixels of each cell of the grid and writes the values to a CSV file.

Each i-frame is output as a csv row where the first position is the timestamp (in seconds) and the rest of the row represents the pixels of the grid (0 - 255, 8-bit grayscale).

For example: if a frame timestamp (in seconds) is 3.14 and the dimensions are 3x3, an example line might look like:

`3.14,42,255,9,13,67,0,27,33,123  // timestamp + 9 values (3x3)`

Each line of the csv will represent one frame.

#### Usage

The `kslice` command takes 5 parameters:

  * `--help` a flag to display the help info.
  * `--input` the video file to process.
  * `--optput` the csv file to write. Optional: when omitted the csv data will be written to stdout.
  * `--x-size` the size of the output grid's x dimension. Optional: defaults to 32.
  * `--y-size` the size of the output grid's y dimension. Optional: defaults to 32.

So an example call looks like this:

`kslice --input="./path/my.mp4" --output="data.csv" --x-size=20 --y-size=20`

#### Dependencies

  * `kslice` makes an external system call to `ffprobe`, so you must have this installed and visible in your `$PATH`.
  * `kslice` links to OpenCV at runtime.
  * `boost` is required at compile time, but is statically linked.

## Build

#### Setup

You must install `boost`, `ffmpeg` and `opencv`. The easiest way to see what is needed to setup the a machine is to look at the CircleCI Dockerfile in `.circleci/Dockerfile` which takes a clean Ubuntu 18.04 machine and gets it ready to build `kslice`.

###### Prerequisites:

```
sudo apt-get update && sudo apt-get install -y \
    build-essential \
    cmake \
    ffmpeg \
    libopencv-dev \
    libboost-all-dev
```

#### Build

Let's say you have cloned the repo to `/repos/kslice` and that you want to do an out-of-source build. The following commands will compile `kslice` and tests.

```
mkdir -p /repos/kslice-build
cd /repos/kslice-build
cmake ../kslice
make -j12
```

There is no `make install` step, just use the binary from `/repos/kslice-build/kslice`, or move it where you want it.

#### Test

Having compiled per the instructions above, we can run the tests with:

`/repos/kslice-build/lib/kslice`

## Approach

Initially I wanted to create a self-contained program, so I began by writing a program that links to, and uses the ffmpeg api. This proved to be quite difficult because the ffmpeg api is unstable and complicated. Also, I might have been doing something wrong, because ffmpeg wanted to link to all of my `macOS`'s frameworks, such as CoreFoundation, CoreVideo, etc. This was feeling like a bad path, so I took a step back.

I then started working with OpenCV which is much easier to use. But I discovered that, although it has ffmpeg as an underlying dependency, it doesn't expose the functionality to detect I-Frames.

With these challenges I decided that it would be acceptable for the `kslice` program to make an external call to `ffprobe` to get the list of i-frames from the file.

#### Program Flow

  * The program calls `ffprobe` to get a list of i-frames. It requests this in `xml` format and writes this data to a temporary file.
  * The program then reads in the temporary `xml` file and extracts the list of i-fram indices.
  * The program then uses OpenCV to get these frames with timestamps, convert them to grayscale, and downsample the frame to the size of the desired output grid using linear interpolation.
  * The frames are then output to either the output file or stdout.

#### Exceptions

Any of the above steps may throw an exception. All exceptions are caught in main where an error message is printed to the user before exiting with a non-zero code.

#### Architecture

The bulk of the program logic is separated from `kslice` main in a library with the namespace `kscore`.

In such a small program, we don't need much in the way of object-oriented patterns. However, I did see an opportunity to inject a strategy object which represents the call to `ffprobe`. `IFrameStrategy` is implemented by `FFProbeStrategy`.  In theory, we could implement different strategies for this and simply inject them into the program logic. We could also mock this for unit testing.

#### Testing

The `kscore` library is tested using Catch2. Not all functions are tested due to lack of time, but there are two end-to-end integration tests which prove that `kslice`, as a whole, is working properly. A few additional unit-level tests are written as well, for example the parsing of program arguments is tested.

The library, tests and `slice` executable are being compiled and tested with each commit via CircleCI (see top for link).

#### Conclusion

This has been a fun and unique coding challenge. Thank you for considering me for the C++ developer opportunity. Based on this challenge and what I have learned about the company, I believe working on your project would be quite exciting!

## Research Links

These are some links that I wanted to keep track of while working on this:

  * [ffmpeg api example](https://www.ffmpeg.org/doxygen/0.6/api-example_8c-source.html)
  * [cmake ffmpeg find script](https://gist.github.com/royshil/6318407)
  * [ffmpeg decode example](https://unick-soft.ru/article.php?id=14)
  * [An ffmpeg and SDL Tutorial](http://dranger.com/ffmpeg/tutorial01.html)
  * [Unanswered OpenCV Question on Keyframe Extraction](http://answers.opencv.org/question/60390/how-to-get-keyframes-from-videofile/)
  * [Image Hashing Blogpost](https://www.pyimagesearch.com/2017/11/27/image-hashing-opencv-python/)
  * [SO ffmpeg linking question](https://stackoverflow.com/questions/25742626/ffmpeg-link-error)
  * [A Question I Posted on OpenCV](http://answers.opencv.org/question/209548/extracting-keyframes-i-frames-from-a-video)
