#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct {
    int width;
    int height;
    char background;
    char *canvas;
} t_canvas;

// Initialize canvas with background character
int init_canvas(t_canvas *canvas) {
    int i = 0;
    
    canvas->canvas = (char*)malloc(canvas->width * canvas->height * sizeof(char));
    if (!canvas->canvas)
        return 0;
    
    while (i < canvas->width * canvas->height) {
        canvas->canvas[i] = canvas->background;
        i++;
    }
    return 1;
}

// Check if a point is inside a rectangle
int is_in_rectangle(float x, float y, float rect_x, float rect_y, float rect_width, float rect_height) {
    return (x >= rect_x && x <= rect_x + rect_width && 
            y >= rect_y && y <= rect_y + rect_height);
}

// Check if a point is on the border of a rectangle
int is_on_border(float x, float y, float rect_x, float rect_y, float rect_width, float rect_height) {
    // Check if the point is inside the rectangle
    if (x < rect_x || x > rect_x + rect_width || y < rect_y || y > rect_y + rect_height)
        return 0;
    
    // Check if the point is close to any border
    if (x - rect_x < 1.0f || rect_x + rect_width - x < 1.0f || 
        y - rect_y < 1.0f || rect_y + rect_height - y < 1.0f)
        return 1;
    
    return 0;
}

// Draw a rectangle on the canvas
void draw_rectangle(t_canvas *canvas, char type, float x, float y, float width, float height, char draw_char) {
    int i = 0;
    int j;
    
    while (i < canvas->height) {
        j = 0;
        while (j < canvas->width) {
            // For filled rectangle
            if (type == 'R' && is_in_rectangle(j, i, x, y, width, height))
                canvas->canvas[i * canvas->width + j] = draw_char;
            // For empty rectangle
            else if (type == 'r' && is_on_border(j, i, x, y, width, height))
                canvas->canvas[i * canvas->width + j] = draw_char;
            j++;
        }
        i++;
    }
}

// Print the canvas to stdout
void print_canvas(t_canvas *canvas) {
    int i = 0;
    
    while (i < canvas->height) {
        write(1, canvas->canvas + (i * canvas->width), canvas->width);
        write(1, "\n", 1);
        i++;
    }
}

int main(int argc, char *argv[]) {
    FILE *file;
    t_canvas canvas;
    char type;
    float x, y, width, height;
    char draw_char;
    int ret;
    
    // Check if we have exactly one argument
    if (argc != 2) {
        write(1, "Error: argument\n", 16);
        return 1;
    }
    
    // Open the operation file
    file = fopen(argv[1], "r");
    if (!file) {
        write(1, "Error: Operation file corrupted\n", 31);
        return 1;
    }
    
    // Read the first line to get canvas dimensions and background character
    ret = fscanf(file, "%d %d %c", &canvas.width, &canvas.height, &canvas.background);
    if (ret != 3 || canvas.width <= 0 || canvas.width > 300 || 
        canvas.height <= 0 || canvas.height > 300) {
        fclose(file);
        write(1, "Error: Operation file corrupted\n", 31);
        return 1;
    }
    
    // Initialize the canvas
    if (!init_canvas(&canvas)) {
        fclose(file);
        write(1, "Error: Operation file corrupted\n", 31);
        return 1;
    }
    
    // Read and process each operation line
    while (1) {
        ret = fscanf(file, " %c %f %f %f %f %c", &type, &x, &y, &width, &height, &draw_char);
        
        // Check for end of file
        if (ret == EOF)
            break;
        
        // Check if we read all 6 values correctly
        if (ret != 6 || (type != 'r' && type != 'R') || width <= 0 || height <= 0) {
            free(canvas.canvas);
            fclose(file);
            write(1, "Error: Operation file corrupted\n", 31);
            return 1;
        }
        
        draw_rectangle(&canvas, type, x, y, width, height, draw_char);
    }
    
    // Print the canvas
    print_canvas(&canvas);
    
    // Clean up
    free(canvas.canvas);
    fclose(file);
    
    return 0;
}
