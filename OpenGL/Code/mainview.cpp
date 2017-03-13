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

    this->rotation_x = 0;
    this->rotation_y = 0;
    this->rotation_z = 0;
    this->scale = 1;

    //set all planet rotations to 0
    for(int i = 0; i < 10; i++){
        this->planet_rotations[i] = QVector3D(1,1,1);
    }
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
    glDeleteTextures (1, &this->texptr);
    glDeleteTextures (1, &this->texptr_2);
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

    this->colourptr = glGetUniformLocation (mainShaderProg->programId(), "colour_object_in");
    this->lightcolourptr = glGetUniformLocation (mainShaderProg->programId (), "colour_light_in");
    this->materialptr = glGetUniformLocation (mainShaderProg->programId (), "material_in");
    this->lightptr = glGetUniformLocation (mainShaderProg->programId(), "position_light_in");

    this->texture_uniform_ptr = glGetUniformLocation (mainShaderProg->programId (), "sampler");

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
    glGenBuffers (1, &this->texture_coordinates);
    glGenTextures (1, &this->texptr);
    glGenTextures (1, &this->texptr_2);
    glGenTextures (1, &this->texptr_3);
    glGenTextures (1, &this->texptr_4);
    glGenTextures (1, &this->texptr_5);
    glGenTextures (1, &this->texptr_6);
    glGenTextures (1, &this->texptr_7);
    glGenTextures (1, &this->texptr_8);
    glGenTextures (1, &this->texptr_9);
    glGenTextures (1, &this->texptr_10);

    // Bind coordinates.
    glBindBuffer (GL_ARRAY_BUFFER, this->coors);
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind colours.
//    glBindBuffer (GL_ARRAY_BUFFER, this->colours);
//    glEnableVertexAttribArray (1);
//    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind normals.
    glBindBuffer (GL_ARRAY_BUFFER, this->normal_buffer);
    glEnableVertexAttribArray (2);
    glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind textures.
    glBindBuffer (GL_ARRAY_BUFFER, this->texture_coordinates);
    glEnableVertexAttribArray (3);
    glVertexAttribPointer (3, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray (0);
}

void MainView::loadModel(QString filename, GLuint bufferObject)
{

    this->cubeModel = new Model(filename);
    this->numTris = cubeModel->getNumTriangles();

    Q_UNUSED(bufferObject);

    // Load coordinates.
    glBindBuffer (GL_ARRAY_BUFFER, this->coors);
    glBufferData (GL_ARRAY_BUFFER,
        sizeof (GL_FLOAT) * 3 * cubeModel->getVertices ().size (),
        cubeModel->getVertices ().data (),
        GL_STATIC_DRAW
        );

    // Load Normals
    glBindBuffer (GL_ARRAY_BUFFER, this->normal_buffer);
    glBufferData (GL_ARRAY_BUFFER,
                  sizeof (GL_FLOAT) * 3 * cubeModel->getNormals().size(),
                  cubeModel->getNormals().data(),
                  GL_STATIC_DRAW
                  );

    glBindBuffer (GL_ARRAY_BUFFER, this->texture_coordinates);
    glBufferData (GL_ARRAY_BUFFER,
                    sizeof (GL_FLOAT) * 2 * cubeModel->getTextureCoords ().size (),
                    cubeModel->getTextureCoords ().data (),
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

    loadTexture (":/textures/sun.jpg", this->texptr);
    loadTexture (":/textures/mercury.jpg", this->texptr_2);
    loadTexture (":/textures/venus.jpg", this->texptr_3);
    loadTexture (":/textures/earth.png", this->texptr_4);
    loadTexture (":/textures/mars.jpg", this->texptr_5);
    loadTexture (":/textures/jupiter.png", this->texptr_6);
    loadTexture (":/textures/saturn.jpg", this->texptr_7);
    loadTexture (":/textures/uranus.jpg", this->texptr_8);
    loadTexture (":/textures/neptune.jpg", this->texptr_9);
    loadTexture (":/textures/pluto.jpg", this->texptr_10);
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

    qDebug () << ":: View matrix: " << this->view;

    determineRotationMatrix ();

    qDebug () << ":: View matrix: " << this->view;

    this->model.translate(QVector3D(0,0,0));
    this->view.translate (QVector3D (0, 0, -2000));
    this->projection.perspective (30, (float) width () / height (),1, 4000);

    glUniformMatrix4fv (this->viewptr, 1, false, this->view.data ());
    glUniformMatrix4fv (this->modelptr, 1, false, this->model.data ());
    glUniformMatrix4fv (this->projectionptr, 1, false, this->projection.data ());
    glBindVertexArray (this->vao);

    glActiveTexture (GL_TEXTURE0);

    renderAnimation();

    mainShaderProg->release();
}

// Add your function implementations below

void MainView::determineRotationMatrix ()
{
    this->model.rotate (this->rotation_x, 1, 0, 0);
    this->model.rotate (this->rotation_y, 0, 1, 0);
    this->model.rotate (this->rotation_z, 0, 0, 1);
    this->model.scale (this->scale);
}

void MainView::loadTexture (QString file, GLuint texptr)
{
    glBindTexture (GL_TEXTURE_2D, texptr);
    QImage t = QImage (file);
    QVector<quint8> v = imageToBytes (t);

    glTexImage2D (GL_TEXTURE_2D, 
        0, 
        GL_RGBA8, 
        t.width (), 
        t.height (), 
        0, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        v.data ()
        );

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

