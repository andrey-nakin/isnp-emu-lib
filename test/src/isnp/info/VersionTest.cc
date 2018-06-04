#include <sstream>
#include <gtest/gtest.h>
#include "isnp/info/Version.hh"

namespace isnp {

namespace info {

TEST(Version, Defines)
{
	EXPECT_TRUE(ISNPEMULIB_VERSION_MAJOR >= 0);
	EXPECT_TRUE(ISNPEMULIB_VERSION_MINOR >= 0);
	EXPECT_TRUE(ISNPEMULIB_VERSION_PATCH >= 0);
}

TEST(Version, Get)
{
	const std::vector<int> ver = Version::Get();
	EXPECT_EQ(3, ver.size());
	EXPECT_EQ(ISNPEMULIB_VERSION_MAJOR, ver[0]);
	EXPECT_EQ(ISNPEMULIB_VERSION_MINOR, ver[1]);
	EXPECT_EQ(ISNPEMULIB_VERSION_PATCH, ver[2]);
}

TEST(Version, GetAsString)
{
	std::stringstream s;
	s
	<< ISNPEMULIB_VERSION_MAJOR
	<< '.'
	<< ISNPEMULIB_VERSION_MINOR
	<< '.'
	<< ISNPEMULIB_VERSION_PATCH;
	const G4String ver = Version::GetAsString();
	EXPECT_EQ(s.str(), ver);
}

TEST(Version, Matches)
{
	EXPECT_TRUE(
			Version::Matches(ISNPEMULIB_VERSION_MAJOR, ISNPEMULIB_VERSION_MINOR, ISNPEMULIB_VERSION_PATCH)
	);
	EXPECT_TRUE(
			Version::Matches(ISNPEMULIB_VERSION_MAJOR, ISNPEMULIB_VERSION_MINOR)
	);
	EXPECT_TRUE(
			Version::Matches(ISNPEMULIB_VERSION_MAJOR)
	);
	EXPECT_FALSE(
			Version::Matches(ISNPEMULIB_VERSION_MAJOR + 1, ISNPEMULIB_VERSION_MINOR, ISNPEMULIB_VERSION_PATCH)
	);
	EXPECT_FALSE(
			Version::Matches(ISNPEMULIB_VERSION_MAJOR, ISNPEMULIB_VERSION_MINOR + 1, ISNPEMULIB_VERSION_PATCH)
	);
	EXPECT_FALSE(
			Version::Matches(ISNPEMULIB_VERSION_MAJOR, ISNPEMULIB_VERSION_MINOR, ISNPEMULIB_VERSION_PATCH + 1)
	);
}

TEST(Version, GetDateAsString)
{
	EXPECT_FALSE(Version::GetDateAsString().isNull());
}

}

}
