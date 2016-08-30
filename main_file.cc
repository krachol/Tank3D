

/*
   Niniejszy program jest wolnym oprogramowaniem; możesz go
   rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
   Licencji Publicznej GNU, wydanej przez Fundację Wolnego
   Oprogramowania - według wersji 2 tej Licencji lub(według twojego
   wyboru) którejś z późniejszych wersji.

   Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
   użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
   gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
   ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
   Powszechnej Licencji Publicznej GNU.

   Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
   Powszechnej Licencji Publicznej GNU(GNU General Public License);
   jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
   Place, Fifth Floor, Boston, MA  02110 - 1301  USA
   */

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include <modelloader.h>

#include <iostream>

using namespace std;
using namespace glm;

float speed_x = 0; // [radiany/s]
float speed_y = 0; // [radiany/s]

//Uchwyty na shadery
//Wskaźnik na obiekt reprezentujący program cieniujący.
ShaderProgram *shaderProgram; 

//Uchwyty na VAO i bufory wierzchołków
GLuint vao;
//Uchwyt na bufor VBO przechowujący tablicę współrzędnych wierzchołków
GLuint bufVertices; 
//Uchwyt na bufor VBO przechowujący tablicę kolorów
GLuint bufColors;  
//Uchwyt na bufor VBO przechowujący tablickę wektorów normalnych
GLuint bufNormals; 

//Kostka
/* float* vertices=Models::CubeInternal::vertices; */
/* float* colors=Models::CubeInternal::colors; */
/* float* normals=Models::CubeInternal::normals; */
/* int vertexCount=Models::CubeInternal::vertexCount; */

//Czajnik
float* vertices=Models::TeapotInternal::vertices;
float* colors=Models::TeapotInternal::colors;
float* normals=Models::TeapotInternal::vertexNormals;
int vertexCount=Models::TeapotInternal::vertexCount;

void key_callback(GLFWwindow* , int key,
                  int , int action, int );
void error_callback(int error, const char* description);
GLuint makeBuffer(void *data, int vertexCount, int vertexSize);

//Procedura obsługi klawiatury

void key_callback(GLFWwindow *, int key, int, int action, int) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT) speed_y = (float) -3.14;
        if (key == GLFW_KEY_RIGHT) speed_y = 3.14;
        if (key == GLFW_KEY_UP) speed_x = (float) -3.14;
        if (key == GLFW_KEY_DOWN) speed_x = 3.14;
    }


    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT) speed_y = 0;
        if (key == GLFW_KEY_RIGHT) speed_y = 0;
        if (key == GLFW_KEY_UP) speed_x = 0;
        if (key == GLFW_KEY_DOWN) speed_x = 0;
    }
}


void error_callback(int , const char *description) {
    fputs(description, stderr);

}

GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
    GLuint handle;

    //Wygeneruj uchwyt na Vertex Buffer Object (VBO),
    //który będzie zawierał tablicę danych
    glGenBuffers(1,&handle);
    //Uaktywnij wygenerowany uchwyt VBO
    glBindBuffer(GL_ARRAY_BUFFER,handle);
    //Wgraj tablicę do VBO
    glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);

    return handle;
}

//Tworzy bufor VBO z tablicy

//Przypisuje bufor VBO do atrybutu 
void assignVBOtoAttribute(ShaderProgram *shaderProgram,
        const char* attributeName, GLuint bufVBO, int vertexSize) {
    //Pobierz numery slotów dla atrybutu
    GLuint location=shaderProgram->getAttribLocation(attributeName); 
    glBindBuffer(GL_ARRAY_BUFFER,bufVBO);  //Uaktywnij uchwyt VBO 
    //Włącz używanie atrybutu o numerze slotu zapisanym w zmiennej location
    glEnableVertexAttribArray(location); 
    //Dane do slotu location mają być brane z aktywnego VBO
    glVertexAttribPointer(location,vertexSize,GL_FLOAT, GL_FALSE, 0, NULL); 
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    //***Tutaj umieszczaj kod, który należy wykonać raz, na początku programu***
    glClearColor(0, 0, 0, 1); //Czyść ekran na czarno	
    glEnable(GL_DEPTH_TEST); //Włącz używanie Z-Bufora
    //Zarejestruj procedurę obsługi klawiatury
    glfwSetKeyCallback(window, key_callback);


    //Wczytaj program cieniujący 
    shaderProgram=new ShaderProgram("vshader.txt",NULL,"fshader.txt"); 


    //*****Przygotowanie do rysowania pojedynczego obiektu*******
    //Zbuduj VBO z danymi obiektu do narysowania
    //VBO ze współrzędnymi wierzchołków
    bufVertices=makeBuffer(vertices, vertexCount, sizeof(float)*4); 
    //VBO z kolorami wierzchołków
    bufColors=makeBuffer(colors, vertexCount, sizeof(float)*4);
    //VBO z wektorami normalnymi wierzchołków
    bufNormals=makeBuffer(normals, vertexCount, sizeof(float)*4);

    //Zbuduj VAO wiążący atrybuty z konkretnymi VBO
    //Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej
    glGenVertexArrays(1,&vao); 

    glBindVertexArray(vao); //Uaktywnij nowo utworzony VAO

    //"vertex" odnosi się do deklaracji "in vec4 vertex;" w vertex shaderze
    assignVBOtoAttribute(shaderProgram,"vertex",bufVertices,4); 
    //"color" odnosi się do deklaracji "in vec4 color;" w vertex shaderze
    assignVBOtoAttribute(shaderProgram,"color",bufColors,4); 
    //"normal" odnosi się do deklaracji "in vec4 normal;" w vertex shaderze
    assignVBOtoAttribute(shaderProgram,"normal",bufNormals,4); 

    glBindVertexArray(0); //Dezaktywuj VAO
    //******Koniec przygotowania obiektu************

}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram() {
    delete shaderProgram; //Usunięcie programu cieniującego

    glDeleteVertexArrays(1,&vao); //Usunięcie vao
    glDeleteBuffers(1,&bufVertices); //Usunięcie VBO z wierzchołkami
    glDeleteBuffers(1,&bufColors); //Usunięcie VBO z kolorami
    glDeleteBuffers(1,&bufNormals); //Usunięcie VBO z wektorami normalnymi


}

void drawObject(GLuint vao, ShaderProgram *shaderProgram, 
        mat4 mP, mat4 mV, mat4 mM) {
    //Włączenie programu cieniującego, który ma zostać użyty do rysowania
    //ale chodzi o pokazanie, 
    //W tym programie wystarczyłoby wywołać to raz, w setupShaders, 
    //że mozna zmieniać program cieniujący podczas rysowania jednej sceny
    shaderProgram->use();

    //Przekaż do shadera macierze P,V i M.
    //W linijkach poniżej, polecenie:
    //  shaderProgram->getUniformLocation("P") 
    //pobiera numer slotu odpowiadającego zmiennej jednorodnej o podanej nazwie
    //UWAGA! "P" w powyższym poleceniu odpowiada deklaracji "uniform mat4 P;" 
    //w vertex shaderze, 
    //a mP w glm::value_ptr(mP) odpowiada argumentowi  "mat4 mP;" TYM pliku.
    //Cała poniższa linijka przekazuje do zmiennej jednorodnej P w 
    //vertex shaderze dane z argumentu mP niniejszej funkcji
    //Pozostałe polecenia działają podobnie.
    glUniformMatrix4fv(
            shaderProgram->getUniformLocation("P"),
            1,
            (GLboolean) false,
            glm::value_ptr(mP));
    glUniformMatrix4fv(
            shaderProgram->getUniformLocation("V"),
            1,
            (GLboolean) false,
            glm::value_ptr(mV));
    glUniformMatrix4fv(
            shaderProgram->getUniformLocation("M"),
            1,
            (GLboolean) false,
            glm::value_ptr(mM));

    //Przekazanie współrzędnych źródła światła do zmiennej jednorodnej lightPos0
    glUniform4f(shaderProgram->getUniformLocation("lightPos0"), 0,0,-5,1); 

    //Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych 
    //w tym VAO powiązań slotów atrybutów z tablicami z danymi
    glBindVertexArray(vao);

    //Narysowanie obiektu
    glDrawArrays(GL_TRIANGLES,0,vertexCount);

    //Posprzątanie po sobie (niekonieczne w sumie jeżeli 
    //korzystamy z VAO dla każdego rysowanego obiektu)
    glBindVertexArray(0);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y) {
    //************Tutaj umieszczaj kod rysujący obraz******************l

    //Wykonaj czyszczenie bufora kolorów
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

    //Wylicz macierz rzutowania
    glm::mat4 P = glm::perspective(50 * PI / 180, 1.0f, 1.0f, 50.0f); 

    glm::mat4 V = glm::lookAt( //Wylicz macierz widoku
            glm::vec3(0.0f, 0.0f, -15.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));


    //Wylicz macierz modelu rysowanego obiektu
    glm::mat4 M = glm::mat4(1.0f); 
    M = glm::rotate(M, angle_x, glm::vec3(1, 0, 0)); 
    M = glm::rotate(M, angle_y, glm::vec3(0, 1, 0)); 

    //Narysuj obiekt
    drawObject(vao,shaderProgram,P,V,M);

    //Przerzuć tylny bufor na przedni
    glfwSwapBuffers(window);
}



int main(void)
{
    Models::ObjModel mymodel("TigerII.obj");
    vertices = mymodel.vertices;
    normals = mymodel.vertexNormals;
    colors = mymodel.colors;
    vertexCount = mymodel.vertexCount;

    GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

    glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

    if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
        fprintf(stderr, "Nie można zainicjować GLFW.\n");
        exit(EXIT_FAILURE); 
    }

    //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 
    window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  


    if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
    {
        fprintf(stderr, "Nie można utworzyć okna.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    //Od tego momentu kontekst okna staje się aktywny 
    //i polecenia OpenGL będą dotyczyć właśnie jego.
    glfwMakeContextCurrent(window); 
    //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora
    glfwSwapInterval(1); 

    if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
        fprintf(stderr, "Nie można zainicjować GLEW.\n");
        exit(EXIT_FAILURE);
    }

    initOpenGLProgram(window); //Operacje inicjujące

    float angle_x = 0; //Kąt obrotu obiektu
    float angle_y = 0; //Kąt obrotu obiektu

    glfwSetTime(0); //Wyzeruj licznik czasu

    //Główna pętla
    //Tak długo jak okno nie powinno zostać zamknięte
    while (!glfwWindowShouldClose(window)) 
    {
        //Zwiększ kąt o prędkość kątową razy 
        //czas jaki upłynął od poprzedniej klatki
        angle_x += speed_x*glfwGetTime(); 
        //Zwiększ kąt o prędkość kątową razy czas 
        //jaki upłynął od poprzedniej klatki
        angle_y += speed_y*glfwGetTime(); 
        glfwSetTime(0); //Wyzeruj licznik czasu
        drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
        //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
        glfwPollEvents(); 
    }

    freeOpenGLProgram();

    glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
    glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
    exit(EXIT_SUCCESS);
}

