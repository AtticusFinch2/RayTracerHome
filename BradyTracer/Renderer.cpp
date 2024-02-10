#include "Renderer.h"
#include "Walnut/Random.h"




void Renderer::OnResize(uint32_t view_Width, uint32_t view_Height)
{
	if (m_FinalImage)
	{
		m_FinalImage->Resize(view_Width, view_Height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(view_Width, view_Height, Walnut::ImageFormat::RGBA);
	}
	delete[] m_ImageData; // make sure we don't obliterate our VRAM every frame
	aspect_ratio = (float)view_Width / view_Height;
	m_ImageData = new uint32_t[view_Height * view_Width];

}


void Renderer::Render()
{
	

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) { // THIS IS THE LOOP

			glm::vec2 coord = { x / (float)m_FinalImage->GetWidth(), y / (float)m_FinalImage->GetHeight() }; 
			coord = coord * 2.0f - 1.0f;
			// (0,0)-(w,h) -> (-1,-1)-(1,1)
			coord.x *= aspect_ratio;
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord) // param is already in 0->1 range
//THIS IS A FRAGMENT SHADER LIKE SHADERTOY
{
	


	glm::vec3 rayDir(coord.x, coord.y, -1.0f);
	rayDir = glm::normalize(rayDir);
	glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
	glm::vec3 spherePos(0.0f, 0.0f, -2.0f);
	float radius = 1.0f; // we are drawing a unit sphere

	//set (x-i)^2 + (y-j)^2 +(z-k)^2 -r^2 =0 
	// for a sphere with center (i,j,k)<spherePos> and radius r.
	//substitite Q<cameraPos> + D<rayDir> * T(scalar) = (x,y,z) in and then solve for T.
	// this is a large and annoying polynomial so im not going to expand here, 
	// but after doing it on paper you get a quadratic with the following a, b, and c as coefficients:

	float a = glm::dot(rayDir, rayDir);
	float b = 2.0f * (glm::dot(cameraPos, rayDir) - glm::dot(spherePos, rayDir));
	float c = glm::dot(cameraPos, cameraPos) + glm::dot(spherePos, spherePos) - radius * radius;
	//now we can just use the quadratic formula to get if our ray hits the sphere (and where it does)

	//quadform discriminant
	// b^2 - 4ac
	float discriminant = b * b - 4.0f * a * c;
	if (discriminant >= 0.0f) { // we have hit the sphere with a tangent or a slice (1 or more solutions)
		return 0xffff00ff;
	}
	//uint8_t red = (uint8_t)((coord.x + 1.0f) / 2.0f * 255.0f);
	//uint8_t green = (uint8_t)((coord.y + 1.0f) / 2.0f * 255.0f);
	return 0xff000000;// | (green << 8) | red;






}