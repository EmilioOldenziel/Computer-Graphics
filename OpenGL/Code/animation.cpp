#include "mainview.h"

void MainView::renderCube(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos, GLint texptr_local, float scale_factor)
{
    // OpenGL assignment 1, part 2: create a function to render the sphere
    // Use Model(":/models/sphere.obj") for the model

    // you must remove these Q_UNUSED when you implement this function

    glBindTexture (GL_TEXTURE_2D, texptr_local);

    this->model.translate(pos);
    this->model.scale(scale_factor);
    this->colour = color;
    this->material = material;
    this->light = lightpos;
    this->normal_matrix = (this->view * this->model).normalMatrix();
    qDebug () << this->normal_matrix;
    glUniformMatrix4fv(this->modelptr, 1, false, this->model.data ());
    glUniform3fv(this->colourptr, 1, &this->colour[0]);
    glUniform3fv(this->lightptr, 1, &this->light[0]);
    glUniform4fv(this->materialptr, 1, &this->material[0]);
    glUniformMatrix3fv (this->normal_matrixptr, 1, false, this->normal_matrix.data ());

    glDrawArrays(GL_TRIANGLES, 0, this->cubeModel->getVertices ().size ());

    this->model.scale(1/scale_factor);
    this->model.translate(-pos);
    glUniformMatrix4fv (this->modelptr, 1, false, this->model.data ());

}

/**
 * Renders a similar scene used for the raytracer:
 * 5 colored spheres with a single light
 */
void MainView::renderAnimation()
{

    QVector3D lightpos = QVector3D(-200,600,1500);

    //re calculate lightposition for all spheres
    lightpos = (this->model * QVector4D(lightpos, 1.0)).toVector3D();

    //Sun
    renderCube(QVector3D(0,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos, this->texptr, 5);

    //Mecury
    renderCube(QVector3D(400,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos, this->texptr_2, 1);

    //Venus
    renderCube(QVector3D(600,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos, this->texptr_3, 1.3);

    //Earth
    renderCube(QVector3D(800,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos, this->texptr_4, 1.5);

    //Mars
    renderCube(QVector3D(1050,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos, this->texptr_5, 1);

    //Jupiter
    renderCube(QVector3D(1450,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos, this->texptr_6, 4);

    //Saturn
    renderCube(QVector3D(1850,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos, this->texptr_7, 2.5);

    //Uranus
    renderCube(QVector3D(2150,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos, this->texptr_8, 2);

    //Neptune
    renderCube(QVector3D(2450,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos, this->texptr_9, 2);

    //Pluto
    renderCube(QVector3D(2650,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos, this->texptr_10, 0.5);
}
