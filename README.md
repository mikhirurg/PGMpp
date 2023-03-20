# PGMpp

## Project description

PGMpp is an image manipulation library for PGM image format. It allows performing various transformations such as:
image inversion, mirroring, and rotation. This library allows drawing lines over the image with an application and applying the sRGB gamma correction. 
Additionally, the library supports various dithering algorithms. And finally, the library allows performing transitions over different color spaces.

## How to use the project

PGMpp supports various image transformation methods which can be used to work with grayscale PGM and colorful PPM images. 

### Simple image transformations

| Transformation | Original | Result | Example |
|----------------|----------|--------|---------|
| Inversion      | <img src="/img/mona_lisa.png" alt="Demo 1" width=250> | <img src="/img/output_1.png" alt="Demo 1" width=250> | [Example 1](src/examples/example_1.cpp) |
| Horizontal flip | <img src="/img/bird.png" alt="Demo 2" width=250> | <img src="/img/output_2.png" alt="Demo 2" width=250> | [Example 2](src/examples/example_2.cpp)|
| Vertical flip | <img src="/img/bird.png" alt="Demo 3" width=250> | <img src="/img/output_3.png" alt="Demo 3" width=250> | [Example 3](src/examples/example_3.cpp)|
| Draw line | <img src="/img/black_square.png" alt="Demo 4" width=250> | <img src="/img/output_4.png" alt="Demo 4" width=250> | [Example 4](src/examples/example_4.cpp)|

### Dithering algorithms

| Algorithm                          | Result  | Example |
|------------------------------------|---------|---------|
| Original                           | <img src="/img/coast.png" alt="Original" width=500> | |
| Atkinson dithering (2 bits)        | <img src="/img/output_5.png" alt="Demo 5" width=500> | [Example 5](src/examples/example_5.cpp)|
| Color bit correction (2 bits)      | <img src="/img/output_6.png" alt="Demo 6" width=500> | [Example 6](src/examples/example_6.cpp)|
| Floyd Steinberg dithering (2 bits) | <img src="/img/output_7.png" alt="Demo 7" width=500> | [Example 7](src/examples/example_7.cpp)|
| JJN dithering (2 bits)             | <img src="/img/output_8.png" alt="Demo 8" width=500> | [Example 8](src/examples/example_8.cpp)|
| Halftone dithering (2 bits)        | <img src="/img/output_9.png" alt="Demo 9" width=500> | [Example 9](src/examples/example_9.cpp)|
| Sierra dithering (2 bits)          | <img src="/img/output_10.png" alt="Demo 10" width=500> | [Example 10](src/examples/example_10.cpp)|
| Error diffusion dithering (2 bits) | <img src="/img/output_11.png" alt="Demo 11" width=500> | [Example 11](src/examples/example_11.cpp)|
| Ordered dithering (2 bits)         | <img src="/img/output_12.png" alt="Demo 12" width=500> | [Example 12](src/examples/example_12.cpp)|
| Random dithering (2 bits)          | <img src="/img/output_13.png" alt="Demo 13" width=500> | [Example 13](src/examples/example_13.cpp)|


### Color space transformations

| From RGB to... | Result | Example |
|----------------|--------|---------|
| RGB (Original) | <img src="/img/lena2.png" alt="Demo 13" width=300> | |
| HSV            | <img src="/img/output_14.png" alt="Demo 14" width=300> | [Example 14](src/examples/example_14.cpp)|
| HSL            | <img src="/img/output_15.png" alt="Demo 15" width=300> | [Example 15](src/examples/example_15.cpp)|
| CMY            | <img src="/img/output_16.png" alt="Demo 16" width=300> | [Example 16](src/examples/example_16.cpp)|
| YCoCg          | <img src="/img/output_17.png" alt="Demo 17" width=300> | [Example 17](src/examples/example_17.cpp)|
| YCbCr601       | <img src="/img/output_18.png" alt="Demo 18" width=300> | [Example 18](src/examples/example_18.cpp)|
| YCbCr709       | <img src="/img/output_19.png" alt="Demo 19" width=300> | [Example 19](src/examples/example_19.cpp)|



## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

## License

[LGPL-3.0](/COPYING.LESSER)
