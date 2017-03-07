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
    glDeleteBuffers (1, &this->texptr);
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
    glBindBuffer (GL_ARRAY_BUFFER, this->texptr);
    glEnableVertexAttribArray (3);
    glVertexAttribPointer (3, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

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

    loadModel(":/models/cube.obj", cubeBO);

    loadTexture (":/textures/rug_logo.png", texptr);

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

    this->view.translate (QVector3D (0, 0, -10));
    this->projection.perspective (30, (float) width () / height (),1, 2000);

    glUniformMatrix4fv (this->viewptr, 1, false, this->view.data ());
    glUniformMatrix4fv (this->modelptr, 1, false, this->model.data ());
    glUniformMatrix4fv (this->projectionptr, 1, false, this->projection.data ());
    glBindVertexArray (this->vao);

    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, this->texptr);

    renderRaytracerScene();

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
    QImage t = QImage (file);
    QVector<quint8> v = imageToBytes (t);

    glGenTextures(1, &texptr);
    glBindTexture (GL_TEXTURE_2D, texptr);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
}

