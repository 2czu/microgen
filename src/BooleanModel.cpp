#include "../include/microgen/BooleanModel.hpp"

BooleanModel::BooleanModel()
{
    this->_size = 64;
    this->_n = 50;
    this->_radius = 6.0;
    this->_seed = 42;
}

BooleanModel::~BooleanModel()
{

}

BooleanModel &BooleanModel::operator=(const BooleanModel &other)
{
    if (this != &other)
    {
        this->_size = other.getSize();
        this->_n = other.getSpheres();
        this->_radius = other.getRadius();
        this->_seed = other.getSeed();
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

std::ostream &operator<<(std::ostream &out, const BooleanModel &model)
{
    unsigned int size = model.getSize();

    out << "Current model :\n"
        << "\tVolume size : " << size << "x" << size << "x" << size << "\n"
        << "\tSpheres amount : " << model.getSpheres() << "\n"
        << "\tSpheres radius : " << model.getRadius() << "\n"
        << "\tSeed : " << model.getSeed() << std::endl;
    return out;
}