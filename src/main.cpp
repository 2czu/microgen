#include <string>
#include <iostream>
#include "../include/BooleanModel.hpp"
#include <CLI/CLI.hpp>

int main(int argc, char **argv)
{
    CLI::App app{"3D microstructures generator"};
    argv = app.ensure_utf8(argv);

    unsigned int size = 64, n = 50;
    double radius = 6;
    size_t seed = 42;
    std::string outfileName = "vol.bin";
    std::string outdir = "out";

    app.add_option("--sz, --size", size, "The voxels volume's size");
    app.add_option("--n, --spheres", n, "Number of spheres inside the volume");
    app.add_option("--r, --radius", radius, "Radius of the spheres");
    app.add_option("--seed", seed, "Spheres generating seed");
    app.add_option("--outfile", outfileName, "Volume binary outfile name");
    app.add_option("--outdir", outdir, "Output directory");

    CLI11_PARSE(app, argc, argv);

    BooleanModel model(size, n, radius, seed, outfileName, outdir);

    std::cout << model;

    model.generate();
    model.writeToFile();

    return 0;
}