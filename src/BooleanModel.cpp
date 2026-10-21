#include "../include/BooleanModel.hpp"

BooleanModel::~BooleanModel()
{

}

BooleanModel::BooleanModel()
{
    this->_size = 64;
    this->_n = 50;
    this->_radius = 6.0;
    this->_seed = 42;
    this->_outfileName = "vol.bin";
    this->_outdir = "out";
}

BooleanModel::BooleanModel(unsigned int size, unsigned int n, double radius, size_t seed)
{
    this->_size = size;
    this->_n = n;
    this->_radius = radius;
    this->_seed = seed;
    this->_outfileName = "vol.bin";
    this->_outdir = "out";
}

BooleanModel::BooleanModel(unsigned int size, unsigned int n, double radius, size_t seed, std::string outfileName)
{
    this->_size = size;
    this->_n = n;
    this->_radius = radius;
    this->_seed = seed;
    this->_outfileName = outfileName;
    this->_outdir = "out";
}


BooleanModel::BooleanModel(unsigned int size, unsigned int n, double radius, size_t seed, std::string outfileName, std::string outdir)
{
    this->_size = size;
    this->_n = n;
    this->_radius = radius;
    this->_seed = seed;
    this->_outfileName = outfileName;
    this->_outdir = outdir;
}




BooleanModel::BooleanModel(const BooleanModel &other)
{
    *this = other;
}


BooleanModel &BooleanModel::operator=(const BooleanModel &other)
{
    if (this != &other)
    {
        this->_size = other.getSize();
        this->_n = other.getSpheres();
        this->_radius = other.getRadius();
        this->_seed = other.getSeed();
        this->_outfileName = other._outfileName;
        this->_outdir = other._outdir;
        this->_volume = other._volume;
    }
    return *this;
}

const unsigned int BooleanModel::getSize() const
{
    return this->_size;
}

const unsigned int BooleanModel::getSpheres() const
{
    return this->_n;
}

const double BooleanModel::getRadius() const
{
    return this->_radius;
}

const size_t       BooleanModel::getSeed() const
{
    return this->_seed;
}

const std::string       &BooleanModel::getOutfileName() const
{
    return this->_outfileName;
}

const std::vector<uint8_t> &BooleanModel::getVolume() const
{
    return this->_volume;
}

void               BooleanModel::setSize(const unsigned int size)
{
    this->_size = size;
}

void               BooleanModel::setSpheres(const unsigned int n)
{
    this->_n = n;
}

void               BooleanModel::setRadius(const double radius)
{
    this->_radius = radius;
}

void               BooleanModel::setSeed(const size_t seed)
{
    this->_seed = seed;
}


std::ostream &operator<<(std::ostream &out, const BooleanModel &model)
{
    unsigned int size = model.getSize();

    out << "Current model :\n"
        << "\tVolume size : " << size << "x" << size << "x" << size << "\n"
        << "\tSpheres amount : " << model.getSpheres() << "\n"
        << "\tSpheres radius : " << model.getRadius() << "\n"
        << "\tSeed : " << model.getSeed() << "\n"
        << "\tOutfile name : " << model.getOutfileName() << std::endl;
    return out;
}

size_t              BooleanModel::idx(int x, int y, int z) const { return x + _size * (y + _size * z); }

void                BooleanModel::generate()
{
    std::mt19937 gen(this->_seed);
    std::uniform_real_distribution<double> dist(0.0, this->_size);

    _volume.assign(this->_size * this->_size * this->_size, 0);

    for (int i = 0; i < this->_n; i++)
    {
        double cx = dist(gen);
        double cy = dist(gen);
        double cz = dist(gen);
        addSphere(cx, cy, cz);
    }
}

void                BooleanModel::addSphere(double cx, double cy, double cz)
{
    double r2 = this->_radius * this->_radius;

    const int x0 = std::max(0,         static_cast<int>(std::floor(cx - _radius)));
    const int x1 = std::min(static_cast<int>(_size) - 1, static_cast<int>(std::ceil (cx + _radius)));
    const int y0 = std::max(0,         static_cast<int>(std::floor(cy - _radius)));
    const int y1 = std::min(static_cast<int>(_size) - 1, static_cast<int>(std::ceil (cy + _radius)));
    const int z0 = std::max(0,         static_cast<int>(std::floor(cz - _radius)));
    const int z1 = std::min(static_cast<int>(_size) - 1, static_cast<int>(std::ceil (cz + _radius)));

    for (double x = x0; x <= x1; ++x)
        for (double y = y0; y <= y1; ++y)
            for (double z = z0; z <= z1; ++z)
            {
                // obsolete
                // double phi = M_PI * 1;
                // double theta = 2 * M_PI * 1;
                
                // double x = _radius * sin(phi) * cos(theta);
                // double y = _radius * cos(phi);
                // double z = _radius * sin(phi) * sin(theta);
                const double dx = x - cx, dy = y - cy, dz = z - cz;

                if (dx*dx + dy*dy + dz*dz <= r2)
                    _volume[idx(x, y, z)] = 1;
            }
}

void               BooleanModel::writeToFile()
{
    const std::filesystem::path outputDir = _outdir.empty()
        ? std::filesystem::path(".")
        : std::filesystem::path(_outdir);
    const std::filesystem::path outputFile = outputDir / _outfileName;

    if (!std::filesystem::is_directory(outputDir))
    {
        std::cerr << "Output directory does not exist or is not a directory: "
                  << outputDir << "." << std::endl;
        return;
    }

    std::ofstream f(outputFile, std::ios::binary);

    if (f.is_open())
    {
        f.write(reinterpret_cast<const char*>(_volume.data()), _volume.size());
        f.close();
        std::cout << "Successfully wrote volume information to " << outputFile << "!" << std::endl;
    }
    else
    {
        std::cerr << "Couldn't open " << _outfileName << "." << std::endl;
    }

    json meta = {
        {"size", this->_size},
        {"spheres", this->_n},
        {"radius", this->_radius},
        {"seed", this->_seed},
        {"porosity", porosity()},
        {"outfile", this->_outfileName},
        {"outdir", this->_outdir},
    };

    const std::filesystem::path metaFile = outputDir / "meta.json";
    std::ofstream fjson(metaFile);
    if (fjson.is_open())
    {
        fjson << meta.dump(4);
        fjson.close();
        std::cout << "Successfully wrote model information to " << metaFile << std::endl;
    }
    else {
        std::cerr << "Couldn't open meta.json." << std::endl;
    }

}

double          BooleanModel::porosity() const
{
    if (_volume.empty())
        return 1; // maybe change to use an exception isntead

    const size_t empty = std::count(_volume.begin(), _volume.end(), 0);
    return static_cast<double>(empty) / static_cast<double>(_volume.size());
}