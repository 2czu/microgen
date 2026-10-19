#include <BooleanModel.hpp>
#include <gtest/gtest.h>

struct BooleanModelTest : public ::testing::Test
{
    void SetUp() override {

    }

    void TearDown() override
    {

    }

    BooleanModel m;
};


TEST_F(BooleanModelTest, PorosityEmptyVolume)
{
    // Arrange
    m.setSpheres(0);
    
    // Act
    m.generate();

    // Result
    EXPECT_EQ(m.porosity(), 1);
}

TEST_F(BooleanModelTest, PorosityWithoutGeneration)
{
    EXPECT_EQ(m.porosity(), 1);
}

TEST_F(BooleanModelTest, RadiusEqualToZero)
{
    m.setRadius(0.0);
    m.generate();

    unsigned int count = std::count(m.getVolume().begin(), m.getVolume().end(), 1);

    EXPECT_EQ(count, 0) << "With radius equal to 0, count of 1's should be 0";
}

TEST_F(BooleanModelTest, VolumeSizeEqualZero)
{
    m.setSize(0);
    m.generate();

    EXPECT_EQ(m.getVolume().size(), 0);
}

TEST(BooleanModel, DifferentSeedDifferentVolume)
{
    BooleanModel a(16, 5, 3.0, 41);
    BooleanModel b(16, 5, 3.0, 42);
    a.generate();
    b.generate();
    EXPECT_NE(a.getVolume(), b.getVolume());
}

TEST(BooleanModel, SameSeedSameVolume) {
    BooleanModel a(16, 5, 3.0, 42);
    BooleanModel b(16, 5, 3.0, 42);
    a.generate();
    b.generate();
    EXPECT_EQ(a.getVolume(), b.getVolume());
}


