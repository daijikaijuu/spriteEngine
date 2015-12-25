// 2015 Domrachev Alexandr <Alexandr.Domrachev@gmail.com>
#include "TXLib.h"
#include <map>

using std::string;

#define a_size(T) (sizeof(T) / sizeof(*T))

class GenericActor
{
public:
	GenericActor(double x, double y, bool animated = true) :
		m_x(x),
		m_y(y),
		m_animated(animated)
	{}
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
	bool   m_animated;
};

class SunActor : public GenericActor
{
public:
	SunActor(double x, double y, double radius) :
		GenericActor(x, y),
		m_radius(radius),
		m_angle(0),
		m_rotaryStartAngle(0),
		m_rotaryEndAngle(0)
	{
		recalcAngles();
	}

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
		GenericActor(x, y),
		m_width(width),
		m_height(height),
		m_doorLeft(doorLeft)
	{}

	virtual void Draw();
	virtual void Animate() { Draw(); }

private:
	double   m_width, m_height;
	bool     m_doorLeft;
};

class HumanActor : public GenericActor
{
public:
	HumanActor(double x, double y, double height, bool animated = true) : 
		GenericActor(x, y, animated),
		m_height(height),
		m_leftHand(0),
		m_rightHand(0),
		m_handDelta(0),
		m_backwardHandMovement(false)
	{
		RecalcHandsPosition();
	}

	virtual void Draw();
	virtual void Animate();

private:
	double m_height;
	double m_leftHand, m_rightHand, m_handDelta;
	bool   m_backwardHandMovement;

	void RecalcHandsPosition();
};

class TreeActor : public GenericActor
{
public:
	TreeActor(double x, double y, double width, double height, COLORREF color) :
		GenericActor(x, y),
		m_width(width),
		m_height(height),
		m_color(color)
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
	MountainsActor(double x = 0, double y = 0) : 
		GenericActor(x, y)
	{}

	virtual void Draw();
	virtual void Animate() { Draw(); }
};

class CloudActor : public GenericActor
{
public:
	CloudActor(double x, double y, double size = 200) :
		GenericActor(x, y),
		m_size(size)
	{}

	virtual void Draw();
	virtual void Animate() { Draw(); }

private:
	double m_size;
};

class GenericScene
{
	using actorsMap = std::map<string, GenericActor*>;
public:
	GenericScene(double width, double height) :
		m_sceneWidth(width),
		m_sceneHeight(height),
		m_actors(),
		m_actorsIndex()
	{}

	virtual ~GenericScene()
	{
		for (auto &it : m_actors)
		{
			delete it.second;
		}
		m_actors.clear();
		m_actorsIndex.clear();
	}

	virtual void Draw();

	void AddActor(string name, GenericActor *actor);
	virtual GenericActor* GetActor(string name);

protected:
	double    m_sceneWidth, m_sceneHeight;
	actorsMap m_actors;
	std::vector<string> m_actorsIndex;
};

class Scene : public GenericScene
{
public:
	Scene(double width, double height);

	virtual void Draw();

	virtual GenericActor* GetActor(string name);
	GenericActor* GetSun() { return m_sun != NULL ? m_sun : GetActor("sun"); }

private:
	GenericActor	*m_sun;

	void DrawBackground(double sunPosition);
	COLORREF RecalcBackgroundColor(double sunPosition);
};
