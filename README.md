# Image Processor

A console application that applies various filters to images, similar to
those found in popular graphics editors.

## Supported Image Format

Input and output files must be in **BMP** format:
- 24-bit BMP
- No compression
- No color table
- `DIB header` type: `BITMAPINFOHEADER`

Example files are available in `test_script/data/`.

Make sure test images are saved as **24-bit BMP**.

## Command-Line Interface

```
{program} {input_path} {output_path}
    [-{filter_name_1} [param1] [param2] ...]
    [-{filter_name_2} [param1] [param2] ...]
    ...
```

When run without arguments, the program prints a help message.

### Example

```bash
./image_processor input.bmp /tmp/output.bmp -crop 800 600 -gs -blur 0.5
```

This does:
1. Load image from `input.bmp`.
2. Crop to 800×600 from the top-left corner.
3. Convert to grayscale.
4. Apply Gaussian blur with sigma 0.5.
5. Save result to `/tmp/output.bmp`.

The list of filters may be empty — the image is then saved unchanged.
Filters are applied in the order they appear in the command line.

## Filters

Color components are real numbers in `[0, 1]`. Pixels are `(R, G, B)`.
`(0, 0, 0)` — black, `(1, 1, 1)` — white.

Matrix filters define each color as a weighted sum of neighboring pixels.
The target pixel corresponds to the central element of the matrix.
Out-of-bounds pixels take the value of the nearest pixel inside the image.

### Base Filters

#### Crop — `-crop width height`
Crops the image to the given width and height, starting from the top-left
corner. If the requested size exceeds the image, the available part is used.

#### Grayscale — `-gs`
Converts to grayscale:

```
R' = G' = B' = 0.299 R + 0.587 G + 0.114 B
```

#### Negative — `-neg`
Inverts colors:

```
R' = 1 - R,  G' = 1 - G,  B' = 1 - B
```

#### Sharpening — `-sharp`
Sharpening filter, kernel:

```
 0  -1   0
-1   5  -1
 0  -1   0
```

#### Edge Detection — `-edge threshold`
Converts to grayscale, then applies kernel:

```
 0  -1   0
-1   4  -1
 0  -1   0
```

Pixels with value above `threshold` become white, others — black.

#### Gaussian Blur — `-blur sigma`
Gaussian blur with parameter `sigma`.

```
C[x0][y0] = sum over all pixels C[x][y] *
            1/(2*pi*sigma^2) * exp(-((x0-x)^2 + (y0-y)^2) / (2*sigma^2))
```

### Custom Filters

Crystallize, Glass Distortion filters implemented.

## Implementation Notes

- Third-party image libraries are **forbidden**.
- Design components to be as general as possible.
- All exceptional situations handled with clear error messages
  using C++ exceptions.
- No scenario (including malformed files) crash the program.
- Use `std::ifstream` / `std::ofstream` for file I/O.
- Suggested components:
  - Image class
  - BMP reader/writer classes
  - Filters
  - Controller that applies filters sequentially
- Common parts use inheritance.


