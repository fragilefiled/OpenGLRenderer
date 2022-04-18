#include"Camera.h"
Camera::Camera()
{
}
glm::mat4 Camera::SetPos(glm::vec3 pos)
{
	this->pos = pos;
	view = glm::lookAt(pos,
		pos+frontDir,
		upDir);
	return view;

}

glm::vec3 Camera::GetPos()
{
	return pos;
}

glm::mat4 Camera::SetFrontDir(glm::vec3 frontDir)
{
	this->frontDir = frontDir;
	view = glm::lookAt(pos,
		pos+frontDir,
		upDir);
	
	return view;

}

glm::vec3 Camera::GetFrontDir()
{
	return frontDir;
}

glm::mat4 Camera::GetView()
{
	return view;
}

glm::mat4 Camera::GetProjection()
{
	if(isPerspective)
	projection = glm::perspective(glm::radians(GetFov()), (float)width / (float)height, 0.1f, 100.0f);
	else
	{
		float temp = 15.0f;
		projection = glm::ortho(-temp, temp, -temp, temp, 0.0f, 2*temp);;
	}
	return projection;
}
glm::vec3 & Camera::getVoxelMinPoint(glm::vec3 cameraPos,int voxelSize) {

	glm::vec3 aabb = glm::vec3(0, 0, 0);
	float halfSize = voxelSize / 2.0f;
	aabb.x = cameraPos.x -halfSize ;
	aabb.y= cameraPos.y - halfSize;
	aabb.z = cameraPos.z - halfSize+0.0f;
	return aabb;
}

glm::vec3& Camera::getVoxelMaxPoint(glm::vec3 cameraPos, int voxelSize) {

	glm::vec3 aabb = glm::vec3(0, 0, 0);
	float halfSize = voxelSize / 2.0f;
	aabb.x = cameraPos.x + halfSize;
	aabb.y = cameraPos.y + halfSize;
	aabb.z = cameraPos.z + halfSize + 0.0f;
	return aabb;
}

std::vector<glm::mat4> Camera::GetVoxelViewProjection(glm::vec3 cameraPos, int voxelSize)
{
	std::vector<glm::mat4> ViewProjectionArray = std::vector<glm::mat4>(3);
   glm::mat4 voxel_projection = glm::ortho(-voxelSize/2.0f, voxelSize / 2.0f, -voxelSize / 2.0f, voxelSize / 2.0f,0.0f,0.0f+voxelSize);
   ViewProjectionArray[0] = glm::lookAt(cameraPos + glm::vec3(voxelSize / 2.0, 0, 0), cameraPos, glm::vec3(0, 1, 0));
   ViewProjectionArray[1] = glm::lookAt(cameraPos + glm::vec3(0, voxelSize / 2.0, 0), cameraPos, glm::vec3(0, 0, -1));
   ViewProjectionArray[2] = glm::lookAt(cameraPos + glm::vec3(0, 0, voxelSize / 2.0), cameraPos, glm::vec3(0, 1, 0));
	for (int i = 0; i < ViewProjectionArray.size(); i++)
		ViewProjectionArray[i] = voxel_projection * ViewProjectionArray[i];
	return ViewProjectionArray;
}
std::vector<glm::mat4> Camera::GetVoxelViewProjectionInverse(glm::vec3 cameraPos, int voxelSize)
{
	std::vector<glm::mat4> ViewProjectionInverseArray = std::vector<glm::mat4>(3);
	glm::mat4 voxel_projection = glm::ortho(-voxelSize / 2.0f, voxelSize / 2.0f, -voxelSize / 2.0f, voxelSize / 2.0f, 0.0f, 0.0f + voxelSize );
	ViewProjectionInverseArray[0] = glm::lookAt(cameraPos + glm::vec3(voxelSize / 2.0, 0, 0), cameraPos, glm::vec3(0, 1, 0));
	ViewProjectionInverseArray[1] = glm::lookAt(cameraPos + glm::vec3(0, voxelSize / 2.0, 0), cameraPos , glm::vec3(0, 0, -1));
	ViewProjectionInverseArray[2] = glm::lookAt(cameraPos + glm::vec3(0, 0, voxelSize / 2.0), cameraPos , glm::vec3(0, 1, 0));
	for (int i = 0; i < ViewProjectionInverseArray.size(); i++) {
		ViewProjectionInverseArray[i] = voxel_projection * ViewProjectionInverseArray[i];
		ViewProjectionInverseArray[i] = glm::inverse(ViewProjectionInverseArray[i]);
	}
	
	return ViewProjectionInverseArray;
}

void Camera::GenerateFrustum(glm::mat4 viewProjection)
{
	glm::vec4 rows[4];
	for (int i = 0; i < 4; i++)
		rows[i] = glm::vec4(viewProjection[0][i], viewProjection[1][i], viewProjection[2][i], viewProjection[3][i]);
	FrustumPlane[0] = rows[3] + rows[0];//Left
	FrustumPlane[1] = rows[3] - rows[0];//Right
	FrustumPlane[2] = rows[3] - rows[1];//Top
	FrustumPlane[3] = rows[3] + rows[1];//Bottom
	FrustumPlane[4] = rows[3] + rows[2];//Near
	FrustumPlane[5] = rows[3] - rows[2];//Far

	for (int i = 0; i < 6; i++)
		FrustumPlane[i] /=glm::length(FrustumPlane[i]);
}

void Camera::Move(Direction direction,float deltaTime,float speed)
{
	switch (direction)
	{
	case FORWARD:
		pos += cameraSpeed * frontDir * deltaTime*speed;

		SetPos(pos);
			break;
	case BACK:
		pos -= cameraSpeed * frontDir * deltaTime * speed;
		SetPos(pos);
		break;
	case LEFT:
		pos -= cameraSpeed * glm::normalize(glm::cross(frontDir, upDir)) * deltaTime * speed;

		SetPos(pos);
		break;
	case RIGHT:
		pos += cameraSpeed * glm::normalize(glm::cross(frontDir, upDir)) * deltaTime * speed;

		SetPos(pos);
		break;
	default:
		break;
	}
}

float Camera::GetFov()
{
 return	fov;
}

void Camera::SetFov(float fov)
{
	this->fov = fov;

}

glm::mat4 Camera::CaculateReflectMatrix(glm::vec3 normal, glm::vec3 positionOnPlane)
{
	float d = -glm::dot(normal, positionOnPlane);
	glm::mat4 reflectM = glm::mat4();
	
	reflectM[0][0] = 1 - 2 * normal.x * normal.x;
	reflectM[0][1] = -2 * normal.x * normal.y;
	reflectM[0][2] = -2 * normal.x * normal.z;
	reflectM[0][3] = -2 * d * normal.x;

	reflectM[1][0] = -2 * normal.x * normal.y;
	reflectM[1][1] = 1 - 2 * normal.y * normal.y;
	reflectM[1][2] = -2 * normal.y * normal.z;
	reflectM[1][3] = -2 * d * normal.y;

	reflectM[2][0] = -2 * normal.x * normal.z;
	reflectM[2][1] = -2 * normal.y * normal.z;
	reflectM[2][2] = 1 - 2 * normal.z * normal.z;
	reflectM[2][3] = -2 * d * normal.z;

	reflectM[3][0] = 0;
	reflectM[3][1] = 0;
	reflectM[3][2] = 0;
	reflectM[3][3] = 1;
	return reflectM;
}

