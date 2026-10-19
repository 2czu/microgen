#pragma once

# include <iostream>
# include <random>
# include <vector>
# include <cmath>
# include <fstream>
# include <algorithm>

class BooleanModel
{
    // Attributes

    private:

        unsigned int          _size;
        unsigned int          _n;
        double                _radius;
        size_t                _seed;
        std::string           _outfileName;
        std::vector<uint8_t>  _volume;

    public:

        // Constructors

        BooleanModel();
        BooleanModel(unsigned int size, unsigned int n, double radius, size_t seed);
        BooleanModel(unsigned int size, unsigned int n, double radius, size_t seed, std::string outfileName);
        BooleanModel(const BooleanModel &other);
        BooleanModel& operator=(const BooleanModel &other);

        // Destructor

        ~BooleanModel();

        // Accessors

        const unsigned int getSize() const;
        const unsigned int getSpheres() const;
        const double       getRadius() const;
        const size_t       getSeed() const;
        const std::string  &getOutfileName() const;
        const std::vector<uint8_t> &getVolume() const;

        void               setSize(const unsigned int size);
        void               setSpheres(const unsigned int n);
        void               setRadius(const double radius);
        void               setSeed(const size_t seed);

        // Methods

        size_t             idx(int x, int y, int z) const;
        void               generate();
        double             porosity() const;
        void               addSphere(double cx, double cy, double cz);
        void               writeToFile();

};

std::ostream &operator<<(std::ostream &out, const BooleanModel &model);