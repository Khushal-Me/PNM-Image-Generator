#include "libpnm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
int validate_dimensions(int img_type, int width, int height);
void generate_pbm_image(int width, int height, const char* filename, int format);
void generate_pgm_image(int width, int height, const char* filename, int format);
void generate_ppm_image(int width, int height, const char* filename, int format);

int main(int argc, char *argv[]) {
    // Command line arguments:
    // argv[1]: image type (1=PBM, 2=PGM, 3=PPM)
    // argv[2]: width
    // argv[3]: height  
    // argv[4]: output filename
    // argv[5]: format (0=ASCII, 1=raw)
    
    if (argc != 6) {
        printf("Error: Invalid number of arguments\n");
        printf("Usage: %s <img_type> <width> <height> <filename> <format>\n", argv[0]);
        printf("  img_type: 1=PBM, 2=PGM, 3=PPM\n");
        printf("  format: 0=ASCII, 1=raw\n");
        return 0;
    }
    
    // Parse arguments
    int img_type = atoi(argv[1]);
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    char *filename = argv[4];
    int format = atoi(argv[5]);
    
    // Validate image type
    if (img_type < 1 || img_type > 3) {
        printf("Error: Invalid image type. Use 1=PBM, 2=PGM, or 3=PPM\n");
        return 0;
    }
    
    // Validate format
    if (format != 0 && format != 1) {
        printf("Error: Invalid format. Use 0=ASCII or 1=raw\n");
        return 0;
    }
    
    // Validate dimensions
    if (!validate_dimensions(img_type, width, height)) {
        return 0;
    }
    
    // Generate appropriate image type
    switch (img_type) {
        case 1:
            generate_pbm_image(width, height, filename, format);
            break;
        case 2:
            generate_pgm_image(width, height, filename, format);
            break;
        case 3:
            generate_ppm_image(width, height, filename, format);
            break;
    }
    
    printf("Image generated successfully: %s\n", filename);
    return 0;
}

int validate_dimensions(int img_type, int width, int height) {
    // Check height constraint (all types)
    if (height < 4 || height % 4 != 0) {
        printf("Error: Height must be divisible by 4 and >= 4 (given: %d)\n", height);
        return 0;
    }
    
    // Check width constraints by image type
    switch (img_type) {
        case 1: // PBM
        case 2: // PGM
            if (width < 4 || width % 4 != 0) {
                printf("Error: Width for PBM/PGM must be divisible by 4 and >= 4 (given: %d)\n", width);
                return 0;
            }
            break;
        case 3: // PPM
            if (width < 6 || width % 6 != 0) {
                printf("Error: Width for PPM must be divisible by 6 and >= 6 (given: %d)\n", width);
                return 0;
            }
            break;
    }
    
    return 1;
}

void generate_pbm_image(int width, int height, const char* filename, int format) {
    struct PBM_Image img;
    int row, col;
    
    // Create the PBM image structure
    if (create_PBM_Image(&img, width, height) != 0) {
        printf("Error: Failed to create PBM image\n");
        return;
    }
    
    // Calculate rectangle boundaries
    // Rectangle is centered and has dimensions width/2 × height/2
    int rect_width = width / 2;
    int rect_height = height / 2;
    int rect_left = (width - rect_width) / 2;
    int rect_right = rect_left + rect_width - 1;
    int rect_top = (height - rect_height) / 2;
    int rect_bottom = rect_top + rect_height - 1;
    
    // Initialize entire image to black (1)
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            img.image[row][col] = BLACK;
        }
    }
    
    // Draw white rectangle (0) in the center
    for (row = rect_top; row <= rect_bottom; row++) {
        for (col = rect_left; col <= rect_right; col++) {
            img.image[row][col] = WHITE;
        }
    }
    
    // Draw two solid diagonal lines
    // Diagonal 1: Top-left to bottom-right
    for (row = 0; row < height; row++) {
        // Calculate column position for this row on the diagonal
        // Line equation: col = (row * width) / height
        col = (row * width) / height;
        if (col < width) {
            img.image[row][col] = BLACK;
        }
    }
    
    // Diagonal 2: Top-right to bottom-left
    for (row = 0; row < height; row++) {
        // Calculate column position for this row on the diagonal
        // Line equation: col = width - 1 - (row * width) / height
        col = width - 1 - (row * width) / height;
        if (col >= 0 && col < width) {
            img.image[row][col] = BLACK;
        }
    }
    
    // Save the image (format: true = raw, false = ASCII)
    bool raw_format = (format == 1) ? true : false;
    if (save_PBM_Image(&img, (char*)filename, raw_format) != 0) {
        printf("Error: Failed to save PBM image\n");
    }
    
    // Free allocated memory
    free_PBM_Image(&img);
}

void generate_pgm_image(int width, int height, const char* filename, int format) {
    struct PGM_Image img;
    int row, col;
    
    // Create the PGM image structure with max gray value of 255
    if (create_PGM_Image(&img, width, height, 255) != 0) {
        printf("Error: Failed to create PGM image\n");
        return;
    }
    
    // Calculate inner rectangle boundaries
    // Rectangle is centered and has dimensions width/2 × height/2
    int rect_width = width / 2;
    int rect_height = height / 2;
    int rect_left = (width - rect_width) / 2;
    int rect_right = rect_left + rect_width - 1;
    int rect_top = (height - rect_height) / 2;
    int rect_bottom = rect_top + rect_height - 1;
    
    // Center point of the image (also center of rectangle)
    int center_row = height / 2;
    int center_col = width / 2;
    
    // Fill the image
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            
            // Check if pixel is inside the inner rectangle
            if (row >= rect_top && row <= rect_bottom && 
                col >= rect_left && col <= rect_right) {
                
                // Relative positions from center (can be negative)
                int row_offset = row - center_row;
                int col_offset = col - center_col;
                
                // Determine which triangle by comparing the diagonal lines
                // The four triangles are separated by two diagonals through the center
                
                // Check which side of the diagonals we're on
                // Diagonal 1: from top-left corner to bottom-right corner (slope = height/width)
                // Diagonal 2: from top-right corner to bottom-left corner (slope = -height/width)
                
                // For a point (col_offset, row_offset) relative to center:
                // Above/below diagonal 1: compare row_offset with (col_offset * rect_height / rect_width)
                // Above/below diagonal 2: compare row_offset with (-col_offset * rect_height / rect_width)
                
                if (row_offset <= (col_offset * rect_height) / rect_width && 
                    row_offset <= (-col_offset * rect_height) / rect_width) {
                    // UPPER triangle: gradient from white (top edge) to black (center)
                    int dist_from_edge = row - rect_top;
                    int max_dist = center_row - rect_top;
                    img.image[row][col] = 255 - (dist_from_edge * 255) / max_dist;
                    
                } else if (row_offset >= (col_offset * rect_height) / rect_width && 
                           row_offset >= (-col_offset * rect_height) / rect_width) {
                    // LOWER triangle: gradient from black (center) to white (bottom edge)
                    int dist_from_center = row - center_row;
                    int max_dist = rect_bottom - center_row;
                    img.image[row][col] = (dist_from_center * 255) / max_dist;
                    
                } else if (col_offset < 0) {
                    // LEFT triangle: gradient from white (left edge) to black (center)
                    int dist_from_edge = col - rect_left;
                    int max_dist = center_col - rect_left;
                    img.image[row][col] = 255 - (dist_from_edge * 255) / max_dist;
                    
                } else {
                    // RIGHT triangle: gradient from black (center) to white (right edge)
                    int dist_from_center = col - center_col;
                    int max_dist = rect_right - center_col;
                    img.image[row][col] = (dist_from_center * 255) / max_dist;
                }
                
            } else {
                // Outside rectangle - set to black (0)
                img.image[row][col] = 0;
            }
        }
    }
    
    // Save the image (format: true = raw, false = ASCII)
    bool raw_format = (format == 1) ? true : false;
    if (save_PGM_Image(&img, (char*)filename, raw_format) != 0) {
        printf("Error: Failed to save PGM image\n");
    }
    
    // Free allocated memory
    free_PGM_Image(&img);
}

void generate_ppm_image(int width, int height, const char* filename, int format) {
    struct PPM_Image img;
    int row, col;
    
    // Create the PPM image structure with max color value of 255
    if (create_PPM_Image(&img, width, height, 255) != 0) {
        printf("Error: Failed to create PPM image\n");
        return;
    }
    
    // Calculate section boundaries
    int half_height = height / 2;
    
    // Upper half has 3 sections (each width/3)
    int section_width_upper = width / 3;
    
    // Lower half has 2 sections (each width/2)
    int section_width_lower = width / 2;
    
    // Fill the image
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            
            if (row < half_height) {
                // UPPER HALF - 3 sections
                // Gradients change vertically (top to bottom) in each section
                
                if (col < section_width_upper) {
                    // Section 1: Red → White (top to bottom)
                    // Red (255,0,0) gradually becomes White (255,255,255)
                    // G and B channels go from 0 to 255
                    int progress = (row * 255) / half_height;
                    img.image[row][col][RED] = 255;
                    img.image[row][col][GREEN] = progress;
                    img.image[row][col][BLUE] = progress;
                    
                } else if (col < 2 * section_width_upper) {
                    // Section 2: White → Green (top to bottom)
                    // White (255,255,255) gradually becomes Green (0,255,0)
                    // R and B channels go from 255 to 0
                    int progress = (row * 255) / half_height;
                    img.image[row][col][RED] = 255 - progress;
                    img.image[row][col][GREEN] = 255;
                    img.image[row][col][BLUE] = 255 - progress;
                    
                } else {
                    // Section 3: Blue → White (top to bottom)
                    // Blue (0,0,255) gradually becomes White (255,255,255)
                    // R and G channels go from 0 to 255
                    int progress = (row * 255) / half_height;
                    img.image[row][col][RED] = progress;
                    img.image[row][col][GREEN] = progress;
                    img.image[row][col][BLUE] = 255;
                }
                
            } else {
                // LOWER HALF - 2 sections
                // Gradients change vertically (top to bottom) in each section
                int local_row = row - half_height;
                
                if (col < section_width_lower) {
                    // Section 1: Black → White (top to bottom)
                    // Black (0,0,0) gradually becomes White (255,255,255)
                    int progress = (local_row * 255) / (height - half_height);
                    img.image[row][col][RED] = progress;
                    img.image[row][col][GREEN] = progress;
                    img.image[row][col][BLUE] = progress;
                    
                } else {
                    // Section 2: White → Black (top to bottom)
                    // White (255,255,255) gradually becomes Black (0,0,0)
                    int progress = (local_row * 255) / (height - half_height);
                    img.image[row][col][RED] = 255 - progress;
                    img.image[row][col][GREEN] = 255 - progress;
                    img.image[row][col][BLUE] = 255 - progress;
                }
            }
        }
    }
    
    // Save the image (format: true = raw, false = ASCII)
    bool raw_format = (format == 1) ? true : false;
    if (save_PPM_Image(&img, (char*)filename, raw_format) != 0) {
        printf("Error: Failed to save PPM image\n");
    }
    
    // Convert and store the generated image into 3 gray images using copy_PPM_to_PGM
    struct PGM_Image red_channel, green_channel, blue_channel;
    
    // Create filename for RED channel
    char red_filename[256];
    snprintf(red_filename, sizeof(red_filename), "RED_%s", filename);
    // Replace .ppm extension with .pgm
    char *ext = strrchr(red_filename, '.');
    if (ext != NULL && strcmp(ext, ".ppm") == 0) {
        strcpy(ext, ".pgm");
    }
    
    // Create filename for GREEN channel
    char green_filename[256];
    snprintf(green_filename, sizeof(green_filename), "GREEN_%s", filename);
    ext = strrchr(green_filename, '.');
    if (ext != NULL && strcmp(ext, ".ppm") == 0) {
        strcpy(ext, ".pgm");
    }
    
    // Create filename for BLUE channel
    char blue_filename[256];
    snprintf(blue_filename, sizeof(blue_filename), "BLUE_%s", filename);
    ext = strrchr(blue_filename, '.');
    if (ext != NULL && strcmp(ext, ".ppm") == 0) {
        strcpy(ext, ".pgm");
    }
    
    // Extract and save RED channel
    if (create_PGM_Image(&red_channel, width, height, 255) == 0) {
        if (copy_PPM_to_PGM(&img, &red_channel, RED) == 0) {
            if (save_PGM_Image(&red_channel, red_filename, raw_format) == 0) {
                printf("RED channel saved: %s\n", red_filename);
            } else {
                printf("Error: Failed to save RED channel image\n");
            }
        }
        free_PGM_Image(&red_channel);
    }
    
    // Extract and save GREEN channel
    if (create_PGM_Image(&green_channel, width, height, 255) == 0) {
        if (copy_PPM_to_PGM(&img, &green_channel, GREEN) == 0) {
            if (save_PGM_Image(&green_channel, green_filename, raw_format) == 0) {
                printf("GREEN channel saved: %s\n", green_filename);
            } else {
                printf("Error: Failed to save GREEN channel image\n");
            }
        }
        free_PGM_Image(&green_channel);
    }
    
    // Extract and save BLUE channel
    if (create_PGM_Image(&blue_channel, width, height, 255) == 0) {
        if (copy_PPM_to_PGM(&img, &blue_channel, BLUE) == 0) {
            if (save_PGM_Image(&blue_channel, blue_filename, raw_format) == 0) {
                printf("BLUE channel saved: %s\n", blue_filename);
            } else {
                printf("Error: Failed to save BLUE channel image\n");
            }
        }
        free_PGM_Image(&blue_channel);
    }
    
    // Free allocated memory
    free_PPM_Image(&img);
}