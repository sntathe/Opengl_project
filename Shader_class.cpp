#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_s.h"

#include <iostream>

#include "Line.h"
#include "Point.h"
#include <vector>
#include "Circle.h"
#include <string>
#include <string.h>

#include <math.h>

using namespace std;
constexpr auto NUMBER_OF_VERTICES = 100;
#define M_PI 3.14

string ongoing_command = "";
string ongoing_command_fb = "";
Point* line_start = NULL;
Point* line_end = NULL;

Point* cen_cir = NULL;
bool mouse_capture = false;

float cir_rad;

float geo_color[3] = { 0.0f , 0.0f , 1.0f };
int poly_count = 0;
Point* poly_cen = NULL;



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window , Shader os);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);



// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

std::vector<Line*> Lines;
std::vector<Circle*> Circles;
std::vector<unsigned int> Circles_VAO;
std::vector<unsigned int> Lines_VAO;

std::vector<unsigned int>  poly_VAO;

unsigned int Circle_VAO_fb;

unsigned int Line_VAO_fb;




bool compare_float(float x, float y, float epsilon = 0.01f) {
    if (fabs(x - y) < epsilon)
        return true; //they are same
    return false; //they are not same
}

float distance_two_points(Point p1, Point p2)
{
    return sqrt(((p1.x - p2.x) * (p1.x - p2.x)) + ((p1.y - p2.y) * (p1.y - p2.y)));
}


int  AskforSelection()
{
    int selection = 0;
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << "Draw Shapes" << std::endl;
    std::cout << "1. Draw free geometry" << std::endl;
    std::cout << "2. Draw Line from ViewPort" << std::endl;
    std::cout << "3. Draw Circle from ViewPort" << std::endl;
    std::cout << "4. Draw Concentric Circle from ViewPort" << std::endl;

    std::cout << "5. Draw Polylines from ViewPort" << std::endl;
    std::cout << "6. Draw Line with Command Prompt" << std::endl;
    std::cout << "7. Draw Circle with Command Prompt" << std::endl;
    std::cout << "8. Draw Polygon" << std::endl;
    std::cout << "9. Change geometry Color" << std::endl;
    std::cout << "10. Clear the screen" << std::endl;
    std::cout << "Enter your selection...." << std::endl;
    cin >> selection;
    std::cout << "-----------------------------------------------------------------" << std::endl;
    return selection;
}


void AskForColor(float* geo_color)
{
    int selection = 0;
    std::cout << "Geometry color" << std::endl;
    std::cout << "1.RED" << std::endl;
    std::cout << "2.BLUE" << std::endl;
    std::cout << "3.GREEN" << std::endl;
    std::cout << "4.YELLOW" << std::endl;
    std::cout << "Enter your selection...." << std::endl;
    cin >> selection;
    switch (selection)
    {
    case 1:
        geo_color[0] = 1.0;
        geo_color[1] = 0.0;
        geo_color[2] = 0.0;
        std::cout << "Geometry color is RED" << std::endl;

        break;
    case 2:
        geo_color[0] = 0.0;
        geo_color[1] = 0.0;
        geo_color[2] = 1.0;
        std::cout << "Geometry color is BLUE" << std::endl;

        break;
    case 3:
        geo_color[0] = 0.0;
        geo_color[1] = 1.0;
        geo_color[2] = 0.0;
        std::cout << "Geometry color is GREEN" << std::endl;

        break;
    case 4:
        geo_color[0] = 1.0;
        geo_color[1] = 1.0;
        geo_color[2] = 0.0;
        std::cout << "Geometry color is YELLOW" << std::endl;

        break;

    default:
        break;
    }

}



Line* AskforLineInputs()

{
    float ver_start[3];
    float ver_end[3];

    std::cout << "Enter start point coordinates" << std::endl;

    for (int i = 0; i < 3; i++)
    {
        cin >> ver_start[i];
    }

    std::cout << "Enter end point coordinates" << std::endl;

    for (int i = 0; i < 3; i++)
    {
        cin >> ver_end[i];
    }

    Point* p1 = new Point(ver_start);
    Point* p2 = new Point(ver_end);
    Line* l1 = new Line(p1, p2);
    return l1;
}


Circle* AskforCircleInputs()

{
    float cen[3];
    float rad;
    std::cout << "Enter center point coordinates" << std::endl;

    for (int i = 0; i < 3; i++)
    {
        cin >> cen[i];
    }
    std::cout << "Enter radius" << std::endl;
    cin >> rad;
    Point* p1 = new Point(cen);
    Circle* c = new Circle(p1, rad);
    return c;
}


int AskForPolygonCount()
{
    int count;
    std::cout << "Enter number of sides" << std::endl;
    cin >> count;
    return count;
}


unsigned int draw_coordinate_axii()
{
    vector<float> vertices;

    float x = 1.0f;
    for (int i = 0; i < 50; i++)
    {
        x = x - 0.05;
        if (x > -0.05 && x < 0)
        {
            vertices.push_back(x); vertices.push_back(1.0f); vertices.push_back(0.0f);
            vertices.push_back(0.7f); vertices.push_back(0.0f); vertices.push_back(0.0f);
            vertices.push_back(x); vertices.push_back(-1.0f); vertices.push_back(0.0f);
            vertices.push_back(0.7f); vertices.push_back(0.0f); vertices.push_back(0.0f);
        }
        else
        {
            vertices.push_back(x); vertices.push_back(1.0f); vertices.push_back(0.0f);
            vertices.push_back(0.2f); vertices.push_back(0.2f); vertices.push_back(0.2f);
            vertices.push_back(x); vertices.push_back(-1.0f); vertices.push_back(0.0f);
            vertices.push_back(0.2f); vertices.push_back(0.2f); vertices.push_back(0.2f);
        }

    }

    float y = 1.0f;
    for (int i = 0; i < 50; i++)
    {
        y = y - 0.05;

        if (y > -0.05 && y < 0)
        {
            vertices.push_back(1.0f); vertices.push_back(y); vertices.push_back(0.0f);
            vertices.push_back(0.7f); vertices.push_back(0.0f); vertices.push_back(0.0f);

            vertices.push_back(-1.0f); vertices.push_back(y); vertices.push_back(0.0f);
            vertices.push_back(0.7f); vertices.push_back(0.0f); vertices.push_back(0.0f);
        }
        else
        {
            vertices.push_back(1.0f); vertices.push_back(y); vertices.push_back(0.0f);
            vertices.push_back(0.2f); vertices.push_back(0.2f); vertices.push_back(0.2f);

            vertices.push_back(-1.0f); vertices.push_back(y); vertices.push_back(0.0f);
            vertices.push_back(0.2f); vertices.push_back(0.2f); vertices.push_back(0.2f);
        }
    }
    for (auto itr = Lines.begin(); itr != Lines.end(); itr++)
    {
        Point* p1 = (*itr)->start;
        Point* p2 = (*itr)->start;
        vertices.push_back(p1->x); vertices.push_back(p1->y); vertices.push_back(p1->z);
        vertices.push_back(0.7f); vertices.push_back(0.0f); vertices.push_back(0.0f);

    }

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    int s = vertices.size();

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, s * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}
void RenderGeometry( Shader ourShader , GLFWwindow* window)
{
   
    unsigned int VAO_axii = draw_coordinate_axii();
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // render the triangle
        ourShader.use();
        ourShader.setVec4("ouripColor", 0.2f, 0.2f, 0.2f, 1.0f);
        glBindVertexArray(VAO_axii);
        glDrawArrays(GL_LINES, 0, 200);

        if (Lines.size() > 0)
        {
            for (auto itr = Lines_VAO.begin(); itr != Lines_VAO.end(); itr++)
            {
                glBindVertexArray(*itr);

                glDrawArrays(GL_LINE_LOOP, 0, 2);
            }
        }
          

        if (Circles.size() > 0)
        {
            for (auto itr = Circles_VAO.begin(); itr != Circles_VAO.end(); itr++)
            {
                glBindVertexArray(*itr);

                glDrawArrays(GL_LINE_LOOP, 0,  NUMBER_OF_VERTICES);
                }
        }

        if (Circle_VAO_fb > 0)
        {
            glBindVertexArray(Circle_VAO_fb);

            glDrawArrays(GL_LINE_LOOP, 0, NUMBER_OF_VERTICES);
        }

        if (Line_VAO_fb > 0)
        {
            glBindVertexArray(Line_VAO_fb);

            glDrawArrays(GL_LINE_LOOP, 0, 2);
        }

    


        if (poly_VAO.size() > 0)
        {
            for (auto itr = poly_VAO.begin(); itr != poly_VAO.end(); itr++)
            {
                glBindVertexArray(*itr);

                glDrawArrays(GL_LINE_LOOP, 0, 2);
            }
        }
        



        
        
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        processInput(window, ourShader);
    }
    glDeleteVertexArrays(1, &VAO_axii);
    glfwTerminate();
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("D:/OPENGL/Draw_Shapes_proj/Project_1/Project_1/src/3.3.shader.vs", "D:/OPENGL/Draw_Shapes_proj/Project_1/Project_1/src/3.3.shader.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    RenderGeometry( ourShader, window);
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------


void create_Line_for_Opengl(Line *l1)
{

    Lines.push_back(l1);
    vector<float> vertices;
    Point* p1 = l1->start;
    Point* p2 = l1->end;
    vertices.push_back(p1->x); vertices.push_back(p1->y); vertices.push_back(p1->z);
    vertices.push_back(geo_color[0]); vertices.push_back(geo_color[1]); vertices.push_back(geo_color[2]);
    vertices.push_back(p2->x); vertices.push_back(p2->y); vertices.push_back(p2->z);
    vertices.push_back(geo_color[0]); vertices.push_back(geo_color[1]); vertices.push_back(geo_color[2]);
    unsigned int VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    int s = vertices.size();
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, s * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    Lines_VAO.push_back(VAO);
}

void create_polygon_for_Opengl_fb_fun(Line* l1)
{

    Lines.push_back(l1);
    vector<float> vertices;
    Point* p1 = l1->start;
    Point* p2 = l1->end;
    vertices.push_back(p1->x); vertices.push_back(p1->y); vertices.push_back(p1->z);
    vertices.push_back(geo_color[0]); vertices.push_back(geo_color[1]); vertices.push_back(geo_color[2]);
    vertices.push_back(p2->x); vertices.push_back(p2->y); vertices.push_back(p2->z);
    vertices.push_back(geo_color[0]); vertices.push_back(geo_color[1]); vertices.push_back(geo_color[2]);
    unsigned int VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    int s = vertices.size();
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, s * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    poly_VAO.push_back(VAO);
}


void create_Line_for_Opengl_fb(Line* l1)
{

    Lines.push_back(l1);
    vector<float> vertices;
    Point* p1 = l1->start;
    Point* p2 = l1->end;
    vertices.push_back(p1->x); vertices.push_back(p1->y); vertices.push_back(p1->z);
    vertices.push_back(geo_color[0]); vertices.push_back(geo_color[1]); vertices.push_back(geo_color[2]);
    vertices.push_back(p2->x); vertices.push_back(p2->y); vertices.push_back(p2->z);
    vertices.push_back(geo_color[0]); vertices.push_back(geo_color[1]); vertices.push_back(geo_color[2]);
    unsigned int VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    int s = vertices.size();
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, s * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    Line_VAO_fb = VAO;
}


void create_Circle_for_Opengl(Circle* c1)
{

    Circles.push_back(c1);
    std::vector<float> vertexBuffer;
    for (double i = 0; i < 2 * M_PI; i += 2 * M_PI / NUMBER_OF_VERTICES) {
        vertexBuffer.push_back(cos(i) * c1->rad + c1->cen->x);    //X coordinate
        vertexBuffer.push_back(sin(i) * c1->rad + c1->cen->y);    //Y coordinate
        vertexBuffer.push_back(0.0);                //Z coordinate
        vertexBuffer.push_back(geo_color[0]); vertexBuffer.push_back(geo_color[1]); vertexBuffer.push_back(geo_color[2]);
    }
    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

    // position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    Circles_VAO.push_back(VAO);
    

}


void  create_polygon_for_Opengl_fb(float rad)
{

    float r = rad;
    float x_centre = poly_cen->x;
    float y_centre = poly_cen->y;
    float x;
    float y;

    Line* prev = NULL;
    Line* curr = NULL;
    Point* prev_pnt = NULL;
    Point* curr_pnt = NULL;


    for (int i = 0; i <= poly_count; i++)
    {
        y = r * cos(2 * M_PI * i / poly_count) + y_centre;
        x = r * sin(2 * M_PI * i / poly_count) + x_centre;

        float curr_vec[3];
        curr_vec[0] = x;
        curr_vec[1] = y;
        curr_vec[2] = 0;

        curr_pnt = new Point(curr_vec);

        if (i > 0)
        {

            Line* l1 = new Line(prev_pnt, curr_pnt);
            create_polygon_for_Opengl_fb_fun(l1);
        }

        prev_pnt = curr_pnt;

    }
}



void create_Circle_for_Opengl_fb(Circle* c1)
{

    Circles.push_back(c1);
    std::vector<float> vertexBuffer;
    for (double i = 0; i < 2 * M_PI; i += 2 * M_PI / NUMBER_OF_VERTICES) {
        vertexBuffer.push_back(cos(i) * c1->rad + c1->cen->x);    //X coordinate
        vertexBuffer.push_back(sin(i) * c1->rad + c1->cen->y);    //Y coordinate
        vertexBuffer.push_back(0.0);                //Z coordinate
        vertexBuffer.push_back(geo_color[0]); vertexBuffer.push_back(geo_color[1]); vertexBuffer.push_back(geo_color[2]);
    }
    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

    // position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    Circle_VAO_fb = VAO;


}

void createPolygon(float rad)
{
    float r = rad;
    float x_centre = poly_cen->x;
    float y_centre = poly_cen->y;
    float x;
    float y;

    Line* prev = NULL;
    Line* curr = NULL;
    Point* prev_pnt = NULL;
    Point* curr_pnt = NULL;


    for (int i = 0; i <= poly_count; i++)
    {
        y = r * cos(2 * M_PI * i / poly_count) + y_centre;
        x = r * sin(2 * M_PI * i / poly_count) + x_centre;

        float curr_vec[3];
        curr_vec[0] = x;
        curr_vec[1] = y;
        curr_vec[2] = 0;

        curr_pnt = new Point(curr_vec);

        if (i > 0)
        {

            Line* l1 = new Line(prev_pnt, curr_pnt);
            create_Line_for_Opengl(l1);
        }

        prev_pnt = curr_pnt;

    }
}
void processInput(GLFWwindow* window , Shader ourShader)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        ongoing_command = "";
         ongoing_command = "";
         line_start = NULL;
         line_end = NULL;

        cen_cir = NULL;

        unsigned int Circle_VAO_fb = 0 ;

        unsigned int Line_VAO_fb = 0 ;
         mouse_capture = false;
    }

    if (ongoing_command == "")
    {
        Line* l1 = NULL;
        int ip;
        int sel = AskforSelection();
        if (sel > 0 && sel < 11)
        {
            if (sel == 1)
            {
                ongoing_command = "FreeGeom";
                cout << "Drawing FreeGeom" << endl;
                cout << "Drag Mouse by left click" << endl;
                cout << "Press Q button on viewport to end the current command" << endl;

            }
            else if (sel == 2)
            {
                ongoing_command = "line";
                cout << "Drawing Line" << endl;
                cout << "Mark start Point on viewport" << endl;
            }
            else if (sel == 3)
            {
                Circle* c1 = NULL;
                ongoing_command = "circle";
                cout << "Drawing Circle" << endl;
                cout << "Mark center Point on viewport" << endl;
            }

            else if (sel == 4)
            {
                Circle* c1 = NULL;
                ongoing_command = "con_circle";
                cout << "Drawing Concetric Circles" << endl;
                cout << "Mark Center Point on viewport" << endl;
            }

            else if (sel == 5)
            {
                Circle* c1 = NULL;
                ongoing_command = "Polyline";
                cout << "Drawing Polyline" << endl;
                cout << "Mark start Point on viewport" << endl;
            }

            else if (sel == 6)
            {
                l1 = AskforLineInputs();
                if (l1)
                {
                    create_Line_for_Opengl(l1);
                    ongoing_command = "";
                }
            }

            else if (sel == 7)
            {
                Circle* c1 = NULL;
                c1 = AskforCircleInputs();
                if (c1)
                {
                    create_Circle_for_Opengl(c1);
                    ongoing_command = "";

                }

            }
            else if (sel == 8)
            {

                poly_count =AskForPolygonCount();
                cout << "Mark center Point on viewport" << endl;
                ongoing_command = "polygon";

            }

            else if (sel == 9)
            {

                AskForColor(geo_color);
                ongoing_command = "";

            }
            else if (sel == 10)
            {
                Lines_VAO.clear();
                Circles_VAO.clear();
            }
            else
            {
                AskforSelection();
            }

        }
       
        else
        {
            AskforSelection();
        }
     }


    else if ( (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && (ongoing_command != "FreeGeom"))
    {
        mouse_capture = true;
    }
    else if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) && (ongoing_command == "FreeGeom"))
    {
        line_start = NULL;
        line_end = NULL;
    }

    else if (((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) && !mouse_capture))

    {
        

        if (ongoing_command_fb == "circle")
        {
            if (cen_cir)
            {
                double xpos, ypos;
                //getting cursor position

                glfwGetCursorPos(window, &xpos, &ypos);
                double xpos_actual, ypos_actual;
                xpos = -1 * (((SCR_WIDTH / 2) - xpos) / (SCR_WIDTH / 2));
                ypos = ((SCR_HEIGHT / 2) - ypos) / (SCR_HEIGHT / 2);


                if (!(compare_float(cen_cir->x, xpos) && compare_float(cen_cir->y, ypos)))
                {
                    Point end;

                    end.x = xpos;
                    end.y = ypos;
                    cir_rad = distance_two_points(*cen_cir, end);
                    //cout << "Radius is " << cir_rad << endl;

                    Point* cen_cir2 = new Point(cen_cir->x, cen_cir->y, 0.0);

                    Circle* c1 = new Circle(cen_cir2, cir_rad);
                    create_Circle_for_Opengl_fb(c1);
                }

            }
        }

        if (ongoing_command_fb == "line")
        {
            if (line_start)
            {
                double xpos, ypos;
                //getting cursor position

                glfwGetCursorPos(window, &xpos, &ypos);
                double xpos_actual, ypos_actual;
                xpos = -1 * (((SCR_WIDTH / 2) - xpos) / (SCR_WIDTH / 2));
                ypos = ((SCR_HEIGHT / 2) - ypos) / (SCR_HEIGHT / 2);
                Point end;


                if (!(compare_float(line_start->x, xpos) && compare_float(line_start->y, ypos)))
                {
                    end.x = xpos;
                    end.y = ypos;
                    float end_vec[3];
                    end_vec[0] = end.x;
                    end_vec[1] = end.y;
                    end_vec[2] = 0;

                    line_end = new Point(end_vec);

                    Line* l1 = new Line(line_start, line_end);


                    create_Line_for_Opengl_fb(l1);
                }
            }

        }


        if (ongoing_command_fb == "polygon")
        {
            poly_VAO.clear();
            if (poly_count > 2 && poly_cen)
            {
                double xpos, ypos;
                //getting cursor position

                glfwGetCursorPos(window, &xpos, &ypos);
                double xpos_actual, ypos_actual;
                xpos = -1 * (((SCR_WIDTH / 2) - xpos) / (SCR_WIDTH / 2));
                ypos = ((SCR_HEIGHT / 2) - ypos) / (SCR_HEIGHT / 2);
                Point end;
                end.x = xpos;
                end.y = ypos;
                end.z = 0.0f;

                float rad = 0.0f;
                rad = distance_two_points(*poly_cen, end);


                //if (!(compare_float(line_start->x, xpos) && compare_float(line_start->y, ypos)))
               // {
                   


                    create_polygon_for_Opengl_fb( rad );
                    
                //}
            }

        }

       
    }
    else if ( ( (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) && mouse_capture && (ongoing_command != "FreeGeom")) ||
        ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && (ongoing_command == "FreeGeom")))
    {
        mouse_capture = false;
        bool is_cen_taken = false;
        double xpos, ypos;
        //getting cursor position
       
        glfwGetCursorPos(window, &xpos, &ypos);
        double xpos_actual, ypos_actual;
        xpos = -1 * (((SCR_WIDTH / 2) - xpos) / (SCR_WIDTH / 2));
        ypos = ((SCR_HEIGHT / 2) - ypos) / (SCR_HEIGHT / 2);

        if (ongoing_command == "line" || ongoing_command == "Polyline" || ongoing_command == "FreeGeom")
        {
            Point start;
            Point end;

            if (!line_start)
            {
                start.x = xpos;
                start.y = ypos;
                float start_vec[3];
                start_vec[0] = start.x;
                start_vec[1] = start.y;
                start_vec[2] = 0;

                if (ongoing_command != "FreeGeom")
                {
                    cout << "Start Point is (" << xpos << "," << ypos << ")" << endl;

                   
                    cout << "Mark end Point on viewport" << endl;

                    ongoing_command_fb = "line";

                }
                line_start = new Point(start_vec);




               

            }
            else
            {
                if (!(compare_float(line_start->x, xpos) && compare_float(line_start->y, ypos)))
                {
                    end.x = xpos;
                    end.y = ypos;
                    float end_vec[3];
                    end_vec[0] = end.x;
                    end_vec[1] = end.y;
                    end_vec[2] = 0;
                    if(ongoing_command != "FreeGeom")
                        cout << "Marked Point is (" << xpos << "," << ypos << ")" << endl;
                    if (ongoing_command != "FreeGeom")
                    {
                        if (ongoing_command == "Polyline")
                            cout << "Mark Points on Viewport OR Press Q button on viewport to end the current command" << endl;
                        else
                            cout << "Mark Start Point  on Viewport OR Press Q button on viewport to end the current command" << endl;


                    }
                    line_end = new Point(end_vec);

                    Line* l1 = new Line(line_start, line_end);

                   
                    create_Line_for_Opengl(l1);
                    if (ongoing_command == "line")
                    {
                        line_start = NULL;
                        line_end = NULL;
                        //ongoing_command = "";
                    }
                       
                    else
                    {
                        line_start = line_end;
                    }
                }
                

            }
        }

        if ( (ongoing_command == "circle") || (ongoing_command == "con_circle"))
        {
            Point cen;
            Point end;
            float rad;
              
                if (!cen_cir)
                {
                  
                    cen.x = xpos;
                    cen.y = ypos;
                    is_cen_taken = true;
                    float ver_cen[3];
                    ver_cen[0] = cen.x;
                    ver_cen[1] = cen.y;
                    ver_cen[2] = 0;
                    cout << "Center Point are (" << xpos  << "," << ypos << ")" << endl;

                    cen_cir = new Point(ver_cen);
                    cout << "Mark radial point viewport" << endl;
                    ongoing_command_fb = "circle";

                }
                else
                {
                  


                    if (!(compare_float(cen_cir->x, xpos) && compare_float(cen_cir->y, ypos)))
                    {
                        end.x = xpos;
                        end.y = ypos;
                        cir_rad = distance_two_points(*cen_cir, end);
                        cout << "Radius is " << cir_rad << endl;

                        Point* cen_cir2 = new Point(cen_cir->x, cen_cir->y, 0.0);

                        Circle* c1 = new Circle(cen_cir2, cir_rad);
                        create_Circle_for_Opengl( c1);

                        if (ongoing_command == "circle")
                            cen_cir = NULL;
                        if (ongoing_command == "circle")
                            cout << "Mark Center Point  on Viewport OR Press Q button on viewport to end the current command" << endl;
                        else
                            cout << "Mark radial Point  on Viewport OR Press Q button on viewport to end the current command" << endl;


                       }
                            
                    
                }
            
        }

        if (ongoing_command == "polygon")
        {
            double xpos, ypos;
            //getting cursor position

            glfwGetCursorPos(window, &xpos, &ypos);
            double xpos_actual, ypos_actual;
            xpos = -1 * (((SCR_WIDTH / 2) - xpos) / (SCR_WIDTH / 2));
            ypos = ((SCR_HEIGHT / 2) - ypos) / (SCR_HEIGHT / 2);
            if (!poly_cen)
            {
                cout << "Mark center Point on Viewport OR Press Q button on viewport to end the current command" << endl;

                poly_cen = new Point(xpos, ypos, 0.0);
                ongoing_command_fb = "polygon";


            }

            else
            {
               
                Point *end = new Point(xpos, ypos, 0.0);


                float rad = 0.0f;
                rad = distance_two_points(*poly_cen, *end);

                createPolygon(rad);
                poly_cen = NULL;


            }






        }


    }
        
}




// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



