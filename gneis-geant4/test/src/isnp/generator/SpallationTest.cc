#include <cmath>

#include <gtest/gtest.h>

#include <G4RunManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4UImanager.hh>

#include <isnp/generator/Spallation.hh>
#include "isnp/facility/component/SpallationTarget.hh"
#include "isnp/testutil/Stat.hh"

namespace isnp {

namespace generator {

TEST(Spallation, Diameter) {

	Spallation spallation;

	EXPECT_DOUBLE_EQ(40 * mm,
			spallation.GetUniformCircle().GetProps().GetDiameter());

	spallation.GetUniformCircle().GetProps().SetDiameter(50 * mm);
	EXPECT_DOUBLE_EQ(50 * mm,
			spallation.GetUniformCircle().GetProps().GetDiameter());

}

TEST(Spallation, PositionX) {

	using namespace isnp::testutil;

	Spallation spallation;
	spallation.SetMode(Spallation::Mode::UniformCircle);

	EXPECT_DOUBLE_EQ(0 * mm, spallation.GetPositionX());

	spallation.SetPositionX(20 * mm);
	EXPECT_DOUBLE_EQ(20 * mm, spallation.GetPositionX());

	Stat x;
	G4Transform3D const zeroTransform;

	for (int i = 0; i < 100000; i++) {
		auto const pos = spallation.GeneratePosition(zeroTransform);
		x += pos.getX();
	}

	EXPECT_TRUE(x.Is(20.0 * mm));
	EXPECT_NEAR(x.GetMin(),
			-spallation.GetUniformCircle().GetProps().GetDiameter() / 2
					+ 20.0 * mm, 1e-1 * mm);
	EXPECT_NEAR(x.GetMax(),
			spallation.GetUniformCircle().GetProps().GetDiameter() / 2
					+ 20.0 * mm, 1e-1 * mm);
	EXPECT_TRUE(x.GetStd() > 0.99 * cm);

}

TEST(Spallation, PositionY) {

	using namespace isnp::testutil;

	Spallation spallation;
	spallation.SetMode(Spallation::Mode::UniformCircle);

	EXPECT_DOUBLE_EQ(0 * mm, spallation.GetPositionY());

	spallation.SetPositionY(20 * mm);
	EXPECT_DOUBLE_EQ(20 * mm, spallation.GetPositionY());

	Stat y;
	G4Transform3D const zeroTransform;

	for (int i = 0; i < 100000; i++) {
		auto const pos = spallation.GeneratePosition(zeroTransform);
		y += pos.getY();
	}

	EXPECT_TRUE(y.Is(20.0 * mm));
	EXPECT_NEAR(y.GetMin(),
			-spallation.GetUniformCircle().GetProps().GetDiameter() / 2
					+ 20.0 * mm, 1e-1 * mm);
	EXPECT_NEAR(y.GetMax(),
			spallation.GetUniformCircle().GetProps().GetDiameter() / 2
					+ 20.0 * mm, 1e-1 * mm);
	EXPECT_TRUE(y.GetStd() > 0.99 * cm);

}

TEST(Spallation, GeneratePositionStatistics) {

	using namespace isnp::testutil;

	Spallation spallation;
	spallation.SetMode(Spallation::Mode::UniformCircle);

	Stat x, y, z, r;
	G4Transform3D const zeroTransform;

	for (int i = 0; i < 1000000; i++) {
		auto const pos = spallation.GeneratePosition(zeroTransform);
		x += pos.getX();
		y += pos.getY();
		z += pos.getZ();
		r += std::sqrt(pos.getX() * pos.getX() + pos.getY() * pos.getY());
	}

	EXPECT_TRUE(x.Is(0.0 * cm));
	EXPECT_NEAR(-2 * cm, x.GetMin(), 0.001 * cm);
	EXPECT_NEAR(2 * cm, x.GetMax(), 0.001 * cm);
	EXPECT_NEAR(1.0 * cm, x.GetStd(), 0.002 * cm);

	EXPECT_TRUE(y.Is(0.0 * cm));
	EXPECT_NEAR(-2 * cm, y.GetMin(), 0.001 * cm);
	EXPECT_NEAR(2 * cm, y.GetMax(), 0.001 * cm);
	EXPECT_NEAR(1.0 * cm, y.GetStd(), 0.002 * cm);

	EXPECT_DOUBLE_EQ(-250.0 * mm, z.GetMean());

	EXPECT_NEAR(2 * cm, r.GetMax(), 0.01 * cm);
	EXPECT_NEAR(0.47 * cm, r.GetStd(), 0.01 * cm);

}

TEST(Spallation, GenerateDirection) {

	Spallation spallation;

	{
		G4Transform3D const zeroTransform;
		auto const dir = spallation.GenerateDirection(zeroTransform);
		EXPECT_DOUBLE_EQ(0.0, dir.getX());
		EXPECT_DOUBLE_EQ(0.0, dir.getY());
		EXPECT_DOUBLE_EQ(1.0, dir.getZ());
	}

	{
		G4double const angle = 30.0 * deg;
		G4RotationMatrix rotm = G4RotationMatrix();
		rotm.rotateY(angle);
		G4ThreeVector const position = G4ThreeVector(1.0 * m, 1.0 * m, 1.0 * m);
		G4Transform3D const transform = G4Transform3D(rotm, position);
		auto const dir = spallation.GenerateDirection(transform);
		EXPECT_DOUBLE_EQ(std::sin(angle), dir.getX());
		EXPECT_DOUBLE_EQ(0.0, dir.getY());
		EXPECT_DOUBLE_EQ(std::cos(angle), dir.getZ());
	}

}

TEST(Spallation, GeneratePosition) {

	using namespace isnp::testutil;

	{
		Spallation spallation;
		spallation.SetMode(Spallation::Mode::UniformCircle);
		spallation.GetUniformCircle().GetProps().SetDiameter(0.0);

		G4Transform3D const zeroTransform;
		auto const pos = spallation.GeneratePosition(zeroTransform);
		EXPECT_DOUBLE_EQ(0.0, pos.getX());
		EXPECT_DOUBLE_EQ(0.0, pos.getY());
		EXPECT_DOUBLE_EQ(-250 * mm, pos.getZ());
	}

	{
		Spallation spallation;
		spallation.SetMode(Spallation::Mode::UniformCircle);

		G4double const r =
				spallation.GetUniformCircle().GetProps().GetDiameter() / 2;

		G4double const angle = 30.0 * deg;
		G4RotationMatrix rotm = G4RotationMatrix();
		rotm.rotateY(angle);
		G4ThreeVector const position = G4ThreeVector(1.0 * m, 2.0 * m, 3.0 * m);
		G4Transform3D const transform = G4Transform3D(rotm, position);

		Stat x, y, z;

		for (int i = 0; i < 1000000; i++) {
			auto const pos = spallation.GeneratePosition(transform);
			x += pos.getX();
			y += pos.getY();
			z += pos.getZ();
		}

		EXPECT_TRUE(x.Is(std::sin(angle) * -250 * mm + 1 * m));
		EXPECT_NEAR(std::sin(angle) * -250 * mm - std::cos(angle) * r + 1 * m,
				x.GetMin(), 0.01 * mm);
		EXPECT_NEAR(std::sin(angle) * -250 * mm + std::cos(angle) * r + 1 * m,
				x.GetMax(), 0.01 * mm);

		EXPECT_TRUE(y.Is(2 * m));
		EXPECT_NEAR(-2 * cm + 2 * m, y.GetMin(), 0.001 * cm);
		EXPECT_NEAR(2 * cm + 2 * m, y.GetMax(), 0.001 * cm);

		EXPECT_TRUE(z.Is(std::cos(angle) * -250 * mm + 3 * m));
		EXPECT_NEAR(std::cos(angle) * -250 * mm - std::sin(angle) * r + 3 * m,
				z.GetMin(), 0.01 * mm);
		EXPECT_NEAR(std::cos(angle) * -250 * mm + std::sin(angle) * r + 3 * m,
				z.GetMax(), 0.01 * mm);
	}

}

TEST(Spallation, DetectTargetTransform) {

	auto const uiManager = G4UImanager::GetUIpointer();
	EXPECT_EQ(0, uiManager->ApplyCommand("/isnp/facility basicSpallation"));

	auto const facility = facility::component::SpallationTarget::GetInstance();
	EXPECT_TRUE(facility != nullptr);

	auto const saveRotation = facility->GetRotation(), savePosition =
			facility->GetPosition();

	{
		facility->SetRotation(G4ThreeVector(0., 0., 0.) * deg);
		facility->SetPosition(G4ThreeVector(10., 20., 30.) * m);

		Spallation spallation;
		spallation.SetMode(Spallation::Mode::UniformCircle);
		spallation.GetUniformCircle().GetProps().SetDiameter(0.0);

		G4Event event;
		spallation.GeneratePrimaries(&event);
		auto const v = event.GetPrimaryVertex(0);
		auto const p = v->GetPrimary();

		EXPECT_EQ(0., p->GetMomentumDirection().getX());
		EXPECT_EQ(0., p->GetMomentumDirection().getY());
		EXPECT_EQ(1., p->GetMomentumDirection().getZ());

		EXPECT_EQ(10. * m, v->GetPosition().getX());
		EXPECT_EQ(20. * m, v->GetPosition().getY());
		EXPECT_EQ(30. * m - 250. * mm, v->GetPosition().getZ());
	}

	{
		facility->SetRotation(G4ThreeVector(-45., 0., 0.) * deg);
		facility->SetPosition(G4ThreeVector(10., 20., 30.) * m);

		Spallation spallation;
		spallation.SetMode(Spallation::Mode::UniformCircle);
		spallation.GetUniformCircle().GetProps().SetDiameter(0.0);

		G4Event event;
		spallation.GeneratePrimaries(&event);
		auto const v = event.GetPrimaryVertex(0);
		auto const p = v->GetPrimary();

		EXPECT_EQ(0., p->GetMomentumDirection().getX());
		EXPECT_NEAR(1. / std::sqrt(2.), p->GetMomentumDirection().getY(),
				1.e-6);
		EXPECT_NEAR(1. / std::sqrt(2.), p->GetMomentumDirection().getZ(),
				1.e-6);

		EXPECT_EQ(10. * m, v->GetPosition().getX());
		EXPECT_EQ(20. * m - 250. * mm * sin(45 * deg), v->GetPosition().getY());
		EXPECT_EQ(30. * m - 250. * mm * sin(45 * deg), v->GetPosition().getZ());
	}

	{
		facility->SetRotation(G4ThreeVector(0., -45., 0.) * deg);
		facility->SetPosition(G4ThreeVector(10., 20., 30.) * m);

		Spallation spallation;
		spallation.SetMode(Spallation::Mode::UniformCircle);
		spallation.GetUniformCircle().GetProps().SetDiameter(0.0);

		G4Event event;
		spallation.GeneratePrimaries(&event);
		auto const v = event.GetPrimaryVertex(0);
		auto const p = v->GetPrimary();

		EXPECT_NEAR(-1. / std::sqrt(2.), p->GetMomentumDirection().getX(),
				1.e-6);
		EXPECT_EQ(0., p->GetMomentumDirection().getY());
		EXPECT_NEAR(1. / std::sqrt(2.), p->GetMomentumDirection().getZ(),
				1.e-6);

		EXPECT_EQ(10. * m + 250. * mm * sin(45 * deg), v->GetPosition().getX());
		EXPECT_EQ(20. * m, v->GetPosition().getY());
		EXPECT_EQ(30. * m - 250. * mm * sin(45 * deg), v->GetPosition().getZ());
	}

	facility->SetRotation(saveRotation);
	facility->SetPosition(savePosition);

}

}

}
