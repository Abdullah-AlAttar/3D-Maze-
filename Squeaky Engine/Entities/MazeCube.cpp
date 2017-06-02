#include "MazeCube.h"

MazeCube::MazeCube(string modelPath, string texturePath,Transform transform)
{
}



HitInfo MazeCube::IsColliding(BoxCollider other)
{
 	return boxCollider.OnCollision(other);
}

BoxCollider & MazeCube::GetBoxCollider()
{
	return boxCollider;
}
