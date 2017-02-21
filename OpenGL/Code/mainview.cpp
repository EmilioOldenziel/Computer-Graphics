#include "mainview.h"
#include "math.h"
#include <QVector>
#include <QVector3D>
#include <random>

#include <QDateTime>

/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));

    this->model = QMatrix4x4 ();
    this->view = QMatrix4x4 ();
    this->projection = QMatrix4x4 ();
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    delete cubeModel;

    // Free Buffer Objects before Vertex Arrays
    glDeleteBuffers (1, &this->coors);
    glDeleteBuffers (1, &this->colours);
    glDeleteBuffers (1, &this->normal_buffer);
    glDeleteVertexArrays (1, &this->vao);

    // Free the main shader
    delete mainShaderProg;

    debugLogger->stopLogging();

    qDebug() << "MainView destructor";
}

/**
 * @brief MainView::createShaderPrograms
 *
 * Creates the shaderprogram for OpenGL rendering
 */
void MainView::createShaderPrograms() {
    // Qt wrapper (way cleaner than using pure OpenGL)
    mainShaderProg = new QOpenGLShaderProgram();
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");
    mainShaderProg->link();

    /* Add your other shaders below */
    this->viewptr = glGetUniformLocation (mainShaderProg->programId(), "view");
    this->modelptr = glGetUniformLocation (mainShaderProg->programId(), "model");
    this->projectionptr = glGetUniformLocation (mainShaderProg->programId(), "projection");
    this->normal_matrixptr = glGetUniformLocation (mainShaderProg->programId(), "normal_matrix");

    this->colourptr = glGetUniformLocation (mainShaderProg->programId(), "colour");
    this->illuminationptr = glGetUniformLocation (mainShaderProg->programId(), "illumination");

    /* End of custom shaders */

    // Store the locations (pointers in gpu memory) of uniforms in Glint's

}

/**
 * @brief MainView::createBuffers
 *
 * Creates necessary buffers for your application
 */
void MainView::createBuffers() {
    // TODO: implement buffer creation
    glGenVertexArrays (1, &this->vao);
    glBindVertexArray (this->vao);

    glGenBuffers (1, &this->coors);
    glGenBuffers (1, &this->colours);
    glGenBuffers (1, &this->normal_buffer);

    // Bind coordinates.
    glBindBuffer (GL_ARRAY_BUFFER, this->coors);
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind colours.
    glBindBuffer (GL_ARRAY_BUFFER, this->colours);
    glEnableVertexAttribArray (1);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind normals.
    glBindBuffer (GL_ARRAY_BUFFER, this->normal_buffer);
    glEnableVertexAttribArray (2);
    glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray (0);
}

void MainView::loadModel(QString filename, GLuint bufferObject) 
{

    this->cubeModel = new Model(filename);
    this->numTris = cubeModel->getNumTriangles();

    Q_UNUSED(bufferObject);

    // add colors
//    QVector <QVector3D> colours = QVector <QVector3D> ();

//    srand (1);
//    for (int i = 0; i < cubeModel->getVertices ().size (); i++)
//    {
//        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//        float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//        float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//        colours.append (QVector3D (r, g, b));
//    }

    // Load coordinates.
    glBindBuffer (GL_ARRAY_BUFFER, this->coors);
    glBufferData (GL_ARRAY_BUFFER, 
        sizeof (GL_FLOAT) * 3 * cubeModel->getVertices ().size (), 
        cubeModel->getVertices ().data (),
        GL_STATIC_DRAW
        );

    // Load colours.
//    glBindBuffer (GL_ARRAY_BUFFER, this->colours);
//    glBufferData (GL_ARRAY_BUFFER,
//        sizeof (GL_FLOAT) * 3 * colours.size (),
//        colours.data (),
//        GL_STATIC_DRAW
//        );

    // Load Normals
    glBindBuffer (GL_ARRAY_BUFFER, this->normal_buffer);
    glBufferData (GL_ARRAY_BUFFER,
                  sizeof (GL_FLOAT) * 3 * cubeModel->getNormals().size(),
                  cubeModel->getNormals().data(),
                  GL_STATIC_DRAW
                  );

}

void MainView::updateBuffers() {
    // Change the data inside buffers (if you want)
    // make sure to change GL_STATIC_DRAW to GL_DYNAMIC_DRAW
    // in the call to glBufferData for better performance

}


void MainView::updateUniforms() {
    // TODO: update the uniforms in the shaders using the glUniform<datatype> functions

}

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ), this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    /* TODO: call your initialization functions here */

    createShaderPrograms();

    createBuffers();

    loadModel(":/models/sphere.obj", cubeBO);

    // For animation, you can start your timer here

}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) {

    // TODO: Update projection to fit the new aspect ratio
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
}

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {

    // Clear the screen before rendering
    glClearColor(0.0f,0.0f,0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShaderProg->bind();

    this->model.setToIdentity ();
    this->view.setToIdentity ();
    this->projection.setToIdentity ();

//  this->view.translate (QVector3D (0, 0, -4));
    this->view.translate (QVector3D (-200, -200, -1000));

//  this->projection.perspective (60, (float) width () / height (), 1, 10);

    this->projection.perspective (30, (float) width () / height (),1, 2000);

    glUniformMatrix4fv (this->viewptr, 1, false, this->view.data ());
    glUniformMatrix4fv (this->modelptr, 1, false, this->model.data ());
    glUniformMatrix4fv (this->projectionptr, 1, false, this->projection.data ());
    glUniformMatrix4fv (this->normal_matrixptr, 1, false, this->normal_matrix.data ());

    glBindVertexArray (this->vao);

//  glDrawArrays (GL_TRIANGLES, 0, this->cubeModel->getVertices ().size ());
    renderRaytracerScene();

    mainShaderProg->release();
}

// Add your function implementations below

// TODO: add your code
