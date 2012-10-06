#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "rush.h"
#include "resolution.h"

/* Position de la caméra */
int angle = 190;

float posX;
float posY = 25;
float posZ;

int LightPos[4] = {5,12,10,0};

/* ID des différents objets */
int id_cube;
int id_plateau;

/* Resolution */
int resolution = 0;
t_chemin chemin;

double microtime(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec*1e-6;
}


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

void InitGL(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
  	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);

	creer_cube();
	creer_plateau();
}

void positionCamera(){
	posX = 30 * cos(angle * M_PI/180);
	posZ = 30 * sin(angle * M_PI/180);
	printf("Caméra positionné en (%f, %f, %f) [angle=%d]\n", posX, posY, posZ, angle);
}


void scene(){
	int i;
	float x, y;
	int color;
	glCallList(id_plateau); // cube 1

	for (i=0; i<parking_actuel->nb_vehicules; i++) {
		if ( i+1 < 9 ) {
			color = i+1;
		} else {
			color = (i+2) % 8;
		}

		switch ( color ) {
			case 1:
				glColor3ub(255, 0, 0);
			break;

			case 2:
				glColor3ub(0, 255, 0);
			break;

			case 3:
				glColor3ub(255, 255, 0);
			break;

			case 4:
				glColor3ub(0, 0, 255);
			break;

			case 5:
				glColor3ub(255, 0, 255);
			break;

			case 6:
				glColor3ub(0, 255, 255);
			break;

			case 7:
				glColor3ub(200, 200, 200);
			break;

			case 8:
				glColor3ub(50, 50, 50);
			break;
		}

		glPushMatrix();
			y = parking_actuel->position[i].ord*-3 + 6 - (parking_actuel->vehicule[i].axe * 3 * (parking_actuel->vehicule[i].taille - 1));
			x = parking_actuel->position[i].abs*3 - 9;
			glTranslatef(y, 0, x);

			x = parking_actuel->vehicule[i].axe * parking_actuel->vehicule[i].taille * 1.5 + (1 - parking_actuel->vehicule[i].axe) * 1.5;
			y = (1 - parking_actuel->vehicule[i].axe) * parking_actuel->vehicule[i].taille * 1.5 + parking_actuel->vehicule[i].axe * 1.5;
			glScalef(x, 1, y);

			glCallList(id_cube);
		glPopMatrix();

	}
}

void draw() {
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(posX, posY, posZ, 0, 0, 0, 0, 1, 0);

	glLightiv(GL_LIGHT0, GL_POSITION, LightPos);
	scene();

	//glRasterPos2f(-10, 10);
	//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '1');

	glutSwapBuffers();
}


void graphique_resolution() {
	int k = resolution - 1;

	bouger(2*chemin[k].deplacement-1, chemin[k].voiture, *parking_actuel);
	nb_coups++;

	if (chemin[k].deplacement == -1 && chemin[k].voiture == -1) {
		resolution = 0;
	} else {
		resolution++;
	}
}

void graphique_menu(int choix) {
	switch(choix){
		case 0:
			exit(0);
		break;

		case 1:
			// TODO
		break;

		case 2:
			resolution = 1;
			chemin = solution(*parking_actuel);
		break;

		case 3:
			configuration_next();
		break;
	}

	glutPostRedisplay();
}

void idle() {
	static double last_time = 0;


	if(resolution){
		if( microtime() - last_time > 0.5){
			graphique_resolution();
			last_time = microtime();
		}
	}

    glutPostRedisplay(); // is this needed ?
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); // Choisit la matrice de projection
	glLoadIdentity();
	gluPerspective(40, (float) w/h, 1, 1000);
	glMatrixMode(GL_MODELVIEW); // on revient sur la matrice MODELVIEW
	glLoadIdentity();
}

void special(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_LEFT:
			printf("Gauche\n");
			angle = (angle + 5) % 360;
		break;

		case GLUT_KEY_RIGHT:
			printf("Droite\n");
			angle = (angle - 5) % 360;
		break;

		case GLUT_KEY_UP:
			printf("Haut\n");
			posY +=5;
		break;

		case GLUT_KEY_DOWN:
			printf("Bas\n");
			posY -= 5;
		break;
	}

	positionCamera();

	glutPostRedisplay();
}

void graphique_init (int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // | GLUT_STEREO
	glutInitWindowSize(640, 480);
    glutCreateWindow("BiG Hour");

    glutDisplayFunc(draw);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);

	glutCreateMenu(graphique_menu);
	glutAddMenuEntry("Jouer", 1);
	glutAddMenuEntry("Resoudre", 2);
	glutAddMenuEntry("Changer", 3);
	glutAddMenuEntry("Quitter", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

    InitGL();
	positionCamera();

    glutMainLoop();
}

