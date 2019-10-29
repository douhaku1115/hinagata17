#include "HorizontalPaddle.h"
#include "glut.h"


bool HorizontalPaddle::intersectBall(Ball const& _ball) {
	if (
		(
		(_ball.m_position.y >= m_position.y)
			&& (_ball.m_lastPosition.y < m_position.y))
		||
		(
		(_ball.m_position.y < m_position.y)
			&& (_ball.m_lastPosition.y>= m_position.y)
			)
		)
	{
		if (
			(_ball.m_position.x >= m_position.x)
			&& (_ball.m_position.x < m_position.x + m_width)
			)return true;
	}return false;
}


void HorizontalPaddle::draw() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLfloat range[2];
	
	glGetFloatv(
		GL_LINE_WIDTH_RANGE,//GLenum pname,
		range); //GLfloat *params);
	glLineWidth(range[1]);//GLfloat width);
	glBegin(GL_LINES);
	{
		glVertex2f(m_position.x, m_position.y);
		vec2 v = m_position + vec2( m_width,0);
		glVertex2fv((GLfloat *)&v);
	}
	glEnd();

	glPopAttrib();
}