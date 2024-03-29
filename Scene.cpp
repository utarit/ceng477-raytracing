#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "Image.h"
#include "tinyxml2.h"
#include <cmath>
#include <thread>

using namespace tinyxml2;

/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */
Vector3f Scene::pixelRenderer(const Ray &ray, const int recursionDepth) {

				float tmin = (unsigned long) -1;
				Shape *object = nullptr;
				ReturnVal val;
				for(auto &obj : pScene->objects)
				{
					auto val_tmp = obj->intersect(ray);
					if(val_tmp.intersectionStatus > 0 && val_tmp.t < tmin)
					{
						tmin = val_tmp.t;
						object = obj;
						val = val_tmp;
					}
				}

				if(object)
				{
					auto material = pScene->materials.at(object->matIndex-1);
					Vector3f shading = pScene->ambientLight *  material->ambientRef;
					for(auto &light: pScene->lights)
					{
						Vector3f light_vector = light->position - val.point;
						light_vector.normalize();
						Ray s = {val.point + light_vector*pScene->shadowRayEps, light_vector};
						float t_light = s.gett(light->position);
						bool contribute = true;
						for(auto &obj2 : pScene->objects)
						{
							ReturnVal shadow_val = obj2->intersect(s);
							if(shadow_val.intersectionStatus > 0 && shadow_val.t >= pScene->intTestEps && shadow_val.t < t_light)
							{
								
								contribute = false;
								break;
							}
						}
						if (contribute){
							Vector3f diffuse = material->diffuseRef;
							auto lightContr = light->computeLightContribution(val.point);
							auto cos_th = max(0.0f, light_vector.dotProduct(val.normalVector));
							Vector3f h = light_vector - ray.direction;
							h.normalize();
							Vector3f ks = material->specularRef;
							float cosa = pow(max(0.0f, val.normalVector.dotProduct(h)), material->phongExp);
							shading = shading + diffuse * cos_th * lightContr + ks * lightContr *cosa;
							// mirror
							
						}
					}
					
					if (material->mirrorRef != 0 && recursionDepth) {
						auto cosa = -2 * val.normalVector.dotProduct(ray.direction);
						auto w_r = val.normalVector * cosa + ray.direction;
						w_r.normalize();
						Ray mirrorRay = {val.point + w_r * shadowRayEps, w_r};
						auto mirrorShading =  pixelRenderer(mirrorRay, recursionDepth - 1);
						shading = shading + material->mirrorRef * mirrorShading;
					}
					return shading;
				} 
				else 
				{
					return backgroundColor;
				}
}
void Scene::partialRenderer(const PartialScene& partialScene) {
	for(int y = partialScene.y; y < partialScene.y + partialScene.height; y++)
		{
			for(int x = partialScene.x; x < partialScene.x + partialScene.width; x++)
			{
				Ray ray = partialScene.cam_ref->getPrimaryRay(y, x);
				
					auto shading = pScene->pixelRenderer(ray, pScene->maxRecursionDepth);

					partialScene.img_ref.setPixelValue(x, y, {
						(unsigned char)min(max(shading.x, 0.0f), 255.0f),
						(unsigned char)min(max(shading.y, 0.0f), 255.0f), 
						(unsigned char)min(max(shading.z, 0.0f), 255.0f)
						});
			}

		}
}

void Scene::renderScene(void)
{
	for(auto &cam: cameras)
	{
		Image image = {cam->imgPlane.nx, cam->imgPlane.ny}; 

		const int number_of_cores = std::thread::hardware_concurrency() * 2;
		const int heightStepSize = cam->imgPlane.ny / number_of_cores;
		auto threads = new thread[number_of_cores];

		for (int th_id = 0; th_id < number_of_cores - 1; ++th_id) {
			PartialScene partialScene = {
				cam->imgPlane.nx,
				heightStepSize,
				0,
				heightStepSize * th_id,
				cam,
				image
			};
			threads[th_id] = std::thread(partialRenderer, partialScene);
		}
		PartialScene partialScene = {
			cam->imgPlane.nx,
			cam->imgPlane.ny - heightStepSize * (number_of_cores - 1),
			0,
			heightStepSize * (number_of_cores - 1),
			cam,
			image
		};
		threads[number_of_cores - 1] = std::thread(partialRenderer, partialScene);

		for (int th_id = 0; th_id < number_of_cores; ++th_id) {
			threads[th_id].join();
		}
		image.saveImage(cam->imageName);
		delete[] threads;





	/*
		for(int y = 0; y < cam->imgPlane.ny; y++)
		{
			for(int x = 0; x < cam->imgPlane.nx; x++)
			{
				Ray ray = cam->getPrimaryRay(y, x);
				float tmin = (unsigned long) -1;
				Shape *object = nullptr;
				ReturnVal val;
				for(auto &obj : objects)
				{
					auto val_tmp = obj->intersect(ray);
					if(val_tmp.intersectionStatus > 0 && val_tmp.t < tmin)
					{
						tmin = val_tmp.t;
						object = obj;
						val = val_tmp;
					}
				}

				if(object)
				{
					auto material = materials.at(object->matIndex-1);
					Vector3f shading = ambientLight *  material->ambientRef;
					for(auto &light: lights)
					{
						Vector3f light_vector = light->position - val.point;
						light_vector = light_vector * (1/light_vector.length());
						Ray s = {val.point + light_vector*shadowRayEps, light_vector};
						float t_light = s.gett(light->position);
						bool contribute = true;
						for(auto &obj2 : objects)
						{
							ReturnVal shadow_val = obj2->intersect(s);
							if(shadow_val.intersectionStatus > 0 && shadow_val.t < t_light)
							{
								if (shadow_val.t < pScene->intTestEps ) {
									std::cout << "shadow val t si intesttepsden küçük " << shadow_val.t << ">" << pScene->intTestEps << std::endl;
									continue;
								}
								contribute = false;
								break;
							}
						}
						if (contribute){
							Vector3f diffuse = material->diffuseRef;
							auto lightContr = light->computeLightContribution(val.point);
							auto cos_th = max(0.0f, light_vector.dotProduct(val.normalVector));
							Vector3f h = light_vector - ray.direction;
							h = h * (1/h.length());
							Vector3f ks = material->specularRef;
							float cosa = pow(max(0.0f, val.normalVector.dotProduct(h)), material->phongExp);
							shading = shading + diffuse * cos_th * lightContr + (ks*lightContr) *cosa;
							// specular
							
						}
					}
					image.setPixelValue(x, y, {
						(unsigned char)min(max(shading.x, 0.0f), 255.0f),
						(unsigned char)min(max(shading.y, 0.0f), 255.0f), 
						(unsigned char)min(max(shading.z, 0.0f), 255.0f)
						});
				} 
				else 
				{
					image.setPixelValue(x, y, 
					{
						(unsigned char)min(max(backgroundColor.r, 0.0f), 255.0f),
						(unsigned char)min(max(backgroundColor.g, 0.0f), 255.0f), 
						(unsigned char)min(max(backgroundColor.b, 0.0f), 255.0f)
						});
				}
			}
		}
			*/

	}

}

// Parses XML file. 
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if(pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if(pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if(pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while(pCamera != nullptr)
	{
        int id;
        char imageName[64];
        Vector3f pos, gaze, up;
        ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");	
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while(pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if(materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
				else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if(materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while(str[cursor] != '\0')
	{
		for(int cnt = 0 ; cnt < 3 ; cnt++)
		{
			if(cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if(cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++; 
			while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");
	
	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int> *meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while(str[cursor] != '\0')
		{
			for(int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if(cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if(cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++; 
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while(pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

