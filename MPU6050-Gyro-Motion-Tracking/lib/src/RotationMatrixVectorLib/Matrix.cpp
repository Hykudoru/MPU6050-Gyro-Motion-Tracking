#include <RotationMatrixVectorLib/Matrix.h>

float Matrix3x3::IDENTITY[3][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};
float Matrix3x3::ZERO[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

// A*B
Matrix3x3 operator*(const Matrix3x3& matrixA, const Matrix3x3& matrixB)
{
    Matrix3x3 dot = Matrix3x3::Multiply(matrixA.matrix, matrixB.matrix);
    return dot; 
}

// Roll-Pitch-Yaw x-y'-z''(intrinsic rotation) or z-y-x (extrinsic rotation)
Matrix3x3 RPY(float roll, float pitch, float yaw)
{
  //Matrix3x3 rotation = Matrix3x3::Multiply(Matrix3x3::Multiply(Matrix3x3::RotX(roll).matrix, Matrix3x3::RotY(pitch).matrix).matrix, Matrix3x3::RotZ(yaw).matrix);//Multiply(rotation.matrix, RotZ(PI/2.0).matrix);
  Matrix3x3 rotation = (Matrix3x3::RotX(roll) * Matrix3x3::RotY(pitch)) * Matrix3x3::RotZ(yaw);//Multiply(rotation.matrix, RotZ(PI/2.0).matrix);
  return rotation;
}

// Yaw-Pitch-Roll z-y'-x''(intrinsic rotation) or x-y-z (extrinsic rotation)
Matrix3x3 YPR(float roll, float pitch, float yaw)
{
  //Matrix3x3 rotation = Matrix3x3::Multiply(Matrix3x3::Multiply(Matrix3x3::RotZ(yaw).matrix, Matrix3x3::RotY(pitch).matrix).matrix, Matrix3x3::RotX(roll).matrix);//Multiply(rotation.matrix, RotZ(PI/2.0).matrix);
  Matrix3x3 rotation = (Matrix3x3::RotZ(yaw) * Matrix3x3::RotY(pitch)) * Matrix3x3::RotX(roll);//Multiply(rotation.matrix, RotZ(PI/2.0).matrix);
  
  return rotation;
}

// //To-Do
// void AngleAxisRotation(float axis[3], float angle)
// {
//     float mag = ((Vector3<float>)axis).Magnitude();
//     for (size_t i = 0; i < 3; i++)
//     {
//         axis[i] *= angle;
//     }
// }