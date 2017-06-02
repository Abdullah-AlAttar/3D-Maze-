#include "Renderer.h"

Renderer::Renderer()
{
	
}

void Renderer::Prepare(mat4 viewMatrix, mat4 projectionMatrix)
{
	glClearColor(0.8, 0.1,0.1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->viewMatrix = viewMatrix;
	this->projectionMatrix = projectionMatrix;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	/*glFrontFace(GL_CW);

	;*/
}

void Renderer::Render(RawModel rawModel)
{
	glBindVertexArray(rawModel.GetVAO());
	glEnableVertexAttribArray(0);

	glDrawElements(GL_TRIANGLES, rawModel.GetVertexCount(), GL_UNSIGNED_INT,0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
;
void Renderer::Render(TexturedModel texturedModel)
{
	RawModel rawModel = texturedModel.GetRawModel();
	glBindVertexArray(rawModel.GetVAO());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturedModel.GetModelTexture().GetTextureID());
	glDrawElements(GL_TRIANGLES, rawModel.GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	
	glBindVertexArray(0);
}
void Renderer::Render(GameObject &gameObject, Shader &shader)
{

	TexturedModel texturedModel = gameObject.GetMesh();
	RawModel rawModel = texturedModel.GetRawModel();
	Texture texture = texturedModel.GetModelTexture();
	glBindVertexArray(rawModel.GetVAO());
	glEnableVertexAttribArray(shader["vVertex"]);
	glEnableVertexAttribArray(shader["vTexCoords"]);
	glEnableVertexAttribArray(shader["vNormal"]);
	mat4 modelMatrix=
		glm::translate(gameObject.transform.position)*
		glm::rotate(glm::radians(gameObject.transform.rotation.x), VECTOR_RIGHT) *
		glm::rotate(glm::radians(gameObject.transform.rotation.y), VECTOR_UP) *
		glm::rotate(glm::radians(gameObject.transform.rotation.z), VECTOR_FORWARD)*
		glm::scale(gameObject.transform.scale);
	mat4 MVP =this->projectionMatrix*viewMatrix*modelMatrix;

	texture.SetReflectivity(2);
	texture.SetShineDamper(16);
	//cout << texture.GetShineDamper() << " " << texture.GetReflectivity() << endl;
	shader.SetUniform(shader("MVP"), MVP);
	shader.SetUniform(shader("modelMatrix"), modelMatrix);
	shader.SetUniform(shader("viewMatrix"), viewMatrix);
	shader.SetUniform(shader("shineDamper"), texture.GetShineDamper());
	shader.SetUniform(shader("reflectivity"), texture.GetReflectivity());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturedModel.GetModelTexture().GetTextureID());
	glDrawElements(GL_TRIANGLES, rawModel.GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(shader["vVertex"]);
	glDisableVertexAttribArray(shader["vTexCoords"]);
	glDisableVertexAttribArray(shader["vNormal"]);


	glBindVertexArray(0);
}

void Renderer::Render(List<GameObject*> gameObjects, Shader & shader)
{
	for (GameObject* gameObject : gameObjects)
	{
		TexturedModel texturedModel = gameObject->GetMesh();
		RawModel rawModel = texturedModel.GetRawModel();
		Texture texture = texturedModel.GetModelTexture();
		glBindVertexArray(rawModel.GetVAO());
		glEnableVertexAttribArray(shader["vPosition"]);
		glEnableVertexAttribArray(shader["vTexCoords"]);
		glEnableVertexAttribArray(shader["vNormal"]);
		mat4 modelMatrix =
			glm::translate(gameObject->transform.position)*
			glm::rotate(glm::radians(gameObject->transform.rotation.x), VECTOR_RIGHT) *
			glm::rotate(glm::radians(gameObject->transform.rotation.y), VECTOR_UP) *
			glm::rotate(glm::radians(gameObject->transform.rotation.z), VECTOR_FORWARD)*
			glm::scale(gameObject->transform.scale);
		mat4 MVP = this->projectionMatrix*viewMatrix*modelMatrix;
		shader.SetUniform(shader("MVP"), MVP);
		shader.SetUniform(shader("modelMatrix"), modelMatrix);
		shader.SetUniform(shader("viewMatrix"), viewMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturedModel.GetModelTexture().GetTextureID());
		glDrawElements(GL_TRIANGLES, rawModel.GetVertexCount(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(shader["vPosition"]);
		glDisableVertexAttribArray(shader["vTexCoords"]);
		glDisableVertexAttribArray(shader["vNormal"]);
		glBindVertexArray(0);
	}
}
void Renderer::Render(map<int, List<GameObject*>> gameObjects, Shader & shader)
{
	for (auto entity : gameObjects)
	{
		for (auto gameOjbect : gameObjects[entity.first])
		{
			cout << gameOjbect->name << endl;
		}
	}
}

void Renderer::BindGameObjectData(GameObject *gameObject, Shader & shader)
{
	TexturedModel texturedModel = gameObject->GetMesh();
	RawModel rawModel = texturedModel.GetRawModel();
	Texture texture = texturedModel.GetModelTexture();
	glBindVertexArray(rawModel.GetVAO());
	glEnableVertexAttribArray(shader["vPosition"]);
	glEnableVertexAttribArray(shader["vTexCoords"]);
	glEnableVertexAttribArray(shader["vNormal"]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturedModel.GetModelTexture().GetTextureID());
}

void Renderer::UnbindCurrentGameObjectData(Shader &shader)
{
	glDisableVertexAttribArray(shader["vPosition"]);
	glDisableVertexAttribArray(shader["vTexCoords"]);
	glDisableVertexAttribArray(shader["vNormal"]);
	glBindVertexArray(0);
}

void Renderer::ApplyTransformationAndDraw(GameObject * gameObject, Shader & shader)
{
	mat4 modelMatrix =
		glm::translate(gameObject->transform.position)*
		glm::rotate(glm::radians(gameObject->transform.rotation.x), VECTOR_RIGHT) *
		glm::rotate(glm::radians(gameObject->transform.rotation.y), VECTOR_UP) *
		glm::rotate(glm::radians(gameObject->transform.rotation.z), VECTOR_FORWARD)*
		glm::scale(gameObject->transform.scale);
	mat4 MVP = this->projectionMatrix*viewMatrix*modelMatrix;
	shader.SetUniform(shader("MVP"), MVP);
	shader.SetUniform(shader("modelMatrix"), modelMatrix);
	shader.SetUniform(shader("viewMatrix"), viewMatrix);
	glDrawElements(GL_TRIANGLES, gameObject->GetMesh().GetRawModel().GetVertexCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::End()
{
	delete this;
}
