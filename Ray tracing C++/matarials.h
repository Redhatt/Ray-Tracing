#ifndef MATARIALS_H
#define MATARIALS_H

#include "globals.h"
#include "color.h"
#include "vector.h"

class Matarial
{
protected:
	float absorb, through, emission; 
	float reflectDiv, throughDiv, rIndex=1.09f;
	Color ambient, diffuse, specular;
	int emissionType;

	bool reflection = false;
	bool light = true;
	bool refraction = true;
	bool ambienceShadow = true;

	float grain = 0.0;
	float angularHueFactor = 0.5;

public:
	Matarial(float absorb = 0.028,
			 float through = 0.99,
			 float emission = 0.0,
			 float reflectDiv = 0 * PI/150,
			 float throughDiv = 0 * PI/150,
			 float grain=0.0,
			 int emissionType = 2,
			 const Color &ambient = Color(0.5),
			 const Color &diffuse = Color(0.5),
			 const Color &specular = Color(1.0));

	virtual ~Matarial();

	// getter and setters
	virtual float getAbsorb();
	virtual void setAbsorb(float absorb);

	virtual float getThrough();
	virtual void setThrough(float through);

	virtual float getEmission();
	virtual void setEmission(float emission);

	virtual float getReflectDiv();
	virtual void setReflectDiv(float reflectDiv);

	virtual float getThroughDiv();
	virtual void setThroughDiv(float throughDiv);

	virtual float getRIndex();
	virtual void setRIndex(float rIndex);

	virtual Color getAmbient();
	virtual void setAmbient(Color &ambient);

	virtual Color getDiffuse();
	virtual void setDiffuse(Color &diffuse);

	virtual Color getSpecular();
	virtual void setSpecular(Color &specular);

	virtual int getEmissionType();
	virtual void setEmissionType(int emissionType);

	virtual void setReflection(bool val);
	virtual bool getReflection();

	virtual void setLight(bool val);
	virtual bool getLight();

	virtual void setRefraction(bool val);
	virtual bool getRefraction();

	virtual void setGrain(float val);
	virtual float getGrain();

	virtual void setAngularHueFactor(float val);
	virtual float getAngularHueFactor();

	virtual void setAmbienceShadow(bool val);
	virtual bool getAmbienceShadow();

	virtual float getEmissionFactor(float distance);
	// virtual light BSDF();
};

#endif
