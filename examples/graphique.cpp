#include "pre.h"

#include <GL/glut.h>

#include <Timer.h>

Timer timer1;
Timer timer2;

/* Position de la caméra */
int angle = 190;

float posX = 0.0f;
float posY = 25.0f;
float posZ = 0.0f;

int LightPos[4] = {5,12,10,0};

/* ID des différents objets */
int id_cube;
int id_plateau;

void creer_plateau() {
	id_plateau = glGenLists(1);
	glNewList(id_plateau, GL_COMPILE);

	glBegin(GL_QUADS);

		// haut
		glNormal3d(0, 1, 0);
		glColor3d(1,1,1); // bleu
		glVertex3d(10,0,10);
		glVertex3d(10,0,-10);
		glVertex3d(-10,0,-10);
		glVertex3d(-10,0,10);

		// bas
		glNormal3d(0,-1, 0);
		glColor3d(1,1,1); // bleu
		glVertex3d(10,-2,10);
		glVertex3d(10,-2,-10);
		glVertex3d(-10,-2,-10);
		glVertex3d(-10,-2,10);

		// arrière
		glNormal3d(0, 0, 1);
		glColor3d(1,0,0); // rouge
		glVertex3d(-10,0,-10);
		glVertex3d(-10,-2,-10);
		glVertex3d(-10,-2,10);
		glVertex3d(-10,0,10);

		// droite
		glNormal3d(-1, 0, 0);
		glColor3d(1,0,0); // rouge
		glVertex3d(10,0,-10);
		glVertex3d(10,-2,-10);
		glVertex3d(-10,-2,-10);
		glVertex3d(-10,0,-10);

		// gauche
		glNormal3d(1, 0, 0);
		glColor3d(1,0,0); // rouge
		glVertex3d(-10,0,10);
		glVertex3d(-10,-2,10);
		glVertex3d(10,-2,10);
		glVertex3d(10,0,10);

		// avant
		glNormal3d(0, 0, -1);
		glColor3d(1,0,0); // rouge
		glVertex3d(10,0,10);
		glVertex3d(10,-2,10);
		glVertex3d(10,-2,-10);
		glVertex3d(10,0,-10);
	glEnd();

	glEndList();
}

void creer_cube(){
	id_cube = glGenLists(1);
	glNewList (id_cube,GL_COMPILE);

	glBegin(GL_QUADS);
		// gauche
		glNormal3d(1, 0, 0);
		glVertex3i(2,2,2);
		glVertex3i(2,0,2);
		glVertex3i(0,0,2);
		glVertex3i(0,2,2);

		// droite
		glNormal3d(-1, 0, 0);
		glVertex3i(2,2,0);
		glVertex3i(2,0,0);
		glVertex3i(0,0,0);
		glVertex3i(0,2,0);

		//avant
		glNormal3d(0, 0, -1);
		glVertex3i(2,2,2);
		glVertex3i(2,0,2);
		glVertex3i(2,0,0);
		glVertex3i(2,2,0);

		// arrière
		glNormal3d(0, 0, 1);
		glVertex3i(0,2,2);
		glVertex3i(0,0,2);
		glVertex3i(0,0,0);
		glVertex3i(0,2,0);

		// haut
		glNormal3d(0, 1, 0);
		glVertex3i(0,2,0);
		glVertex3i(0,2,2);
		glVertex3i(2,2,2);
		glVertex3i(2,2,0);

		// bas
		glNormal3d(0, -1, 0);
		glVertex3i(0,0,0);
		glVertex3i(0,0,2);
		glVertex3i(2,0,2);
		glVertex3i(2,0,0);
	glEnd();

	glEndList();
}

void positionCamera(void)
{
	posX = 30 * cos(angle * M_PI/180);
	posZ = 30 * sin(angle * M_PI/180);
	//std::cout << "Caméra positionné en (" << posX << ", " << posY << ", " << posZ << ") [angle="<< angle <<"]" << std::endl;
}

void draw(void)
{
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(posX, posY, posZ, 0, 0, 0, 0, 1, 0);

	glLightiv(GL_LIGHT0, GL_POSITION, LightPos);

	glCallList(id_plateau);

	timer1.Stop();

	SimpleApplication::OnPreUpdate(timer1.getElapsedTime());

	timer1.Start();

	glutSwapBuffers();
}

void idle(void)
{
	timer2.Stop();

	SimpleApplication::OnPostUpdate(timer2.getElapsedTime());

	timer2.Start();

    glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); // Choisit la matrice de projection
	glLoadIdentity();
	gluPerspective(40, (float) w/h, 1, 1000);
	glMatrixMode(GL_MODELVIEW); // on revient sur la matrice MODELVIEW
	glLoadIdentity();
}

void special(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_F1:
			//resolution = 1;
			//chemin = parking_actuel->solution();
		break;

		case GLUT_KEY_F2:
			//resolution = 0;
			//load_next_level();
		break;

		case GLUT_KEY_LEFT:
			angle = (angle + 5) % 360;
		break;

		case GLUT_KEY_RIGHT:
			angle = (angle - 5) % 360;
		break;

		case GLUT_KEY_UP:
			posY +=5;
		break;

		case GLUT_KEY_DOWN:
			posY -= 5;
		break;
	}

	positionCamera();

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (GLUT_LEFT_BUTTON == button)
	{
		if (GLUT_DOWN == state)
		{
			SimpleApplication::OnTouchUp(0, x, y);
		}
		else
		{
			SimpleApplication::OnTouchDown(0, x, y);
		}
	}
}

void motion(int x, int y)
{
	SimpleApplication::OnTouchMove(0, x, y);
}

int main(int argc, char ** argv)
{
	SimpleApplication::OnPreInitialize();

	timer1.Start();
	timer2.Start();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // | GLUT_STEREO
	glutInitWindowSize(1024, 768);
    glutCreateWindow("BiG Hour");

    glutDisplayFunc(draw);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);

	// Init GL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
  	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);

	creer_cube();
	creer_plateau();

	// Init Camera
	positionCamera();

	SimpleApplication::OnPostInitialize();

    glutMainLoop();

    return 0;
}

