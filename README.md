# PNM Image Generator

A lightweight C program for generating Netpbm format images (PBM, PGM, and PPM) with customizable patterns, gradients, and color effects.

## Overview

This tool creates three types of Netpbm format images with procedurally generated patterns:

- **PBM (Portable BitMap)**: Black and white images with geometric patterns
- **PGM (Portable GrayMap)**: Grayscale images with smooth gradient effects
- **PPM (Portable PixMap)**: Full-color images with multi-channel color gradients

Perfect for testing image processing pipelines, generating test patterns, or creating procedural graphics.

## Features

- Generate three different image formats (PBM, PGM, PPM)
- Customizable dimensions with automatic validation
- Support for both ASCII and binary (raw) output formats
- Automatic color channel extraction for PPM images
- Fast and lightweight with minimal dependencies
- Comprehensive input validation and error handling


## Quick Start

```bash
# Build the project
cd main
make all

# Generate a black and white pattern
./main 1 120 120 output.pbm 0

# Generate a grayscale gradient
./main 2 200 200 gradient.pgm 0

# Generate a colorful image
./main 3 120 120 colors.ppm 1
```

## Building the Project

Navigate to the `main` directory and use the provided Makefile:

```bash
cd main
make all
```

To clean build artifacts:

```bash
make clean
```

To clean generated images:

```bash
make cleanPNM    # Clean all image files
make cleanPBM    # Clean only PBM files
make cleanPGM    # Clean only PGM files
make cleanPPM    # Clean only PPM files
```

## Usage

```bash
./main <img_type> <width> <height> <filename> <format>
```

### Parameters

- **img_type**: Type of image to generate
  - `1` = PBM (Portable BitMap)
  - `2` = PGM (Portable GrayMap)
  - `3` = PPM (Portable PixMap)
  
- **width**: Image width in pixels
  - PBM/PGM: Must be ≥ 4 and divisible by 4
  - PPM: Must be ≥ 6 and divisible by 6
  
- **height**: Image height in pixels
  - All types: Must be ≥ 4 and divisible by 4
  
- **filename**: Output filename (e.g., `output.pbm`, `image.pgm`, `color.ppm`)
  
- **format**: Output format
  - `0` = ASCII (human-readable text format)
  - `1` = Raw (binary format, smaller file size)

## Image Types & Patterns

### PBM Images (Type 1)

Generates black and white images featuring:
- Black background
- White centered rectangle (width/2 × height/2)
- Two black diagonal lines crossing the entire image

**Examples:**
```bash
./main 1 120 120 pattern.pbm 0    # ASCII format
./main 1 120 120 pattern.pbm 1    # Binary format
```

### PGM Images (Type 2)

Generates grayscale images with:
- Black background
- Centered rectangle (width/2 × height/2) with four triangular gradient sections:
  - **Upper triangle**: White to black gradient
  - **Lower triangle**: Black to white gradient
  - **Left triangle**: White to black gradient
  - **Right triangle**: Black to white gradient

**Examples:**
```bash
./main 2 120 120 gradient.pgm 0    # ASCII format
./main 2 200 200 gradient.pgm 1    # Binary format
```

### PPM Images (Type 3)

Generates full-color images with multi-section gradients:

**Upper half** (3 vertical sections):
1. Red → White gradient
2. White → Green gradient
3. Blue → White gradient

**Lower half** (2 vertical sections):
1. Black → White gradient
2. White → Black gradient

**Bonus:** Automatically generates three separate PGM files with individual color channels:
- `RED_<filename>.pgm`
- `GREEN_<filename>.pgm`
- `BLUE_<filename>.pgm`

**Examples:**
```bash
./main 3 120 120 colors.ppm 0    # ASCII format + channel files
./main 3 120 120 colors.ppm 1    # Binary format + channel files
```

## Dimension Constraints

| Image Type | Width Constraint | Height Constraint |
|-----------|------------------|-------------------|
| PBM | ≥ 4 and divisible by 4 | ≥ 4 and divisible by 4 |
| PGM | ≥ 4 and divisible by 4 | ≥ 4 and divisible by 4 |
| PPM | ≥ 6 and divisible by 6 | ≥ 4 and divisible by 4 |

## Testing

The Makefile includes several test targets for validation:

```bash
make testValidation  # Test input validation
make testPBM        # Generate sample PBM images
make testPGM        # Generate sample PGM images
make testPPM        # Generate sample PPM images
make testAll        # Run all tests
```

## Error Handling

The program includes comprehensive input validation:

- ✅ Validates number of command-line arguments
- ✅ Checks image type (1, 2, or 3)
- ✅ Verifies output format (0 or 1)
- ✅ Ensures width meets type-specific constraints
- ✅ Validates height requirements
- ✅ Handles memory allocation failures
- ✅ Reports file save errors

## Project Structure

```
main/
├── main.c          # Main program logic
├── libpnm.c        # PNM library implementation
├── libpnm.h        # PNM library header
├── makefile        # Build configuration
└── images/         # Output directory for generated images
    ├── pbm/        # PBM samples
    ├── pgm/        # PGM samples
    ├── ppm/        # PPM samples
    └── ppm_to_pgm/ # Extracted color channels
```

## Dependencies

- Standard C library
- GCC compiler (or compatible C compiler)
- Make build system

The project includes a custom `libpnm` library for PNM image operations:
- `libpnm.h`: Data structures and function declarations
- `libpnm.c`: Core image generation and manipulation functions

## Viewing Generated Images

View the generated images with any tool that supports Netpbm formats:

- **GIMP**: Full support for PBM, PGM, and PPM
- **ImageMagick**: Command-line image manipulation
- **XnView**: Cross-platform image viewer
- **IrfanView**: Windows image viewer
- **Preview** (macOS): Native support for PNM formats

## Tips & Tricks

- **File Size**: Use raw format (`1`) for smaller file sizes, especially for large images
- **Debugging**: Use ASCII format (`0`) to inspect image data in a text editor
- **Testing**: Start with smaller dimensions (e.g., 120×120) for faster generation
- **Color Analysis**: Use PPM type to automatically extract RGB channels as separate grayscale images

## Example Workflow

1. **Build the project:**
   ```bash
   cd main
   make all
   ```

2. **Generate test patterns:**
   ```bash
   ./main 1 120 120 test_pattern.pbm 0
   ./main 2 200 200 gradient_test.pgm 0
   ./main 3 120 120 color_test.ppm 1
   ```

3. **Run comprehensive tests:**
   ```bash
   make testAll
   ```

4. **View the generated images:**
   ```bash
   open images/pbm/square.pbm    # macOS
   # or use your preferred image viewer
   ```

## License

This project is available under the terms specified in the LICENSE file.

## Contributing

Contributions, issues, and feature requests are welcome! Feel free to check the issues page or submit a pull request.
