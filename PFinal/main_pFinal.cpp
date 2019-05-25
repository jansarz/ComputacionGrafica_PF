//Semestre 2017 - 2
//************************************************************//
//************************************************************//
//************** Alumno (s): Sánchez Santiago José Manuel****//
//*************				Salazar Ramirez Jonathan	******//
//*************											******//
//************************************************************//
//************************************************************//

#include "texture.h"
#include "figuras.h"
#include "Camera.h"
#include "cmodel/CModel.h"
#include <MMSystem.h>
#include <Windows.h>
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
static GLuint ciudad_display_list;	//Display List for the Monito

//NEW// Keyframes
//Variables de dibujo y manipulacion
float posX =0, posY = 2.5, posZ =-3.5, rotRodIzq = 0, rotRodDer = 0, rotBraIzq = 0, rotBraDer = 0;
float giroMonito = 0;
int seg = 0, min = 0,foto=1;
float hor = 0,glob=0;
bool globoT = false;
#define MAX_FRAMES 5
int i_max_steps = 90;
int i_curr_steps = 0;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotRodDer;
	float rotBraIzq;
	float rotBraDer;
	float rotInc;	
	float rotInc2;
	float rotInc3;
	float rotInc4;
	float giroMonito;
	float giroMonitoInc;
	
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex=0;			//introducir datos
bool play=false;
int playIndex=0;

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

bool clock = false,pantalla=false;

char ruta[23]= "texturas/video/0.bmp";
int w = 500, h = 500;
int frame=0,timebase=0;
char s[30],texto[10];

CCamera objCamera;	//Create objet Camera

GLfloat g_lookupdown = 0.0f;    // Look Position In The Z-Axis (NEW) 

int font=(int)GLUT_BITMAP_HELVETICA_18;


//GLfloat Diffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				// Diffuse Light Values
GLfloat Diffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };				// Diffuse Light Values
GLfloat Specular[] = { 1.0, 1.0, 1.0, 1.0 };				// Specular Light Values
GLfloat Position[]= { 0.0f, 7.0f, -5.0f, 0.0f };			// Light Position
GLfloat Position2[]= { 0.0f, 0.0f, -5.0f, 1.0f };			// Light Position

GLfloat m_dif1[] = { 0.0f, 0.2f, 1.0f, 1.0f };				// Diffuse Light Values
GLfloat m_spec1[] = { 0.0, 0.0, 0.0, 1.0 };				// Specular Light Values
GLfloat m_amb1[] = { 0.0, 0.0, 0.0, 1.0 };				// Ambiental Light Values
GLfloat m_s1[] = {18};

GLfloat m_dif2[] = { 0.8f, 0.2f, 0.0f, 1.0f };				// Diffuse Light Values
GLfloat m_spec2[] = { 0.0, 0.0, 0.0, 1.0 };				// Specular Light Values
GLfloat m_amb2[] = { 0.0, 0.0, 0.0, 1.0 };				// Ambiental Light Values
GLfloat m_s2[] = {22};

CTexture text1;
CTexture text2;
CTexture text3;	//Flecha
CTexture text4;	//Pavimento
CTexture text5;	//Pasto01
CTexture text6;	//Casa01


///////////TEXTURAS NUEVAS///////////////////////////

CTexture textVerde;
CTexture textAlmohada;
CTexture textCajonera;
CTexture textEdredon;
CTexture textEdredon2;
CTexture textLibros;
CTexture textLuz;
CTexture textMaderaBlanca;
CTexture textMaderaChocolate;
CTexture textMaderaEstante;
CTexture textMetal;
CTexture textNegro;
CTexture textNegroDesvanecido;
CTexture textPantalla[2];
CTexture textVentana;
CTexture textCasa;
CTexture textFachada;
CTexture textTabique;
CTexture textPiso;
CTexture textTecho;
CTexture textMesa;
CTexture textReloj;
CTexture textMapa;
////HAB2//
CTexture textSofa;
CTexture textCojin;
CTexture textLampara;
CTexture textChocolate;
CTexture textPared;
CTexture textPiso2;
CTexture textTapete;
CTexture textTapiz;


float abrirPuerta = 0;
//END NEW//////////////////////////////////////////

CFiguras fig1;
CFiguras fig2;
CFiguras fig3;
CFiguras fig4;	//Pasto01
CFiguras fig5;	//Casa01
CFiguras fig6;
CFiguras fig7;	//Para crear Monito


void saveFrame ( void )
{
	
	printf("frameindex %d\n",FrameIndex);			

	KeyFrame[FrameIndex].posX=posX;
	KeyFrame[FrameIndex].posY=posY;
	KeyFrame[FrameIndex].posZ=posZ;

	KeyFrame[FrameIndex].rotRodIzq= rotRodIzq;
	KeyFrame[FrameIndex].rotRodDer = rotRodDer;
	KeyFrame[FrameIndex].rotBraIzq = rotBraIzq;
	KeyFrame[FrameIndex].rotBraDer = rotBraDer;

	KeyFrame[FrameIndex].giroMonito= giroMonito;
			
	FrameIndex++;
}

void resetElements( void )
{
	posX=KeyFrame[0].posX;
	posY=KeyFrame[0].posY;
	posZ=KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	rotBraIzq = KeyFrame[0].rotBraIzq;
	rotBraDer = KeyFrame[0].rotBraDer;

	rotRodDer = KeyFrame[0].rotRodDer;
	giroMonito=KeyFrame[0].giroMonito;


}

void interpolation ( void )
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;	
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;	
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;	

	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;	
	KeyFrame[playIndex].rotInc3 = (KeyFrame[playIndex + 1].rotBraIzq - KeyFrame[playIndex].rotBraIzq) / i_max_steps;
	KeyFrame[playIndex].rotInc4 = (KeyFrame[playIndex + 1].rotBraDer - KeyFrame[playIndex].rotBraDer) / i_max_steps;

	KeyFrame[playIndex].rotInc2 = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;
	KeyFrame[playIndex].giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}


void ciudad ()
{

		glPushMatrix(); //Camino1
			glTranslatef(23.5,0.0,0.0);
			glScalef(40,0.1,7);
			glDisable(GL_LIGHTING);
			fig3.prisma2(1.0,1.0,1.0,text4.GLindex, 0);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Camino2
			glTranslatef(-23.5,0.0,0.0);
			glScalef(40,0.1,7);
			glDisable(GL_LIGHTING);
			fig3.prisma2(1.0,1.0,1.0,text4.GLindex, 0);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Pasto
			glTranslatef(0.0,0.0,-4.0);
			glScalef(87,0.1,1);
			glDisable(GL_LIGHTING);
			fig4.prisma2(1.0,1.0,1.0,text5.GLindex, 0);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Pasto
			glTranslatef(0.0,0.0,4.0);
			glScalef(87,0.1,1);
			glDisable(GL_LIGHTING);
			fig4.prisma2(1.0,1.0,1.0,text5.GLindex, 0);
			glEnable(GL_LIGHTING);
		glPopMatrix();

}

void monito()
{
	//glNewList(1, GL_COMPILE);
	glPushMatrix();//Pecho
		glScalef(0.5, 0.5, 0.5);
		fig7.prisma(2.0 ,2.0 ,1 ,text2.GLindex);

		glPushMatrix();//Cuello
			glTranslatef(0, 1.0, 0.0);
			fig7.cilindro(0.25, 0.25, 15, 0);
			glPushMatrix();//Cabeza
				glTranslatef(0, 1.0, 0);
				fig7.esfera(0.75, 15, 15, 0);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix(); //Brazo derecho-->
			glTranslatef(1.25, 0.65, 0);
			fig7.esfera(0.5, 12, 12, 0);
			glPushMatrix();
				glTranslatef(0.25, 0, 0);
				glRotatef(-45 + rotBraDer, 0, 1, 0);
				glTranslatef(0.75, 0, 0);
				fig7.prisma(0.7, 1.5, 0.7, 0);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix(); //Brazo izquierdo <--
			glTranslatef(-1.25, 0.65, 0);
			fig7.esfera(0.5, 12, 12, 0);
			glPushMatrix();
				glTranslatef(-0.25, 0, 0);
				glRotatef(45 + rotBraIzq, 0, 1, 0);
				glRotatef(25, 0, 0, 1);
				glRotatef(25, 1, 0, 0);
				glTranslatef(-0.75, 0, 0);
				fig7.prisma(0.7, 1.5, 0.7, 0);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();//Cintura
			glColor3f(0, 0, 1);
			glTranslatef(0, -1.5, 0);
			fig7.prisma(1, 2, 1, 0);

			glPushMatrix(); //Pie Derecho -->
				glTranslatef(0.75, -0.5, 0);
				glRotatef(-15, 1, 0, 0);
				glTranslatef(0, -0.5, 0);
				fig7.prisma(1.0, 0.5, 1, 0);

				glPushMatrix();
					glTranslatef(0, -0.5, 0);
					glRotatef(15+rotRodDer, 1, 0, 0);
					glTranslatef(0, -0.75, 0);
					fig7.prisma(1.5, 0.5, 1, 0);

					glPushMatrix();
						glTranslatef(0, -0.75, 0.3);
						fig7.prisma(0.2, 1.2, 1.5, 0);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();


			glPushMatrix(); //Pie Izquierdo -->
				glTranslatef(-0.75, -0.5, 0);
				glRotatef(-5, 1, 0, 0);
				glTranslatef(0, -0.5, 0);
				fig7.prisma(1.0, 0.5, 1, 0);

				glPushMatrix();
					glTranslatef(0, -0.5, 0);
					glRotatef(15+rotRodIzq, 1, 0, 0);
					glTranslatef(0, -0.75, 0);
					fig7.prisma(1.5, 0.5, 1, 0);

					glPushMatrix();
						glTranslatef(0, -0.75, 0.3);
						fig7.prisma(0.2, 1.2, 1.5, 0);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();


		glPopMatrix();


		glColor3f(1, 1, 1);
	glPopMatrix();
	//glEndList();
}



//////////NUEVA CASA//////////////////////////////////////


///////////FIN  CASA////////////////////////////////////





GLuint createDL() 
{
	GLuint ciudadDL;
	//GLuint cieloDL;

	// Create the id for the list
	ciudadDL = glGenLists(1);
	// start list
	glNewList(ciudadDL,GL_COMPILE);
	// call the function that contains 
	// the rendering commands
		ciudad();
		//monito();
	// endList
	glEndList();

	return(ciudadDL);
}
			
void InitGL ( GLvoid )     // Inicializamos parametros
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Negro de fondo	

	glEnable(GL_TEXTURE_2D);

	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT1, GL_POSITION, Position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glClearDepth(1.0f);									// Configuramos Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Habilitamos Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a realizar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

    
    text1.LoadBMP("01.bmp");
	text1.BuildGLTexture();
	text1.ReleaseImage();

	text2.LoadBMP("logopumas.bmp");
	text2.BuildGLTexture();
	text2.ReleaseImage();

	text3.LoadTGA("city/arrow.tga");
	text3.BuildGLTexture();
	text3.ReleaseImage();

	text4.LoadTGA("city/pavimento.tga");
	text4.BuildGLTexture();
	text4.ReleaseImage();

	text5.LoadTGA("city/pasto01.tga");
	text5.BuildGLTexture();
	text5.ReleaseImage();

	text6.LoadTGA("city/casa01.tga");
	text6.BuildGLTexture();
	text6.ReleaseImage();


	///////NUEVAS TEXTURAS////////////////////////////////////////////
	textVerde.LoadBMP("texturas/alfombra_verde.bmp");
	textVerde.BuildGLTexture();
	textVerde.ReleaseImage();

	textAlmohada.LoadBMP("texturas/almohada.bmp");
	textAlmohada.BuildGLTexture();
	textAlmohada.ReleaseImage();

	textCajonera.LoadBMP("texturas/cajonera.bmp");
	textCajonera.BuildGLTexture();
	textCajonera.ReleaseImage();

	textEdredon.LoadBMP("texturas/Edredon.bmp");
	textEdredon.BuildGLTexture();
	textEdredon.ReleaseImage();

	textEdredon2.LoadTGA("texturas/edredon2.tga");
	textEdredon2.BuildGLTexture();
	textEdredon2.ReleaseImage();
	
	textLibros.LoadTGA("texturas/libros.tga");
	textLibros.BuildGLTexture();
	textLibros.ReleaseImage();

	textLuz.LoadBMP("texturas/luz.bmp");
	textLuz.BuildGLTexture();
	textLuz.ReleaseImage();

	textMaderaBlanca.LoadBMP("texturas/madera_blanca.bmp");
	textMaderaBlanca.BuildGLTexture();
	textMaderaBlanca.ReleaseImage();

	textMaderaChocolate.LoadTGA("texturas/madera_chocolate.tga");
	textMaderaChocolate.BuildGLTexture();
	textMaderaChocolate.ReleaseImage();

	textMaderaEstante.LoadBMP("texturas/madera_estante.bmp");
	textMaderaEstante.BuildGLTexture();
	textMaderaEstante.ReleaseImage();

	textMetal.LoadBMP("texturas/metal.bmp");
	textMetal.BuildGLTexture();
	textMetal.ReleaseImage();

	textNegro.LoadBMP("texturas/negro.bmp");
	textNegro.BuildGLTexture();
	textNegro.ReleaseImage();

	textNegroDesvanecido.LoadBMP("texturas/negrodesvanecido.bmp");
	textNegroDesvanecido.BuildGLTexture();
	textNegroDesvanecido.ReleaseImage();

	for (int i= 0; i <2; i++) {		
		sprintf(texto, "%d", i);
		strcpy(ruta, "texturas/video/");
		strcat(ruta, texto);
		strcat(ruta, ".bmp");		

		textPantalla[i].LoadBMP(ruta);
		textPantalla[i].BuildGLTexture();
		textPantalla[i].ReleaseImage();
	}
	
	
	textVentana.LoadTGA("texturas/ventana.tga");
	textVentana.BuildGLTexture();
	textVentana.ReleaseImage();

	textCasa.LoadBMP("texturas/fondo_madera_blanca.bmp");
	textCasa.BuildGLTexture();
	textCasa.ReleaseImage();

	textTabique.LoadBMP("texturas/tabique.bmp");
	textTabique.BuildGLTexture();
	textTabique.ReleaseImage();

	textFachada.LoadTGA("texturas/fachada.tga");
	textFachada.BuildGLTexture();
	textFachada.ReleaseImage();

	textPiso.LoadBMP("texturas/piso.bmp");
	textPiso.BuildGLTexture();
	textPiso.ReleaseImage();

	textTecho.LoadBMP("texturas/techo.bmp");
	textTecho.BuildGLTexture();
	textTecho.ReleaseImage();

	textMesa.LoadBMP("texturas/mesa.bmp");
	textMesa.BuildGLTexture();
	textMesa.ReleaseImage();

	textReloj.LoadTGA("texturas/reloj.tga");
	textReloj.BuildGLTexture();
	textReloj.ReleaseImage();

	textMapa.LoadBMP("texturas/mapa.bmp");
	textMapa.BuildGLTexture();
	textMapa.ReleaseImage();
	
	//HAB 2/
	textSofa.LoadBMP("texturas/hab/sofa.bmp");
	textSofa.BuildGLTexture();
	textSofa.ReleaseImage();

	textCojin.LoadBMP("texturas/hab/cojin.bmp");
	textCojin.BuildGLTexture();
	textCojin.ReleaseImage();

	textLampara.LoadBMP("texturas/hab/lampara.bmp");
	textLampara.BuildGLTexture();
	textLampara.ReleaseImage();

	textChocolate.LoadBMP("texturas/hab/madera_chocolate.bmp");
	textChocolate.BuildGLTexture();
	textChocolate.ReleaseImage();
	
	textPared.LoadBMP("texturas/hab/pared.bmp");
	textPared.BuildGLTexture();
	textPared.ReleaseImage();

	textPiso2.LoadBMP("texturas/hab/piso.bmp");
	textPiso2.BuildGLTexture();
	textPiso2.ReleaseImage();

	textTapete.LoadBMP("texturas/hab/tapete.bmp");
	textTapete.BuildGLTexture();
	textTapete.ReleaseImage();

	textTapiz.LoadBMP("texturas/hab/tapiz.bmp");
	textTapiz.BuildGLTexture();
	textTapiz.ReleaseImage();


	//////FIN NUEVAS TEXTURAS//////////////////////////////

	objCamera.Position_Camera(0,2.5f,3, 0,2.5f,0, 0, 1, 0);

	//NEW Crear una lista de dibujo
	ciudad_display_list = createDL();

	//NEW Iniciar variables de KeyFrames
	for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX =0;
		KeyFrame[i].posY =0;
		KeyFrame[i].posZ =0;
		KeyFrame[i].incX =0;
		KeyFrame[i].incY =0;
		KeyFrame[i].incZ =0;
		KeyFrame[i].rotRodIzq =0;
		KeyFrame[i].rotRodDer = 0;
		KeyFrame[i].rotBraDer = 0;
		KeyFrame[i].rotBraIzq = 0;
		KeyFrame[i].rotInc =0;
		KeyFrame[i].rotInc2= 0;
		KeyFrame[i].rotInc3 = 0;
		KeyFrame[i].rotInc4 = 0;
		KeyFrame[i].giroMonito =0;
		KeyFrame[i].giroMonitoInc =0;
	}
	//NEW//////////////////NEW//////////////////NEW///////////

}

void pintaTexto(float x, float y, float z, void *font,char *string)
{
  
  char *c;     //Almacena los caracteres a escribir
  glRasterPos3f(x, y, z);	//Posicion apartir del centro de la ventana
  //glWindowPos2i(150,100);
  for (c=string; *c != '\0'; c++) //Condicion de fin de cadena
  {
    glutBitmapCharacter(font, *c); //imprime
  }
}

//////Muebles Casa/////
void casa() {
	glPushMatrix();
		glTranslatef(-8.0, 5.0, -12.0);
		fig1.prisma3(10, 36, 15, textFachada.GLindex, textTabique.GLindex, textPiso.GLindex, textTecho.GLindex);
	glPopMatrix();
}

void cama() {	
	glPushMatrix();
		glTranslatef(4.0, 1.25, -9.0);
		//colchon
		glPushMatrix();			
			glRotatef(-90, 0.0, 1.0, 0.0);
			glRotatef(-90, 1.0, 0.0, 0.0);
			fig1.prisma2(5, 8, 0.5, textEdredon2.GLindex, textEdredon.GLindex);
		glPopMatrix();
		//almohadas
		glPushMatrix();	
			glTranslatef(1.3, 0.5, 3.0);
			fig1.prisma2(0.4, 2.2, 1.5, textAlmohada.GLindex, textAlmohada.GLindex);
			glTranslatef(-2.6, 0.0, 0.0);
			fig1.prisma2(0.4, 2.2, 1.5, textAlmohada.GLindex, textAlmohada.GLindex);
		glPopMatrix();
		//base
		glPushMatrix();
			glTranslatef(0.0, -0.5, 0.0);
			glRotatef(-90, 0.0, 1.0, 0.0);
			glRotatef(-90, 1.0, 0.0, 0.0);		
			fig1.prisma(5, 8, 0.5, textMaderaChocolate.GLindex);
		glPopMatrix();
		//patas
		glPushMatrix();
			glTranslatef(-2.3, -1.0, -3.5);
			fig1.prisma(0.5, 0.2, 0.2,textMaderaChocolate.GLindex);//DelanteraIzq
			glTranslatef(4.3, 0.0, 0.0);
			fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//DelanteraDer
			glTranslatef(0.0, 0.0, 7.0);
			fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//traseraDer
			glTranslatef(-4.3, 0.0, 0.0);
			fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//traseraIzq
		glPopMatrix();
	glPopMatrix();
}

void estante() {
	glPushMatrix();
		glTranslatef(7.5, 1.5, -17.5);
		fig1.prisma2(3, 2, 2, textMaderaEstante.GLindex, textCajonera.GLindex);
	glPopMatrix();
}

void television() {	
	glPushMatrix();
		glTranslatef(4.0, 6.0, -19.4);
		if (pantalla) {				
			glScalef(3, 4, 0.1);
			glRotatef(90, 1.0, 0.0, 0.0);
			fig1.prisma_anun(textPantalla[1].GLindex,textNegro.GLindex);
		}else {
			fig1.prisma2(4, 3, 0.1, textNegro.GLindex, textPantalla[0].GLindex);
		}
		glPopMatrix();
}

void mesa() {
	//Base
	glPushMatrix();
		glTranslatef(-5.0,2.0,-18.4);
		fig1.prisma2(0.3,5.0,2.0,textMesa.GLindex, textMesa.GLindex);
	glPopMatrix();
	//Patas
	glPushMatrix();
		glTranslatef(-7.6, 1.1, -18.4);
		fig1.prisma2(2.12,0.3,2.0, textMesa.GLindex, textMesa.GLindex);
		glTranslatef(5.0, 0.0, 0.0);
		fig1.prisma2(2.12, 0.3, 2.0, textMesa.GLindex, textMesa.GLindex);
	glPopMatrix();

}

void reloj() {
	glPushMatrix();
		//Base
		glTranslatef(-1.5, 7.0, -19.4);
		glPushMatrix();			
			fig1.plano(2,2, textReloj.GLindex);
		glPopMatrix();
		//Manecillas//
		//Segundos
		glPushMatrix();			
			glTranslatef(1.0, 1.0, 0.1);
			glRotatef(seg*-6, 0.0, 0.0, 1.0);
			fig1.plano(0.7, 0.01, textNegro.GLindex);
		glPopMatrix();
		//minutos
		glPushMatrix();			
			glTranslatef(1.0, 1.0, 0.1);
			glRotatef(min*-6, 0.0, 0.0, 1.0);
			fig1.plano(0.7, 0.03, textNegro.GLindex);
		glPopMatrix();
		//horas
		glPushMatrix();			
			glTranslatef(1.0, 1.0, 0.1);
			glRotatef(hor, 0.0, 0.0, 1.0);
			fig1.plano(0.5, 0.03, textNegro.GLindex);
		glPopMatrix();
	glPopMatrix();
}

void librero() {
	glPushMatrix();
	glScalef(0.4,1.0,0.3);
		//Base Superior
		glPushMatrix();
			glTranslatef(-5.0, 2.0, -18.5);
			fig1.prisma2(0.3, 5.0, 2.0, textMesa.GLindex, textMesa.GLindex);
		glPopMatrix();
		//Laterales
		glPushMatrix();
			glTranslatef(-7.6, 1.1, -18.5);
			fig1.prisma2(2.12, 0.3, 2.0, textMesa.GLindex, textMesa.GLindex);
			glTranslatef(5.0, 0.0, 0.0);
			fig1.prisma2(2.12, 0.3, 2.0, textMesa.GLindex, textMesa.GLindex);
		glPopMatrix();
		//Base Inferior
		glPushMatrix();
			glTranslatef(-5.0, 0.2, -18.5);
			fig1.prisma2(0.3, 5.0, 2.0, textMesa.GLindex, textMesa.GLindex);
		glPopMatrix();
		//Frente
		glPushMatrix();
			glTranslatef(-7.45, 0.35, -17.5);
			fig1.plano(2.18, 4.7,textLibros.GLindex);
		glPopMatrix();
	glPopMatrix();
}

void lampara() {
	glPushMatrix();
		glTranslatef(0.0,9.0,-14.0);
		fig1.cilindro(0.03,1.0,20, textNegroDesvanecido.GLindex);
		glTranslatef(0.0, -0.6
, 0.0);
		fig1.esfera2(0.6, 20, 20, textNegroDesvanecido.GLindex);
		fig1.esfera2(0.5, 20, 20, textLuz.GLindex);
	glPopMatrix();
}

void silla() {
	glPushMatrix();
		glTranslatef(-5.0, 0.2,-16.0);
		//Base
		glPushMatrix();
			fig1.esfera(0.07,20,20,textNegro.GLindex);
			glTranslatef(0.6, 0.0, 0.0);
			fig1.esfera(0.07, 20, 20, textNegro.GLindex);
			glTranslatef(-0.3,0.0,-0.3);
			fig1.esfera(0.07, 20, 20, textNegro.GLindex);
			glTranslatef(0.0, 0.0, 0.6);
			fig1.esfera(0.07, 20, 20, textNegro.GLindex);
			glTranslatef(0.0, 0.6, -0.3);
			fig1.cilindro(0.03, 0.6, 20, textMetal.GLindex);
			glTranslatef(0.0, -0.3, 0.0);//CENTRO SILLA
			fig1.cilindro(0.07, 0.3, 20, textMetal.GLindex);
			glTranslatef(0.0, -0.1, 0.0);
			glPushMatrix();
				glRotatef(90,0.0,1.0,0.0);
				fig1.prisma(0.25, 0.7, 0.1, textMetal.GLindex);
			glPopMatrix();
			fig1.prisma(0.25,0.7,0.1,textMetal.GLindex);
		glPopMatrix();
		//Asiento
		glTranslatef(0.34, 1.2, -0.05);
		glPushMatrix();
			fig1.prisma2(0.1,0.5,0.5,textVerde.GLindex,textNegro.GLindex);
			glTranslatef(0.0, 1.0, 0.2);
			fig1.prisma2(1.0, 0.5, 0.05, textVerde.GLindex, textNegro.GLindex);
			glTranslatef(0.0, -0.6, 0.05);
			fig1.prisma(0.7, 0.1, 0.01, textNegro.GLindex);
		glPopMatrix();

	glPopMatrix();

}

void globo() {
	glPushMatrix();
		glTranslatef(-6.5, 2.45, -18.0);
		glPushMatrix();	
			glRotatef(glob,0.0,1.0,0.0);
			fig1.esfera(0.2, 20.0, 20.0, textMapa.GLindex);
		glPopMatrix();
		//Base
		glPushMatrix();			
			glColor3f(1.0,1.0,0.0);
			glRotatef(90,1.0,0.0,0.0);
			fig1.torus(0.23, 0.2, 20, 20);
			glColor3f(1.0, 1.0, 1.0);			
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -0.3, 0.0);
			fig1.esfera2(0.1, 20.0, 20.0, textNegro.GLindex);
		glPopMatrix();

	

	glPopMatrix();
}

///HABITACION 2//////////
void hab2() {
	glPushMatrix();
	glTranslatef(-17.0, 5.0, -12.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	fig1.prisma3(9.9, 14.9, 17.9, textFachada.GLindex, textPared.GLindex, textPiso2.GLindex, textTecho.GLindex);
	glPopMatrix();

}

void sofa() {
	glPushMatrix();	
	glScalef(0.5,1.0,1.0);
	glTranslatef(-18.0, 1.25, -9.0);
	glScalef(0.5, 1.0, 0.5);
	//Tapiz
	glPushMatrix();
	glTranslatef(2.2,1.0,0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	fig1.prisma2(2.5, 8, 0.5, textSofa.GLindex, textTapiz.GLindex);
	glPopMatrix();
	//Tapiz2
	glPushMatrix();
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	fig1.prisma2(5, 8, 0.5, textSofa.GLindex, textSofa.GLindex);
	glPopMatrix();
	//Cojines
	glPushMatrix();
	glTranslatef(1.3, 1.0, -1.7);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	fig1.prisma2(0.4, 2.2, 1.5, textCojin.GLindex, textCojin.GLindex);
	glTranslatef(-3.6, 0.0, 0.0);
	fig1.prisma2(0.4, 2.2, 1.5, textCojin.GLindex, textCojin.GLindex);
	glPopMatrix();
	//base
	glPushMatrix();
	glTranslatef(0.0, -0.5, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	fig1.prisma(5, 8, 0.5, textMaderaChocolate.GLindex);
	glPopMatrix();
	//patas
	glPushMatrix();
	glTranslatef(-2.3, -1.0, -3.5);
	fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//DelanteraIzq
	glTranslatef(4.3, 0.0, 0.0);
	fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//DelanteraDer
	glTranslatef(0.0, 0.0, 7.0);
	fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//traseraDer
	glTranslatef(-4.3, 0.0, 0.0);
	fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//traseraIzq
	glPopMatrix();
	glPopMatrix();
}

void tapiz() {
	glPushMatrix();
		glTranslatef(-23.0,0.2,-16.0);
		glRotatef(90,1.0,0.0,0.0);
		fig3.plano(4.0, 5.0, textTapete.GLindex);
	glPopMatrix();
}

void  mesa2() {
	glPushMatrix();
		glTranslatef(-18.0,1.3,-18.0);
		glScalef(0.1,0.3,0.05);
		//base
		glPushMatrix();
		glTranslatef(0.0, -0.5, 0.0);
		glRotatef(-90, 0.0, 1.0, 0.0);
		glRotatef(-90, 1.0, 0.0, 0.0);
		fig1.prisma(5, 8, 0.5, textMesa.GLindex);
		glPopMatrix();
		//patas
		glPushMatrix();
		glTranslatef(-2.3, -2.0, -3.5);
		glScalef(1.0, 7.0, 1.0);
		fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//DelanteraIzq
		glTranslatef(4.3, 0.0, 0.0);
		fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//DelanteraDer
		glTranslatef(0.0, 0.0, 7.0);
		fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//traseraDer
		glTranslatef(-4.3, 0.0, 0.0);
		fig1.prisma(0.5, 0.2, 0.2, textMaderaChocolate.GLindex);//traseraIzq
		glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void display ( void )   // Creamos la funcion donde se dibuja
{
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glPushMatrix();
	

	glRotatef(g_lookupdown,1.0f,0,0);
		gluLookAt(	objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,	
					objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,	
					objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);
	
		glPushMatrix();		
			glPushMatrix(); //Creamos cielo
				glDisable(GL_LIGHTING);
				glTranslatef(0,60,0);
				fig1.skybox(130.0, 130.0, 130.0,text1.GLindex);
				glEnable(GL_LIGHTING);
			glPopMatrix();

			glPushMatrix();
				glEnable ( GL_COLOR_MATERIAL );
					glColor3f(1, 1, 1);
					//glScalef(0.5, 0.5, 0.5);
					//monito();
					glCallList(ciudad_display_list);
					glTranslatef(posX, posY, posZ);
					glRotatef(giroMonito, 0, 1, 0);
					//monito();
				glDisable ( GL_COLOR_MATERIAL );
			glPopMatrix();			

			glPushMatrix(); //Flecha
				glScalef(7,0.1,7);
				glDisable(GL_LIGHTING);
				fig3.prisma2(1.0,1.0,1.0,text4.GLindex, NULL);
				glEnable(GL_LIGHTING);
			glPopMatrix();
		glPopMatrix();


		
		glDisable(GL_LIGHTING);
		/////////////////CASA///////////////////
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1f);
		glPushMatrix();		
			//Cuarto
			casa();
			//Lampara
			lampara();
			//Estante
			estante();
			//Television
			television();
			//Reloj
			reloj();
			//Cama
			cama();
			//Escritorio
			mesa();
			//Libreros
			glPushMatrix();
				//LibreroIzq
				glTranslatef(-4.25,5.0,-13.5);
				librero();
				glTranslatef(2.5, 0.0, 0.0);
				librero();
			glPopMatrix();
			//Silla
			silla();
			//Globo Terraqueo
			globo();
			//Habitacion 2//
			hab2();
			//Sofas
			glPushMatrix();
				glTranslatef(-16.0,0.0,0.0);
				lampara();
			glPopMatrix();
			glPushMatrix();				
				glTranslatef(-33.0, 0.0, -23.0);
				glRotatef(180, 0.0, 1.0, 0.0);
				sofa();
			glPopMatrix();
			glPushMatrix();									
				glTranslatef(-12.0, 0.0, -27.0);
				glRotatef(90, 0.0, 1.0, 0.0);
				sofa();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-30.0, 0.0, -2.0);
				glRotatef(-90, 0.0, 1.0, 0.0);
				sofa();
			glPopMatrix();
			//Papel Tapiz//
			tapiz();
			//Mesas//
			mesa2();
			glPushMatrix();						
				glScalef(2.0, 1.0, 2.0);
				glTranslatef(8, 0.0, 11.0);
				mesa2();
			glPopMatrix();



		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_LIGHTING);		
		













		glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
			glColor3f(1.0,0.0,0.0);
			pintaTexto(-11,12.0,-14.0,(void *)font,"PROYECTO FINAL CG LAB 2019_2");
			pintaTexto(-11,8.5,-14,(void *)font,s);
			glColor3f(1.0,1.0,1.0);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

	glutSwapBuffers ( );
}

void animacion(){
	//Movimiento del monito
	if(play)
	{		
		
		if(	i_curr_steps >= i_max_steps) //end of animation between frames?
		{			
			playIndex++;		
			if(playIndex>FrameIndex-2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex=0;
				play=false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();

			}
		}
		else
		{
			//Draw animation
			posX+=KeyFrame[playIndex].incX;
			posY+=KeyFrame[playIndex].incY;
			posZ+=KeyFrame[playIndex].incZ;

			rotRodIzq +=KeyFrame[playIndex].rotInc;
			rotRodDer += KeyFrame[playIndex].rotInc2;

			rotBraIzq += KeyFrame[playIndex].rotInc3;
			rotBraDer += KeyFrame[playIndex].rotInc4;

			giroMonito+=KeyFrame[playIndex].giroMonitoInc;

			i_curr_steps++;
		}
		
	}

	//Movimiento Pantalla
	if (pantalla) {
		Sleep(400);
		fig1.text_izq += 0.04;
		fig1.text_der += 0.04;			
		if (fig1.text_der >= 1) {
			fig1.text_der = 0.04;
			fig1.text_izq = 0.0;
		}
		//PlaySound(TEXT("Sonidos.wav"), NULL, SND_SYNC);//Música de Fondo//
	}
	

	glutPostRedisplay();
	//Movimiento Reloj
	if (clock) {
		for(int i=0;i<1000000;i++){}
		seg++;
		printf("\n%f:%d:%d",hor,min,seg);
		if (hor == -360) {
			hor = 0;
		}
		if (min == 61) {
			min = 1;
			hor++;
		}else if (seg == 61) {
			seg = 1;
			min++;
			hor -= 0.5;
		}								
	}

	//Movimineto GloboT
	if (globoT) {
		glob++;
		if (glob == 360)
			glob = 0;
	}
	
}

void reshape ( int width , int height )   // Creamos funcion Reshape
{
  if (height==0)										// Prevenir division entre cero
	{
		height=1;
	}

	glViewport(0,0,width,height);	

	glMatrixMode(GL_PROJECTION);						// Seleccionamos Projection Matrix
	glLoadIdentity();

	// Tipo de Vista
	
	glFrustum (-0.1, 0.1,-0.1, 0.1, 0.1, 170.0);

	glMatrixMode(GL_MODELVIEW);							// Seleccionamos Modelview Matrix
	glLoadIdentity();
}

void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key ) {
		case 'w':   //Movimientos de camara
		case 'W':
			objCamera.Move_Camera( CAMERASPEED+0.2 );
			break;

		case 's':
		case 'S':
			objCamera.Move_Camera(-(CAMERASPEED+0.2));
			break;

		case 'a':
		case 'A':
			objCamera.Strafe_Camera(-(CAMERASPEED+0.4));
			break;

		case 'd':
		case 'D':
			objCamera.Strafe_Camera( CAMERASPEED+0.4 );
			break;

		case 'k':		//
		case 'K':
			if(FrameIndex<MAX_FRAMES)
			{
				saveFrame();
			}

			break;

		case 'l':						
		case 'L':
			if(play==false && (FrameIndex>1))
			{

				resetElements();
				//First Interpolation				
				interpolation();

				play=true;
				playIndex=0;
				i_curr_steps = 0;
			}
			else
			{
				play=false;
			}
			break;

		case 'y':						
		case 'Y':
			posZ++;
			printf("%f \n", posZ);
			break;

		case 'g':						
		case 'G':
			globoT=!globoT;
			break;

		case 'h':						
		case 'H':
			posZ--;
			printf("%f \n", posZ);
			break;

		case 'j':						
		case 'J':
			posX++;
			printf("%f \n", posX);
			break;

		case 'c':
			rotBraIzq++;
			printf("%f \n", rotBraIzq);
			break;

		case 'C':
			rotBraIzq--;
			printf("%f \n", rotBraIzq);
			break;

		case 'v':
			rotBraDer++;
			printf("%f \n", rotBraDer);
			break;

		case 'V':
			rotBraDer--;
			printf("%f \n", rotBraDer);
			break;

		case 'n':
			rotRodDer++;
			printf("%f \n", rotRodDer);
			break;

		case 'N':
			rotRodDer--;
			printf("%f \n", rotRodDer);
			break;

		case 'b':						
			rotRodIzq++;
			printf("%f \n", rotRodIzq);
			break;

		case 'B':						
			rotRodIzq--;
			printf("%f \n", rotRodIzq);
			break;

		case 'p':						
			pantalla=!pantalla;
			break;

		case 'P':						
			pantalla = !pantalla;
			break;

		case 'r':
			clock=!clock;
			break;

		case 'R':
			clock =!clock;
			break;

		case 27:        // Cuando Esc es presionado...
			exit ( 0 );   // Salimos del programa
			break;        
		default:        // Cualquier otra
			break;
  }

  glutPostRedisplay();
}

void arrow_keys ( int a_keys, int x, int y )  // Funcion para manejo de teclas especiales (arrow keys)
{
  switch ( a_keys ) {
	case GLUT_KEY_PAGE_UP:
		objCamera.UpDown_Camera(CAMERASPEED);
		break;

	case GLUT_KEY_PAGE_DOWN:
		objCamera.UpDown_Camera(-CAMERASPEED);
		break;

    case GLUT_KEY_UP:     // Presionamos tecla ARRIBA...
		g_lookupdown -= 1.0f;
		break;

    case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
		g_lookupdown += 1.0f;
		break;

	case GLUT_KEY_LEFT:
		objCamera.Rotate_View(-CAMERASPEED);
		break;

	case GLUT_KEY_RIGHT:
		objCamera.Rotate_View( CAMERASPEED);
		break;

    default:
		break;
  }
  glutPostRedisplay();
}

void menuKeyFrame( int id)
{
	switch (id)
	{
		case 0:	//Save KeyFrame
			if(FrameIndex<MAX_FRAMES)
			{
				saveFrame();
			}
			break;

		case 1:	//Play animation
			if(play==false && FrameIndex >1)
			{

				resetElements();
				//First Interpolation
				interpolation();

				play=true;
				playIndex=0;
				i_curr_steps = 0;
			}
			else
			{
				play=false;
			}
			break;


	}
}


void menu( int id)
{
	
}



int main ( int argc, char** argv )   // Main Function
{
  int submenu;
  glutInit            (&argc, argv); // Inicializamos OpenGL
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode (Clores RGB y alpha | Buffer Doble )
  glutInitWindowSize  (1300, 600);	// Tamaño de la Ventana
  glutInitWindowPosition (0, 0);	//Posicion de la Ventana
  glutCreateWindow    ("Proyecto Final CG TEORIA 2019_2"); // Nombre de la Ventana
  //glutFullScreen     ( );         // Full Screen
  InitGL ();						// Parametros iniciales de la aplicacion
  glutDisplayFunc     ( display );  //Indicamos a Glut función de dibujo
  glutReshapeFunc     ( reshape );	//Indicamos a Glut función en caso de cambio de tamano
  glutKeyboardFunc    ( keyboard );	//Indicamos a Glut función de manejo de teclado
  glutSpecialFunc     ( arrow_keys );	//Otras
  glutIdleFunc		  ( animacion );

  submenu = glutCreateMenu	  ( menuKeyFrame );
  glutAddMenuEntry	  ("Guardar KeyFrame", 0);
  glutAddMenuEntry	  ("Reproducir Animacion", 1);
  glutCreateMenu	  ( menu );
  glutAddSubMenu	  ("Animacion Monito", submenu);
 
  glutAttachMenu	  (GLUT_RIGHT_BUTTON);


  glutMainLoop        ( );          // 

  return 0;
}