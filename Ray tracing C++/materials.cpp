#include "globals.h"
#include "color.h"
#include "vector.h"
#include "materials.h"

Material::Material(float absorb,
                   float through,
                   float emission,
                   float reflectDiv,
                   float throughDiv,
                   float grain,
                   int emissionType,
                   const Color &ambient,
                   const Color &diffuse,
                   const Color &specular) :
	absorb(absorb),
	through(through),
	emission(emission),
	reflectDiv(reflectDiv),
	throughDiv(throughDiv),
	grain(grain),
	emissionType(emissionType),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular)
{
}

Material::~Material()
{
}

float Material::getAbsorb()
{
	return this->absorb;
}

void Material::setAbsorb(float absorb)
{
	this->absorb = absorb;
}

float Material::getThrough()
{
	return this->through;
}

void Material::setThrough(float through)
{
	this->through = through;
}

float Material::getEmission()
{
	return this->emission;
}

void Material::setEmission(float emission)
{
	this->emission = emission;
}

float Material::getReflectDiv()
{
	return this->reflectDiv;
}

void Material::setReflectDiv(float reflectDiv)
{
	this->reflectDiv = reflectDiv;
}

float Material::getThroughDiv()
{
	return this->throughDiv;
}

void Material::setThroughDiv(float throughDiv)
{
	this->throughDiv = throughDiv;
}

float Material::getRIndex()
{
	return this->rIndex;
}

void Material::setRIndex(float rIndex)
{
	this->rIndex = rIndex;
}

Color Material::getAmbient()
{
	Color c = (grain * (((2.0 * rand()* RAND_MAX_INV) - 1.0))) + ambient;
	c.clamp();
	return c;
}

void Material::setAmbient(Color &ambient)
{
	this->ambient = ambient;
}

Color Material::getDiffuse()
{
	return diffuse;
}

void Material::setDiffuse(Color &diffuse)
{
	this->diffuse = diffuse;
}

Color Material::getSpecular()
{
	return specular;
}

void Material::setSpecular(Color &specular)
{
	this->specular = specular;
}

int Material::getEmissionType() {
	return emissionType;
}

void Material::setEmissionType(int emissionType) {
	this->emissionType = emissionType;
}

void Material::setReflection(bool val)
{
	this->reflection = val;
}
bool Material::getReflection()
{
	return this->reflection;
}

void Material::setLight(bool val)
{
	this->light = val;
}
bool Material::getLight()
{
	return light;
}

void Material::setRefraction(bool val)
{
	this->refraction = val;
}
bool Material::getRefraction()
{
	return refraction;
}

void Material::setGrain(float val)
{
	this->grain = val;
}
float Material::getGrain()
{
	return grain;
}

void Material::setAngularHueFactor(float val)
{
	this->angularHueFactor = val;
}
float Material::getAngularHueFactor()
{
	return angularHueFactor;
}

void Material::setAmbienceShadow(bool val)
{
	this->ambienceShadow = val;
}
bool Material::getAmbienceShadow()
{
	return ambienceShadow;
}

float Material::getEmissionFactor(float distance2) {
	if (emissionType == 2) return distance2;
	else if (emissionType == 1) return sqrt(distance2);
	else if (emissionType == 0) return 100;
	else return 1.0;
}