#pragma once

# include <iostream>

class BooleanModel
{
    // Attributes

    private:

        unsigned int   _size;
        unsigned int   _n;
        double   _radius;
        size_t         _seed;

    public:

        // Constructors

        BooleanModel();
        BooleanModel(const BooleanModel &other);
        BooleanModel& operator=(const BooleanModel &other);

        // Destructor

        ~BooleanModel();

        // Accessors

        const unsigned int getSize() const;
        const unsigned int getSpheres() const;
        const double getRadius() const;
        const size_t       getSeed() const;

        void               setSize(const unsigned int &size);
        void               setSpheres(const unsigned int &n);
        void               setRadius(const double &radius);
        void               setSeed(const size_t &seed);

        // Methods

        unsigned int       idx(unsigned int x, unsigned int y, unsigned int z);
        void               generate();
        double             porosity();



};

std::ostream &operator<<(std::ostream &out, const BooleanModel &model);