// 2015 Domrachev Alexandr <Alexandr.Domrachev@gmail.com>
#include "homework02.h"

const int WIDTH = 1028;
const int HEIGHT = 720;
const int GROUND_LEVEL = HEIGHT - 120;
const int ACTOR_Y_PLACEMENT = GROUND_LEVEL + (HEIGHT - GROUND_LEVEL) / 2;

Scene *scene= NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CleanUP();

int main()
{
	txCreateWindow(WIDTH, HEIGHT);
	txSetWindowsHook(WndProc);

	scene = new Scene(WIDTH, HEIGHT);

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		txBegin();
		if (scene != NULL)
		{
			scene->Draw();
		}
		txEnd();
	}

	return 0;
}

Scene::Scene(double width, double height) :
	GenericScene(width, height),
	m_sun(NULL)
{
	AddActor("sun",				new SunActor(-200, 200, m_sceneWidth / 15));
	AddActor("cloudNearSun",	new CloudActor(m_sceneWidth - 300, 50, 200));
	AddActor("cloud01",			new CloudActor(300, 100, 400));
	AddActor("cloud02",			new CloudActor(100, 40, 200));
	AddActor("mountain",		new MountainsActor());
	AddActor("human",			new HumanActor(900, ACTOR_Y_PLACEMENT - 30, 60));
	AddActor("tree",			new TreeActor(200, ACTOR_Y_PLACEMENT, 100, 300, RGB(0, 100, 0)));
	AddActor("house",			new HouseActor(700, 200, 200, 200, true));
}

void Scene::Draw()
{
	GenericActor *sun = GetSun();
	DrawBackground(sun->X());

	GenericScene::Draw();

	double x = sun->X();
	if (x < m_sceneWidth - 100)
	{
		sun->Move(0.3f, 0);
	}
}


void SunActor::Draw()
{
	txSetColor(TX_YELLOW, 3);
	txSetFillColor(TX_YELLOW);
	txCircle(m_x - m_radius, m_y - m_radius, m_radius);

	double length = m_radius;
	double rays = 16;
	double theta = m_rotaryStartAngle;
	double angleAmount = (m_rotaryEndAngle - m_rotaryStartAngle) / rays;

	for (int i = 0; i < rays; ++i)
	{
		double x1 = m_x - m_radius + cos(theta) * m_radius * 1.2;
		double y1 = m_y - m_radius + sin(theta) * m_radius * 1.2;
		double x2 = x1 + cos(theta) * length;
		double y2 = y1 + sin(theta) * length;
		theta += angleAmount;
		txLine(x1, y1, x2, y2);
	}
}

void SunActor::Animate()
{
	Draw();

	if (m_animated)
	{
		m_angle += 0.05f;
		recalcAngles();
	}
}


void HouseActor::Draw()
{
	txSetColor(TX_DARKGRAY, 1);
	txSetFillColor(TX_DARKGRAY);
	txRectangle(
		m_x - m_width / 2, ACTOR_Y_PLACEMENT - m_height,
		m_x + m_width / 2, ACTOR_Y_PLACEMENT
	);

	// roof
	txSetColor(TX_RED, 1);
	txSetFillColor(TX_RED);
	POINT roof[] = {
		{(LONG)(m_x - m_width / 2), (LONG)(ACTOR_Y_PLACEMENT - m_height)},
		{(LONG)(m_x + m_width / 2), (LONG)(ACTOR_Y_PLACEMENT - m_height)},
		{(LONG)m_x,                 (LONG)(ACTOR_Y_PLACEMENT - m_height - m_height * 25 / 100)}
	};
	txPolygon(roof, a_size(roof));

	// windows
	txSetFillColor(TX_DARKGRAY);
	double xOffset = m_width * 6.666666666 / 100;
	double yOffset = m_height * 4 / 100;
	double x1 = m_x - m_width / 2 + xOffset;
	double y1 = ACTOR_Y_PLACEMENT - m_height + yOffset;
	double windowWidth = m_width * 40 / 100;
	double windowHeight = m_height * 35 / 100;
	for (int ix = 0; ix < 2; ++ix)
	{
		for (int iy = 0; iy < 2; ++iy)
		{
			txSetColor(TX_BLACK, 6);
			txRectangle(
				x1 + (xOffset + windowWidth) * ix, y1 + (yOffset + windowHeight) * iy,
				x1 + (xOffset + windowWidth) * ix + windowWidth, y1 + (yOffset + windowHeight) * iy + windowHeight);
			txSetColor(TX_BLACK, 3);
			txLine(
				x1 + (xOffset + windowWidth) * ix, y1 + (yOffset + windowHeight) * iy + windowHeight / 3,
				x1 + (xOffset + windowWidth) * ix + windowWidth, y1 + (yOffset + windowHeight) * iy + windowHeight / 3);
			txLine(
				x1 + (xOffset + windowWidth) * ix + windowWidth / 2, y1 + (yOffset + windowHeight) * iy,
				x1 + (xOffset + windowWidth) * ix + windowWidth / 2, y1 + (yOffset + windowHeight) * iy + windowHeight);
		}
	}


	// door
	txSetColor(TX_BLACK, 6);
	x1 = m_doorLeft ? m_x - m_width / 2 - 1 : m_x + m_width / 2 + 1;
	txLine(x1, ACTOR_Y_PLACEMENT - m_height * 40 / 100, x1, ACTOR_Y_PLACEMENT - 6);
}

void HumanActor::Draw()
{
	txSetColor(TX_BLACK, 3);
	txSetFillColor(TX_PINK);

	// body
	txLine(m_x, m_y + m_height / 20, m_x, m_y + m_height * 60 / 100);
	txCircle(m_x, m_y + m_height / 10, m_height * 15 / 100);
	// hands
	txLine(m_x, m_y + m_height * 35 / 100, m_x + m_height * 25 / 100, m_leftHand);
	txLine(m_x, m_y + m_height * 35 / 100, m_x - m_height * 25 / 100, m_rightHand);
	// legs
	txLine(m_x, m_y + m_height * 60 / 100, m_x - m_height * 20 / 100, m_y + m_height);
	txLine(m_x, m_y + m_height * 60 / 100, m_x + m_height * 20 / 100, m_y + m_height);
	txSetFillColor(TX_WHITE);
	// eyes
	txCircle(m_x - m_height * 15 / 100 / 2.5, m_y + m_height * 15 / 100 * 20 / 100, m_height * 15 / 100 * 15 / 100);
	txCircle(m_x + m_height * 15 / 100 / 2.5, m_y + m_height * 15 / 100 * 20 / 100, m_height * 15 / 100 * 15 / 100);
	// nose
	txLine(m_x, m_y + m_height * 15 / 100 * 50 / 100, m_x, m_y + m_height * 13 / 100);
	// mouth
	txEllipse(m_x - m_height * 15 / 100 / 2.5, m_y + m_height * 16 / 100, m_x + m_height * 15 / 100 / 2.5, m_y + m_height * 18 / 100);
}

void HumanActor::Animate()
{
	if (m_animated)
	{
		RecalcHandsPosition();
	}
	Draw();
}

void HumanActor::RecalcHandsPosition()
{
	double steps = 100;
	double step = m_height * 30 / 100 / steps;

	m_leftHand  = m_y + step * m_handDelta;
	m_rightHand = m_y + step * m_handDelta;

	if (m_handDelta > steps)
	{
		m_backwardHandMovement = true;
	}
	if (m_handDelta < 0)
	{
		m_backwardHandMovement = false;
	}
	m_handDelta = m_backwardHandMovement ? m_handDelta - 1 : m_handDelta + 1;
}

void TreeActor::Draw()
{
	txSetFillColor(m_color);
	txSetColor(m_color, 1);

	POINT tree[] = {
		{(LONG)(m_x),                 (LONG)(m_y - m_height)},
		{(LONG)(m_x - m_width / 4),   (LONG)(m_y - m_height / 1.2)},
		{(LONG)(m_x - m_width / 6),   (LONG)(m_y - m_height / 1.2)},
		{(LONG)(m_x - m_width / 3),   (LONG)(m_y - m_height / 1.3)},
		{(LONG)(m_x - m_width / 6),   (LONG)(m_y - m_height / 1.3)},
		{(LONG)(m_x - m_width / 2.8), (LONG)(m_y - m_height / 1.5)},
		{(LONG)(m_x - m_width / 5.8), (LONG)(m_y - m_height / 1.47)},
		{(LONG)(m_x - m_width / 2.5), (LONG)(m_y - m_height / 1.9)},
		{(LONG)(m_x - m_width / 5.8), (LONG)(m_y - m_height / 1.9)},
		{(LONG)(m_x - m_width / 1.5), (LONG)(m_y - m_height * 15 / 100)},
		{(LONG)(m_x),                 (LONG)(m_y - m_height * 15 / 100)},
		{(LONG)(m_x),                 (LONG)(m_y - m_height)},
		{(LONG)(m_x + m_width / 4),   (LONG)(m_y - m_height / 1.2)},
		{(LONG)(m_x + m_width / 6),   (LONG)(m_y - m_height / 1.2)},
		{(LONG)(m_x + m_width / 3),   (LONG)(m_y - m_height / 1.3)},
		{(LONG)(m_x + m_width / 6),   (LONG)(m_y - m_height / 1.3)},
		{(LONG)(m_x + m_width / 2.8), (LONG)(m_y - m_height / 1.5)},
		{(LONG)(m_x + m_width / 5.8), (LONG)(m_y - m_height / 1.47)},
		{(LONG)(m_x + m_width / 2.5), (LONG)(m_y - m_height / 1.9)},
		{(LONG)(m_x + m_width / 5.8), (LONG)(m_y - m_height / 1.9)},
		{(LONG)(m_x + m_width / 1.5), (LONG)(m_y - m_height * 15 / 100)},
		{(LONG)(m_x),                 (LONG)(m_y - m_height * 15 / 100)}
	};
	txPolygon(tree, a_size(tree));

	txSetFillColor(RGB(55, 50, 30));
	txSetColor(m_color, 1);
	txRectangle(m_x - m_width / 7, m_y - m_height * 15 / 100, m_x + m_width / 7, m_y);
}

void MountainsActor::Draw()
{
	// background mountains
	txSetFillColor(RGB(30, 30, 30));
	txSetColor(RGB(30, 30, 30));
	POINT bg_mountain[] = {
		{0, GROUND_LEVEL},
		{WIDTH / 5, GROUND_LEVEL / 2},
		{WIDTH / 2, GROUND_LEVEL},
		{WIDTH - WIDTH / 5, GROUND_LEVEL / 2},
		{WIDTH, GROUND_LEVEL}
	};
	txPolygon(bg_mountain, a_size(bg_mountain));

	// foreground mountain
	txSetFillColor(RGB(50, 50, 50));
	txSetColor(RGB(50, 50, 50));
	POINT fg_mountain[] = {
		{(LONG)(WIDTH / 5),         GROUND_LEVEL},
		{(LONG)(WIDTH / 2.7),       (LONG)(GROUND_LEVEL / 1.87)},
		{(LONG)(WIDTH / 2.5),       (LONG)(GROUND_LEVEL / 1.9)},
		{(LONG)(WIDTH / 2),         (LONG)(GROUND_LEVEL / 4)},
		{(LONG)(WIDTH / 1.5),       (LONG)(GROUND_LEVEL / 1.9)},
		{(LONG)(WIDTH - WIDTH / 5), GROUND_LEVEL}
	};
	txPolygon(fg_mountain, a_size(fg_mountain));

	// snowcap
	txSetFillColor(RGB(200, 200, 200));
	txSetColor(RGB(200, 200, 200));
	POINT snowcap[] = {
		{(LONG)(WIDTH / 2.5), (LONG)(GROUND_LEVEL / 1.9)},
		{(LONG)(WIDTH / 2),   (LONG)(GROUND_LEVEL / 4)},
		{(LONG)(WIDTH / 1.5), (LONG)(GROUND_LEVEL / 1.9)}
	};
	txPolygon(snowcap, a_size(snowcap));
}


void CloudActor::Draw()
{
	txSetColor(TX_WHITE, 0);
	txSetFillColor(TX_WHITE);

	double x = m_x;
	double y = m_y;
	double xs = m_size;
	double ys = m_size * 2 / 5;

	txEllipse(x,            y + ys / 3,   x + xs, y + ys);
	txEllipse(x + xs / 5,	y,            x + xs, y + ys);
	txEllipse(x + xs / 5,	y + ys / 1.5, x + xs, y + ys);
	txEllipse(x + xs / 2.5, y + ys / 1.5, x + xs, y + ys);
	txEllipse(x + xs / 1.6, y,            x + xs, y + ys);
	txEllipse(x + xs / 1.6, y + ys,		  x + xs, y + ys);
	txEllipse(x + xs / 1.4, y + ys / 1.2, x + xs, y + ys);
}

void Scene::DrawBackground(double sunPosition)
{
	// sky
	COLORREF backgroundColor = RecalcBackgroundColor(sunPosition);
	txSetColor(backgroundColor, 1);
	txSetFillColor(backgroundColor);
	txRectangle(0, 0, m_sceneWidth, GROUND_LEVEL);

	// grass
	txSetColor(TX_GREEN, 1);
	txSetFillColor(TX_GREEN);
	txRectangle(0, GROUND_LEVEL, m_sceneWidth, m_sceneHeight);
}

COLORREF Scene::RecalcBackgroundColor(double sunPosition)
{
	if ((sunPosition < 0) || (sunPosition > m_sceneWidth))
	{
		return RGB(0, 0, 0);
	}

	unsigned short blue = 0;
	double step = m_sceneWidth / 255;
	blue = (unsigned short)(sunPosition / step);
	if (blue > 255) blue = 255;

	return RGB(0, 0, blue);
}

GenericActor* Scene::GetActor(string name)
{
	if (name == "sun")
	{
		m_sun = m_sun != NULL ? m_sun : GenericScene::GetActor(name);
		return m_sun;
	}

	return GenericScene::GetActor(name);
}


void GenericScene::Draw()
{
	for (auto &it : m_actors)
	{
		it.second->Animate();
	}
}

void GenericScene::AddActor(string name, GenericActor *actor)
{
	m_actors.insert(pair<string, GenericActor*>(name, actor));
}

GenericActor* GenericScene::GetActor(string name)
{
	actorsMap::iterator it = m_actors.find(name);
	assert(it != m_actors.end());

	return it->second;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_DESTROY:
		CleanUP();
		break;
	default:
		break;
	}

	return 0;
}

void CleanUP()
{
	if (scene != NULL)
	{
		delete scene;
		scene = NULL;
	}
}