#include "color.h"
#include "vector.h"
#include "matarials.h"

Matarial::Matarial(float absorb,
				   float through,
				   float emission,
				   float reflectDiv,
				   float throughDiv,
				   int emissionType,
				   const Color &ambient,
				   const Color &diffuse,
				   const Color &specular) : absorb(absorb),
											through(through),
											emission(emission),
											reflectDiv(reflectDiv),
											throughDiv(throughDiv),
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
	return ambient;
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

float Matarial::getEmissionFactor(float distance){
	if (emissionType == 2) return sqr(distance);
	else if (emissionType == 1) return distance;
	else if (emissionType == 0) return 1.0f;
	else return 1.0f;
}