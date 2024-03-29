#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    MainView(QWidget *parent = 0);
    ~MainView();

    void updateRotation(int x, int y, int z);
    void updateModel(QString name);
    void updateShader(QString name);
    void updateScale(float scale);

    /* Add your public members below */

    /* End of public members */

    QVector3D convertHSLtoRGB(float H, float S, float L);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private:
    QOpenGLDebugLogger *debugLogger;

    void createShaderPrograms();
    void createBuffers();
    void updateBuffers();
    void updateUniforms();

    // Raytracer scene functions
    QVector3D rotate3D(QVector3D point, float angle);
    void renderObject(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos, GLint texptr_local, float scale_factor);
    //void renderAnimation();
    void renderCat();

    /* Add your private members below */
    void loadCatModel(QString filename, GLuint bufferObject);
    void loadQuadModel(QString filename, GLuint bufferObject);

    // Shader programs, GLint for uniforms/buffer objects, other variables
    QOpenGLShaderProgram *mainShaderProg;
    QOpenGLShaderProgram *secondShaderProg;

    QTimer timer; // timer used for animation

    Model *cubeModel;
    GLuint cubeBO;

    Model *quadModel;
    GLuint quadBO;

    unsigned numTris_cat;
    unsigned numTris_quad;

    GLuint vao;
    GLuint coors_cat;
    GLuint coors_quad;
    GLuint colours;
    GLuint normal_buffer_cat;
    GLuint normal_buffer_quad;

    QMatrix4x4 model;
    float planet_rotations[10];
    float planet_rotation_period[10] = {1.0, 6.0, 15.3, 24.9, 46.8, 295.36, 733.56, 2092.0, 4104.0, 6168.0};
    QMatrix4x4 view;
    QMatrix4x4 projection;

    QMatrix3x3 normal_matrix;

    QVector3D colour;
    QVector4D material;
    QVector3D light;

    GLint viewptr;
    GLint modelptr;
    GLint projectionptr;

    GLint normal_matrixptr;

    GLint colourptr;
    GLint lightcolourptr;
    GLint materialptr;
    GLint lightptr;

    // Rotation parameters requirements
    QMatrix4x4 model_reset;
    int rotation_x;
    int rotation_y;
    int rotation_z;
    double scale;

    int rotation_x_start;
    int rotation_y_start;

    void determineRotationMatrix ();

    // Texture stuff
    GLuint poesje_texture;
    GLuint texture_coordinates_cat;
    GLuint texture_coordinates_quad;
    GLint texture_uniform_ptr;

    GLuint texptr;
    GLuint texptr_2;
    GLuint texptr_3;

    GLint original_frame_buffer;
    GLuint frame_buffer;

    int frameCounter;

    void loadTexture (QString file, GLuint texptr);
    QVector<quint8> imageToBytes(QImage image);

    /* End of private members */

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H
