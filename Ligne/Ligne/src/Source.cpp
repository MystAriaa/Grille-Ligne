#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

#define LONGUEUR_FENETRE 1276 //multiple de DECOUPAGE_GRILLE + 1 (plus propre)
#define HAUTEUR_FENETRE 715
#define DECOUPAGE_GRILLE 10

//--------------------------------------------------------------------------------------------------------------------------------

struct point //point de coordonée x,y
{
    double x, y;
};

point roundpoint(point a) //fct qui arroudi à l'entier inferieur +50%
{
    point b;
    b.x = (int)a.x ;
    b.y = (int)a.y ;
    return b;
}

int main(void) //fct principale
{
    GLFWwindow* window;

    int decoupage_ligne;

    point af, bf; //point a et b que l'on tape
    point ai, bi; //point a et b apres etre arrondi

    std::cout << "Longeur de la fenetre = " << LONGUEUR_FENETRE << " pixels. \n";
    std::cout << "Hauteur de la fenetre = " << HAUTEUR_FENETRE << " pixels. \n";
    std::cout << "La resolution choisie du grillage est de 1 cube pour " << DECOUPAGE_GRILLE << " pixels. \n";
    std::cout << "L'abscisse est donc bornee a Xmax = " << LONGUEUR_FENETRE/(DECOUPAGE_GRILLE+1)-LONGUEUR_FENETRE%(DECOUPAGE_GRILLE+1) << ". \n";
    std::cout << "L'ordonee est donc bornee a Ymax = " << HAUTEUR_FENETRE / (DECOUPAGE_GRILLE + 1) - HAUTEUR_FENETRE % (DECOUPAGE_GRILLE + 1) << ". \n";
    std::cout << "\n ---------------------------------------- \n";
    std::cout << "\nTapez l'abscisse de A : ";
    std::cin >> af.x;
    std::cout << "Tapez l'ordonnee de A : ";
    std::cin >> af.y;
    std::cout << "Tapez l'abscisse de B : ";
    std::cin >> bf.x;
    std::cout << "Tapez l'ordonnee de B : ";
    std::cin >> bf.y;

    ai = roundpoint(af); //arrondissement
    bi = roundpoint(bf);

    GLfloat lineVertices[] = //Ligne original calcule par la librairie
    {
        af.x*(DECOUPAGE_GRILLE+1), af.y* (DECOUPAGE_GRILLE + 1), 0,
        bf.x* (DECOUPAGE_GRILLE + 1), bf.y* (DECOUPAGE_GRILLE + 1), 0
    };

    //----------------------------CALCUL-DU-NOMBRE-DE-CASE-A-COLORIER--------------------------------------------------------------
    int deltax = abs(bi.x - ai.x) + 1;
    int deltay = abs(bi.y - ai.y) + 1;
    std::vector<point> ListeDesPointsDecouper = {};
    std::vector<point> ListeDesPointsDecouperRound = {};

    if (deltax > deltay) //on decoupe en deltax-1 parts sur l'abscisse
    {
        decoupage_ligne = deltax - 1;       
    }
    if (deltax <= deltay) //on decoupe en deltay-1 parts sur l'ordonnée
    {
        decoupage_ligne = deltay - 1;
    }
    for (int k = 0; k < decoupage_ligne; k++) //calcul les points de coupures
    {
        point temp;
        temp.x = (((ai.x + 0.5) * k + (bi.x + 0.5) * (decoupage_ligne - k)) / decoupage_ligne);
        temp.y = (((ai.y + 0.5) * k + (bi.y + 0.5) * (decoupage_ligne - k)) / decoupage_ligne);
        ListeDesPointsDecouper.push_back(temp);
    }
    for (int i = 0; i < ListeDesPointsDecouper.size(); i++) //on arrondi les points de coupures
    {
        ListeDesPointsDecouperRound.push_back(roundpoint(ListeDesPointsDecouper[i]));
    }

    //-------------------DEBUG-----------------------------------------------------------------------------------------------------
    std::cout << "\n \n \n \n";
    std::cout << "Debug ListeDesPointsDecouper \n";
    std::cout << "x" << "  " << "y" << "\n";
    for (int i = 0; i < ListeDesPointsDecouper.size(); i++)
    {
        std::cout << ListeDesPointsDecouper[i].x << "  " << ListeDesPointsDecouper[i].y;
        std::cout << "\n";
    }
    std::cout << "\n \n \n \n";
    std::cout << "Debug ListeDesPointsDecouperRound \n";
    std::cout << "x" << "  " << "y" << "\n";
    for (int i = 0; i < ListeDesPointsDecouperRound.size(); i++)
    {
        std::cout << ListeDesPointsDecouperRound[i].x << "  " << ListeDesPointsDecouperRound[i].y;
        std::cout << "\n";
    }
    //-------------------DEBUG-END-------------------------------------------------------------------------------------------------


    // Initialise la librairie graphique
    if (!glfwInit())
    {
        return -1;
    }
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(LONGUEUR_FENETRE, HAUTEUR_FENETRE, "Grille+Ligne", NULL, NULL);
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    // Make the window's context current
    glfwMakeContextCurrent(window);
    glViewport(0.0f, 0.0f, screenWidth, screenHeight); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, LONGUEUR_FENETRE, 0, HAUTEUR_FENETRE, 0, 600); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity(); // same as above comment

    GLfloat CouleurContour[] =
    {
        0, 255, 255,
        0, 255, 255,
        0, 255, 255,
        0, 255, 255
    };
    GLfloat CouleurInterieur[] =
    {
        255, 255, 255,
        255, 255, 255,
        255, 255, 255,
        255, 255, 255
    };
    GLfloat CouleurPoint[] =
    {
        0, 255, 0,
        0, 255, 0,
        0, 255, 0,
        0, 255, 0
    };
    GLfloat CouleurLigne[] =
    {
        255, 0, 0,
        255, 0, 0,
        255, 0, 0,
        255, 0, 0
    };

    GLfloat PointA[] =
    {
        (DECOUPAGE_GRILLE+1)*ai.x,(DECOUPAGE_GRILLE + 1)* ai.y, 0,
        (DECOUPAGE_GRILLE + 1)* ai.x,(DECOUPAGE_GRILLE + 1)* ai.y+DECOUPAGE_GRILLE, 0,
        (DECOUPAGE_GRILLE + 1)* ai.x + DECOUPAGE_GRILLE,(DECOUPAGE_GRILLE + 1)* ai.y + DECOUPAGE_GRILLE, 0,
        (DECOUPAGE_GRILLE + 1)* ai.x + DECOUPAGE_GRILLE,(DECOUPAGE_GRILLE + 1)* ai.y, 0     
    };
    GLfloat PointB[] =
    {
        (DECOUPAGE_GRILLE + 1) * bi.x,(DECOUPAGE_GRILLE + 1) * bi.y, 0,
        (DECOUPAGE_GRILLE + 1) * bi.x,(DECOUPAGE_GRILLE + 1) * bi.y + DECOUPAGE_GRILLE, 0,
        (DECOUPAGE_GRILLE + 1) * bi.x + DECOUPAGE_GRILLE,(DECOUPAGE_GRILLE + 1) * bi.y + DECOUPAGE_GRILLE, 0,
        (DECOUPAGE_GRILLE + 1) * bi.x + DECOUPAGE_GRILLE,(DECOUPAGE_GRILLE + 1) * bi.y, 0
    };

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render OpenGL here
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        for (int i = 0; i < LONGUEUR_FENETRE; i++) //Creation de la grille blanche + contour
        {
            for (int j = 0; j < HAUTEUR_FENETRE; j++)
            {
                GLfloat verticesij[] =
                {
                    i * DECOUPAGE_GRILLE + i, j * DECOUPAGE_GRILLE + j, 0,
                    i * DECOUPAGE_GRILLE + i, j * DECOUPAGE_GRILLE + j + DECOUPAGE_GRILLE, 0,
                    i * DECOUPAGE_GRILLE + i + DECOUPAGE_GRILLE, j * DECOUPAGE_GRILLE + j + DECOUPAGE_GRILLE, 0,
                    i * DECOUPAGE_GRILLE + i + DECOUPAGE_GRILLE, j * DECOUPAGE_GRILLE + j, 0
                };
                glVertexPointer(3, GL_FLOAT, 0, verticesij);
                if (i==0||j==0||i * DECOUPAGE_GRILLE + i + 2* DECOUPAGE_GRILLE > LONGUEUR_FENETRE ||j * DECOUPAGE_GRILLE + j + 2* DECOUPAGE_GRILLE > HAUTEUR_FENETRE)
                {
                    glColorPointer(3, GL_FLOAT, 0, CouleurContour);
                }
                else
                {
                    glColorPointer(3, GL_FLOAT, 0, CouleurInterieur);
                }
                glDrawArrays(GL_POLYGON, 0, 4);
            };   
        };
        
        for (int i = 0; i < ListeDesPointsDecouperRound.size(); i++) //creation des carrés rouge de la ligne
        {       
            GLfloat verticesLigne[] =
            {
                ListeDesPointsDecouperRound[i].x*(DECOUPAGE_GRILLE + 1),ListeDesPointsDecouperRound[i].y* (DECOUPAGE_GRILLE + 1),0,
                ListeDesPointsDecouperRound[i].x* (DECOUPAGE_GRILLE + 1),ListeDesPointsDecouperRound[i].y* (DECOUPAGE_GRILLE + 1) +DECOUPAGE_GRILLE,0,
                ListeDesPointsDecouperRound[i].x* (DECOUPAGE_GRILLE + 1) + DECOUPAGE_GRILLE,ListeDesPointsDecouperRound[i].y* (DECOUPAGE_GRILLE + 1) + DECOUPAGE_GRILLE,0,
                ListeDesPointsDecouperRound[i].x* (DECOUPAGE_GRILLE + 1) + DECOUPAGE_GRILLE,ListeDesPointsDecouperRound[i].y* (DECOUPAGE_GRILLE + 1),0
            };
            glVertexPointer(3, GL_FLOAT, 0, verticesLigne);
            glColorPointer(3, GL_FLOAT, 0, CouleurLigne);
            glDrawArrays(GL_POLYGON, 0, 4);
        }

        glVertexPointer(3, GL_FLOAT, 0, PointA); //Affichage des extremitées
        glColorPointer(3, GL_FLOAT, 0, CouleurPoint);
        glDrawArrays(GL_POLYGON, 0, 5);
        glVertexPointer(3, GL_FLOAT, 0, PointB);
        glColorPointer(3, GL_FLOAT, 0, CouleurPoint);
        glDrawArrays(GL_POLYGON, 0, 5);

        glColorPointer(3, GL_FLOAT, 0, CouleurContour); //Affichage de la ligne de debug
        glEnable(GL_LINE_SMOOTH);
        glPushAttrib(GL_LINE_BIT);
        glLineWidth(2);
        glVertexPointer(3, GL_FLOAT, 0, lineVertices);
        glDrawArrays(GL_LINES, 0, 2);


        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY); 



        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
