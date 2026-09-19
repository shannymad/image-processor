#include <iostream>
#include <string>

#include "bmp_io.h"
#include "image.h"
#include "pipeline.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: image_processor input.bmp output.bmp [filters...]\n";
        std::cerr << "Filters:\n";
        std::cerr << "  -crop width height\n";
        std::cerr << "  -gs\n";
        std::cerr << "  -neg\n";
        std::cerr << "  -sharp\n";
        std::cerr << "  -edge threshold\n";
        std::cerr << "  -blur sigma\n";
        std::cerr << "  -crystallize radius\n";
        std::cerr << "  -glass amplitude period\n";
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    try {
        Image image = ReadBMP(input_path);

        Pipeline pipeline = Pipeline::FromArgs(argc, argv);
        pipeline.Apply(image);

        WriteBMP(image, output_path);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
