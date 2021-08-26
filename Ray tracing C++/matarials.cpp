#include "globals.h"
#include "color.h"
#include "vector.h"
#include "matarials.h"

Matarial::Matarial(float absorb,
				   float through,
				   float emission,
				   float reflectDiv,
				   float throughDiv,
				   float grain,
				   int emissionType,
				   const Color &ambient,
				   const Color &diffuse,
				   const Color &specular) : absorb(absorb),
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

Matarial::~Matarial()
{
}

float Matarial::getAbsorb()
{
	return absorb;
}

void Matarial::setAbsorb(float absorb)
{
	this->absorb = absorb;
}

float Matarial::getThrough()
{
	return through;
}

void Matarial::setThrough(float through)
{
	this->through = through;
}

float Matarial::getEmission()
{
	return emission;
}

void Matarial::setEmission(float emission)
{
	this->emission = emission;
}

float Matarial::getReflectDiv()
{
	return reflectDiv;
}

void Matarial::setReflectDiv(float reflectDiv)
{
	this->reflectDiv = reflectDiv;
}

float Matarial::getThroughDiv()
{
	return throughDiv;
}

void Matarial::setThroughDiv(float throughDiv)
{
	this->throughDiv = throughDiv;
}

float Matarial::getRIndex()
{
	return rIndex;
}

void Matarial::setRIndex(float rIndex)
{
	this->rIndex = rIndex;
}

Color Matarial::getAmbient()
{
	Color c = (grain * (((2.0*rand()/RAND_MAX) - 1.0))) + ambient;
	c.clamp();
	return c;
}

void Matarial::setAmbient(Color &ambient)
{
	this->ambient = ambient;
}

Color Matarial::getDiffuse()
{
	return diffuse;
}

void Matarial::setDiffuse(Color &diffuse)
{
	this->diffuse = diffuse;
}

Color Matarial::getSpecular()
{
	return specular;
}

void Matarial::setSpecular(Color &specular)
{
	this->specular = specular;
}

int Matarial::getEmissionType() {
	return emissionType;
}

void Matarial::setEmissionType(int emissionType) {
	this->emissionType = emissionType;
}

void Matarial::setReflection(bool val)
{
	this->reflection = val;
}
bool Matarial::getReflection()
{
	return reflection;
}

void Matarial::setLight(bool val)
{
	this->light = val;
}
bool Matarial::getLight()
{
	return light;
}

void Matarial::setRefraction(bool val)
{
	this->refraction = val;
}
bool Matarial::getRefraction()
{
	return refraction;
}

void Matarial::setGrain(float val)
{
	this->grain = val;
}
float Matarial::getGrain()
{
	return grain;
}

void Matarial::setAngularHueFactor(float val)
{
	this->angularHueFactor = val;
}
float Matarial::getAngularHueFactor()
{
	return angularHueFactor;
}

void Matarial::setAmbienceShadow(bool val)
{
	this->ambienceShadow = val;
}
bool Matarial::getAmbienceShadow()
{
	return ambienceShadow;
}

float Matarial::getEmissionFactor(float distance2){
	if (emissionType == 2) return distance2;
	else if (emissionType == 1) return sqrt(distance2);
	else if (emissionType == 0) return 100;
	else return 1.0;
}