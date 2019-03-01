kslice
------

#### Continuous Integration Status

Continuous integration testing is running on CircleCI. Click [here](https://circleci.com/gh/webern/kslice) to go to the project.

  * `master` [![CircleCI](https://circleci.com/gh/webern/kslice/tree/master.svg?style=svg)](https://circleci.com/gh/webern/kslice/tree/master)
  * `develop` [![CircleCI](https://circleci.com/gh/webern/kslice/tree/develop.svg?style=svg)](https://circleci.com/gh/webern/kslice/tree/develop)

## Description

`kslice` is a command line utility for extracting i-frames (also known as key frames) from video and normalizing them (by grayscale pixel averaging) to a grid of, e.g. 32x32, 64x64, etc. This is the format often used as input into neural nets and other classification algorithms.

Given a video file and dimensions (e.g. 32x32, 64x64, 128x128), `kslice` extracts i-frames from the video, convert the frame into grayscale, split each frame into a grid of said dimensions, calculate median value of all the pixels of each cell of the grid and write the values to a CSV file together with the timestamp of the frame.

Each i-frame is then output as a csv row where the first position is the timestamp (in seconds) and the rest of the row represents the pixels of the grid (0 - 255, 8-bit grayscale).

For example: if a frame timestamp (in seconds) is 3.14 and the dimensions are 3x3, an example line might look like:

`3.14,42,255,9,13,67,0,27,33,123  // timestamp + 9 values (3x3)`

The output will have a line like this for each i-frame.

#### Usage

The `kslice` command takes 5 parameters:

  * `--help` a flag to displace the help info.
  * `--input` the video file to process.
  * `--optput` the csv file to write. Optional: when omitted the csv data will be written to stdout.
  * `--x-size` the size of the output grid's x dimension. Optional: defaults to 32.
  * `--y-size` the size of the output grid's y dimension. Optional: defaults to 32.

So an example call looks like this:

`kslice --input="./path/my.mp4" --output="data.csv" --x-size=20 --y-size=20`

#### Dependencies

  * `kslice` makes an external system call to `ffprobe`, so you must have this installed and visible in your `$PATH`.
  * `kslice` links to OpenCV at runtime.

##### Example:



Answer should be posted in a git repo.

Inspiration and Research
------------------------
  * [ffmpeg api example](https://www.ffmpeg.org/doxygen/0.6/api-example_8c-source.html)
  * [cmake ffmpeg find script](https://gist.github.com/royshil/6318407)
  * [ffmpeg decode example](https://unick-soft.ru/article.php?id=14)
  * [An ffmpeg and SDL Tutorial](http://dranger.com/ffmpeg/tutorial01.html)
  * [Unanswered OpenCV Question on Keyframe Extraction](http://answers.opencv.org/question/60390/how-to-get-keyframes-from-videofile/)
  * [Image Hashing Blogpost](https://www.pyimagesearch.com/2017/11/27/image-hashing-opencv-python/)
  * [SO ffmpeg linking question](https://stackoverflow.com/questions/25742626/ffmpeg-link-error)
  * [A Question I Posted on OpenCV](http://answers.opencv.org/question/209548/extracting-keyframes-i-frames-from-a-video)
  
Questions and Correspondence
----------------------------

Me:

> Keyframes:
>
> The instructions call for extracting keyframes from a video. At first, based on my experience with encoding video as a user of Adobe Premiere Pro, I though this meant extracting the special frames in which all pixels are included (as opposed to just the diff pixels). I fumbled around looking for a way to do this using ffmpeg.  ffmpeg is extremely difficult to use, I got it to compile and run but none of my programs produced the expected output.  Meanwhile, my research wasn’t turning up anything as far as a standard way of finding these ’special’ frames that I was looking for.
>
> I then realized that ‘keyframes’ might be a more generic term meaning something like ‘interesting’ frames (where interesting has something to do with ‘distance’ from the previous frame) or some such connotation.
>
> I also switched to OpenCV and had much better luck writing viable programs. Instead of detecting ‘keyframes’, the program I have here sorts a hash of the frames based on an extremely naive hashing function which basically amounts to how dark or light the frame is, and then selects N frames from that sorted list.
>
> Some of my reading leads me to believe that we can define keyframes as a set of frames that best summarize a video. It looks like defining and detecting these may be an area of active research in computer science.
>
> So, to continue I would probably need a hint as to what we mean by keyframes and a hint as to what method I might deploy to detect them.

Response (pasted from a response to another applicant):

> Many people working on this challenge process all the frames and don't think what's the difference between "frame" and "keyframe". We know it's tricky, and your descriptions how to resolve the problem are absolutely right. There are several ways how to proceed:
>   1) use opencv to read all the frames, and just make a comment in the code that you process all the frames and more work would be needed to process the keyframes only,
>   2) more low level approach is to use directly ffmpeg to extract the frames, e.g. based on this example https://github.com/FFmpeg/FFmpeg/blob/master/doc/examples/decode_video.c , where there is a flag in ffmpeg that identifies the keyframes. But we understand this is time consuming,
>   3) the easiest and fully functional would be calling something like this:
> `ffprobe -select_streams v -i x.mp4 -print_format json -show_entries frame=pict_type,coded_picture_number`
to extract indexes of the keyframes, which you could redirect to a file (>>), and then optionally pass as an argument to your executable, so that it can then skip non-keyframes. There are other formats for "print_format" option, which might be easier to process. ffprobe comes with ffmpeg. If this argument is not provided, the executable would process all frames as in 1)
>
> I would recommend to go with 1) and if you have still some time do 3). 
>
> Feel free to use any libraries and tools that fit you (opencv solution is perfectly fine). We understand the time is very limited and what can be achieved in this short given time. So please keep the solution simple and just put your ideas about optimizations or anything else to the code or readme. 