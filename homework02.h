// 2015 Domrachev Alexandr <Alexandr.Domrachev@gmail.com>
#include "TXLib.h"
#include <unordered_map>

using namespace std;

#define a_size(T) (sizeof(T) / sizeof(*T))

class GenericActor
{
public:
	GenericActor(double x, double y) : m_x(x), m_y(y) {}
	virtual ~GenericActor() {}

	virtual void Draw() = 0;
	virtual void Animate() = 0;

	void Move(double xShift, double yShift)
	{
		m_x += xShift;
		m_y += yShift;
	}

	double X() { return m_x; }
	double Y() { return m_y; }

protected:
	double m_x, m_y;
};


class SunActor : public GenericActor
{
public:
	SunActor(double x, double y, double radius) :
		GenericActor(x, y),
		m_radius(radius),
		m_angle(0), m_rotaryStartAngle(0), m_rotaryEndAngle(0)
	{}

	virtual void Draw();
	virtual void Animate();

private:
	double m_radius;
	double m_angle;
	double m_rotaryStartAngle;
	double m_rotaryEndAngle;

	void recalcAngles()
	{
		m_rotaryStartAngle = m_angle * txPI / 180;
		m_rotaryEndAngle = (m_angle + 360) * txPI / 180;
	}
};

class HouseActor : public GenericActor
{
public:
	HouseActor(double x, double y, double width, double height, bool doorLeft = false) :
		GenericActor(x, y), m_width(width), m_height(height), m_doorLeft(doorLeft)
	{}

	virtual void Draw();
	virtual void Animate() { Draw(); }

private:
	double m_width, m_height;
	bool   m_doorLeft;
};

class HumanActor : public GenericActor
{
public:
	HumanActor(double x, double y, double height) : GenericActor(x, y), m_height(height) {}

	virtual void Draw();
	virtual void Animate() { Draw(); }

private:
	double m_height;
};

class TreeActor : public GenericActor
{
public:
	TreeActor(double x, double y, double width, double height, COLORREF color) :
		GenericActor(x, y), m_width(width), m_height(height), m_color(color)
	{}

	virtual void Draw();
	virtual void Animate() { Draw(); }

private:
	double 	 m_width, m_height;
	COLORREF m_color;
};

class MountainsActor : public GenericActor
{
public:
	MountainsActor(double x = 0, double y = 0) : GenericActor(x, y)
	{}

	virtual void Draw();
	virtual void Animate() { Draw(); }
};

class GenericScene
{
	using actorsMap = unordered_map<string, GenericActor*>;
public:
	GenericScene(double width, double height) : m_sceneWidth(width), m_sceneHeight(height), m_actors()
	{}

	virtual ~GenericScene()
	{
		for (auto &it : m_actors)
		{
			delete it.second;
		}
		m_actors.clear();
	}

	virtual void Draw();

	void AddActor(string name, GenericActor *actor);
	GenericActor* GetActor(string name);

protected:
	double    m_sceneWidth, m_sceneHeight;
	actorsMap m_actors;
};

class Scene : public GenericScene
{
public:
	Scene(double width, double height);

	virtual void Draw();

private:
	void DrawBackground(double sunPosition);

	COLORREF RecalcBackgroundColor(double sunPosition);
};
