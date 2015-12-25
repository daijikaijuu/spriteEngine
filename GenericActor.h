#pragma once

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
