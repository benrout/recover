#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Creates new data type to store a byte of data
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for correct number of command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open input file and store pointer
    FILE *file = fopen(argv[1], "r");

    // Check if file can be opened for reading
    if (file == NULL)
    {
        printf("File cannot be opened for reading\n");
        return 1;
    }

    // Initialise file buffer
    BYTE buffer[512];

    // Initialise file count
    int file_count = 0;

    // Initiliase filename for JPEG
    char filename[8];

    // Initialise output file pointer
    FILE *img = NULL;

    // Loop until end of input file, Read 512 bytes from file into buffer
    while (fread(buffer, 512, 1, file) == 1)
    {
        // Check if start of new JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Increment file_count
            file_count++;

            // Print correct string format to filename
            sprintf(filename, "%03i.jpg", file_count - 1);

            // If first JPEG
            if (img == NULL)
            {
                // Open first JPEG file and get pointer
                img = fopen(filename, "w");

                // Write to output file from buffer
                fwrite(buffer, 512, 1, img);
            }
            else
            {
                // Close current JPEG file
                fclose(img);

                // Open new image file
                img = fopen(filename, "w");

                // Write to new jpg file
                fwrite(buffer, 512, 1, img);
            }
        }
        // If not the start of a new JPEG file
        else
        {
            // If output file currently open
            if (img != NULL)
            {
                // Continue writing to file
                fwrite(buffer, 512, 1, img);
            }
        }

    }

    // Close input file
    fclose(file);

    // Close output file
    fclose(img);

    return 0;
}