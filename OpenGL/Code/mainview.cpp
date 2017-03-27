#include "mainview.h"
#include "math.h"
#include <QVector>
#include <QVector3D>

#include <QDateTime>
#include <QTimer>
#include <time.h>

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
    this->frameCounter = 0;
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
    glDeleteBuffers (1, &this->coors_cat);
    glDeleteBuffers (1, &this->coors_quad);
    glDeleteBuffers (1, &this->colours);
    glDeleteBuffers (1, &this->normal_buffer_cat);
    glDeleteBuffers (1, &this->normal_buffer_quad);
    glDeleteTextures (1, &this->texptr);
    glDeleteTextures (1, &this->texptr_2);
    glDeleteTextures (1, &this->texptr_3);
    glDeleteVertexArrays (1, &this->vao);
    // glDeleteFramebuffers(1, &this->frame_buffer);

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

    secondShaderProg = new QOpenGLShaderProgram ();
    secondShaderProg->addShaderFromSourceFile (QOpenGLShader::Vertex, "Some placeholder");
    secondShaderProg->addShaderFromSourceFile (QOpenGLShader::Fragment, "Some placeholder");
    secondShaderProg->link ();

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

    glGenFramebuffers(1, &this->frame_buffer);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer);

    glGenBuffers (1, &this->coors_cat);
    glGenBuffers (1, &this->coors_quad);
    glGenBuffers (1, &this->colours);
    glGenBuffers (1, &this->normal_buffer_cat);
    glGenBuffers (1, &this->texture_coordinates_cat);
    glGenTextures (1, &this->poesje_texture);
    glGenTextures (1, &this->texptr);
    glGenTextures (1, &this->texptr_2);
    glGenTextures (1, &this->texptr_3);

    // Bind coordinates.
    glBindBuffer (GL_ARRAY_BUFFER, this->coors_cat);
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer (GL_ARRAY_BUFFER, this->coors_quad);
    glEnableVertexAttribArray (6);
    glVertexAttribPointer (6, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray (0);
}

void MainView::loadCatModel(QString filename, GLuint bufferObject)
{

    this->cubeModel = new Model(filename);
    this->numTris_cat = cubeModel->getNumTriangles();

    Q_UNUSED(bufferObject);

    // Load coordinates.
    glBindBuffer (GL_ARRAY_BUFFER, this->coors_cat);
    glBufferData (GL_ARRAY_BUFFER,
        sizeof (GL_FLOAT) * 3 * cubeModel->getVertices ().size (),
        cubeModel->getVertices ().data (),
        GL_STATIC_DRAW
        );

    // Load Normals
    glBindBuffer (GL_ARRAY_BUFFER, this->normal_buffer_cat);
    glBufferData (GL_ARRAY_BUFFER,
                  sizeof (GL_FLOAT) * 3 * cubeModel->getNormals().size(),
                  cubeModel->getNormals().data(),
                  GL_STATIC_DRAW
                  );

    glBindBuffer (GL_ARRAY_BUFFER, this->texture_coordinates_cat);
    glBufferData (GL_ARRAY_BUFFER,
                    sizeof (GL_FLOAT) * 2 * cubeModel->getTextureCoords ().size (),
                    cubeModel->getTextureCoords ().data (),
                    GL_STATIC_DRAW
                    );

}

void MainView::loadQuadModel(QString filename, GLuint bufferObject)
{

    this->quadModel = new Model(filename);
    this->numTris_quad = quadModel->getNumTriangles();

    Q_UNUSED(bufferObject);

    // Load coordinates.
    glBindBuffer (GL_ARRAY_BUFFER, this->coors_quad);
    glBufferData (GL_ARRAY_BUFFER,
        sizeof (GL_FLOAT) * 3 * quadModel->getVertices ().size (),
        quadModel->getVertices ().data (),
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

    loadCatModel(":/models/cat.obj", cubeBO);
    loadQuadModel(":/models/flat_surface.obj", quadBO);

    loadTexture (":/textures/cat_diff.png", this->poesje_texture);

    //tex1
    glBindTexture (GL_TEXTURE_2D, this->texptr);
    glTexImage2D (GL_TEXTURE_2D,
        0,
        GL_RGB,
        width (),
        height (),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //tex2
    glBindTexture (GL_TEXTURE_2D, this->texptr_2);
    glTexImage2D (GL_TEXTURE_2D,
        0,
        GL_RGB,
        width (),
        height (),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //tex3
    glBindTexture (GL_TEXTURE_2D, this->texptr_3);
    glTexImage2D (GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT,
        width (),
        height (),
        0,
        GL_DEPTH_COMPONENT,
        GL_UNSIGNED_BYTE,
        NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        this->texptr,
        0
    );

    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT1,
        GL_TEXTURE_2D,
        this->texptr_2,
        0
    );

    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        this->texptr_3,
        0
    );

    GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    //loadTexture (":/textures/cat_diff.png", this->texptr);
    //loadTexture (":/textures/cat_norm.png", this->texptr_2);
    //loadTexture (":/textures/cat_spec.png", this->texptr_3);
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
    glGetIntegerv (GL_FRAMEBUFFER_BINDING, &original_frame_buffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);

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
    this->view.translate (QVector3D (0, 0, -5));
    this->projection.perspective (30, (float) width () / height (),1, 500);

    glUniformMatrix4fv (this->viewptr, 1, false, this->view.data ());
    glUniformMatrix4fv (this->modelptr, 1, false, this->model.data ());
    glUniformMatrix4fv (this->projectionptr, 1, false, this->projection.data ());
    glBindVertexArray (this->vao);

    glActiveTexture (GL_TEXTURE0);

    renderCat();

    mainShaderProg->release();
    glBindFramebuffer (GL_DRAW_FRAMEBUFFER, original_frame_buffer);
    secondShaderProg->bind ();

    // TODO: DRAW A QUAD! HERE.
    // Use buffered textures.

    secondShaderProg->release ();
    // Draw your textured quad, using the gBuffers
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

