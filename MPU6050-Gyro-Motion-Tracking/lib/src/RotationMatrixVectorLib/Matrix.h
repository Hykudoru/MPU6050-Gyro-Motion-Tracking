#include <RotationMatrixVectorLib/Vector.h>
#include <cmath>
#ifndef MATRIX_H
#define MATRIX_H
/* 
    This matrix library can be used for calculating rotation matrices used in 3-dimensional coordinate space.
   
    Euler conventions: Right-handed, Intrinsic.

    ===============[ Things to Know ]================

    Rotation matrices can be used to track the orientation of a mobile rigid-body relative to a globally fixed world axis frame of reference. 
    Combining a sequence of elemental rotations in any order to get the newly rotated axis frame.
    Intrinsic rotations rotate about the axes of the fixed coordinate system frame attached to the rigid body in motion. 

    ===============[ How To use this library ]================

    - Extracting colums and rows follow the pattern: Matrix[row][column]. 
    - Matrix multiplication A*B or Multiply(A, B) assumes A is the row matrix while B is the column matrix.
    - The *= operator can be used like so: A *= B, means A = A*B, where B is the column matrix.

    EXAMPLES:

    --------------------------------------------------------------------------------------------------
    Z AXIS ROTATION: 

        rotation = Matrix3x3::RotZ(PI/2.0);
    --------------------------------------------------------------------------------------------------
    EULER ROTATION:

        Matrix3x3 rotation = (Matrix3x3::RotZ(yaw) * Matrix3x3::RotY(pitch)) * Matrix3x3::RotX(roll);
    --------------------------------------------------------------------------------------------------
    YAW-PITCH-ROLL:

        rotation = YPR(180*PI/180, 45*PI/180, 90*PI/180);
    --------------------------------------------------------------------------------------------------
    Undo Previous rotation:
            
        1st. rotation *= YPR(180*PI/180, 45*PI/180, 90*PI/180);
        2nd. rotation *= RPY(-180*PI/180, -45*PI/180, -90*PI/180);
    --------------------------------------------------------------------------------------------------


*/

class Matrix3x3 
{
public: 
    float matrix[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    static float IDENTITY[3][3];
    static float ZERO[3][3];

    void Set(float matrix3x3[3][3])
    {
        for (size_t r = 0; r < 3; r++)
        {
            for (size_t c = 0; c < 3; c++)
            {
                this->matrix[r][c] = matrix3x3[r][c];
            }
        }
    }

    Matrix3x3() {}
    Matrix3x3(float matrix3x3[3][3])
    {
        this->Set(matrix3x3);
    }
    
    static Matrix3x3 Transposed(const float matrix[][3])
    //------Untested-------
    {
        Matrix3x3 matrixT = Matrix3x3();
        for (int r = 0; r < 3; r++)
        {
            for (int c = 0; c < 3; c++)
            {
                matrixT.matrix[0][c] = matrix[r][0];
                matrixT.matrix[1][c] = matrix[r][1];
                matrixT.matrix[2][c] = matrix[r][2];
            }
        }
        return matrixT;
    }

    Matrix3x3 Transpose()
    //------Untested-------
    {
        Matrix3x3 matrixT = Matrix3x3();
        for (int r = 0; r < 3; r++)
        {
            for (int c = 0; c < 3; c++)
            {
                matrixT.matrix[0][c] = this->matrix[r][0];
                matrixT.matrix[1][c] = this->matrix[r][1];
                matrixT.matrix[2][c] = this->matrix[r][2];
            }
        }

        this->Set(matrixT.matrix);

        return matrix;
    }

    void T()
    {
        this->Transpose();
    }

    // A*B
    static Matrix3x3 Multiply(const float matrixA[][3], const float matrixB[][3])
    {
        Matrix3x3 result = Matrix3x3();
        for (size_t r = 0; r < 3; r++)
        {
            Vector3<float> rowVec = Vector3<float>(matrixA[r][0], matrixA[r][1], matrixA[r][2]);
            for (size_t c = 0; c < 3; c++)
            {
                Vector3<float> columnVec;
                columnVec.x = matrixB[0][c];
                columnVec.y = matrixB[1][c];
                columnVec.z = matrixB[2][c];
                result.matrix[r][c] = DotProduct(Vector3<float>(rowVec), columnVec);
            }
        }

        return result;
    }

    Vector3<float> Multiply(float columnVec[])
    //------Untested-------
    {
        float resultVector[3];
        for (int r = 0; r < 3; r++)
        {
            Vector3<float> matrixRowVector = Vector3<float>(matrix[r][0], matrix[r][1], matrix[r][2]);
            resultVector[r] = DotProduct(Vector3<float>(matrixRowVector), (Vector3<float>)columnVec);
        }

        return resultVector;
    }

    // Same as matrixA = matrixA * matrixB
    Matrix3x3& operator*=(Matrix3x3 matrixB)
    {
        *this = Multiply(this->matrix, matrixB.matrix);
        return *this;
    }

    Matrix3x3& operator=(float matrix[3][3])
    {
        //*this = Matrix3x3(matrix);
        this->Set(matrix);
        return *this;
    }

    // Returns a Rotation Matrix about the X axis (in radians)
    // | 1,   0,      0     |
    // | 0, Cos(T), -Sin(T) |
    // | 0, Sin(T),  Cos(T) |
    static Matrix3x3 RotX(float theta) 
    {
        float Cos = cos(theta);
        float Sin = sin(theta);

        float standardRotX[3][3] = 
        {
            {1, 0, 0},     
            {0, Cos, -Sin},  
            {0, Sin, Cos}
        };

        Matrix3x3 matrix(standardRotX);
        return matrix;
    }

    // Returns a Rotation Matrix about the Y axis (in radians)
    // | Cos(T),  0,   Sin(T)  |
    // |   0,     1,     0     |
    // |-Sin(T),  0,   Cos(T)  |
    static Matrix3x3 RotY(float theta) 
    {
        float Cos = cos(theta);
        float Sin = sin(theta);

        float standardRotY[3][3] = 
        {
            {Cos, 0, Sin},     
            {0,   1,  0 },  
            {-Sin,0, Cos}
        };

        Matrix3x3 matrix(standardRotY);
        return matrix;
    }

    // Returns a Rotation Matrix about the Z axis (in radians)
    // | Cos(T), -Sin(T), 0 |
    // | Sin(T),  Cos(T), 0 |
    // |   0,      0,     1 |
    static Matrix3x3 RotZ(float theta) 
    {
        float Cos = cos(theta);
        float Sin = sin(theta);

        float standardRotZ[3][3] = 
        {
            {Cos, -Sin,  0},     
            {Sin,  Cos,  0},  
            {0,     0,   1}
        };

        Matrix3x3 matrix(standardRotZ);
        return matrix;
    }
};
 
// A*B
Matrix3x3 operator*(const Matrix3x3& matrixA, const Matrix3x3& matrixB);

// Roll-Pitch-Yaw x-y'-z''(intrinsic rotation) or z-y-x (extrinsic rotation)
Matrix3x3 RPY(float roll, float pitch, float yaw);

// Yaw-Pitch-Roll z-y'-x''(intrinsic rotation) or x-y-z (extrinsic rotation)
Matrix3x3 YPR(float roll, float pitch, float yaw);
#endif