// #include "matarial.h"
// #include "color.h"

// Matarial::Matarial() :
// 	absorb(0.01f),
// 	through(0.0f),
// 	emission(100.0f),
// 	reflectDiv(PI/20),
// 	throughDiv(0.0f),
// 	ambient(0.35f),
// 	diffuse(0.35f),
// 	specular(1.0f)
// {}

// Matarial::Matarial(const Color& ambient) :
// 	absorb(0.2f),
// 	through(0.0f),
// 	emission(100.0f),
// 	reflectDiv(PI/100),
// 	throughDiv(0.0f),
// 	ambient(ambient),
// 	diffuse(ambient),
// 	specular(1.0f)
// {}

// Matarial::Matarial(float absorb, 
// 			 	   float through, 
// 			 	   float emission,
// 			 	   float reflectDiv, 
// 			 	   float throughDiv,
// 			 	   const Color& ambient,
// 			 	   const Color& diffuse,
// 			 	   const Color& specular) :
// 	absorb(absorb),
// 	through(through),
// 	emission(100.0f),
// 	reflectDiv(reflectDiv),
// 	throughDiv(throughDiv),
// 	ambient(ambient),
// 	diffuse(diffuse),
// 	specular(specular)
// {}

// Matarial::~Matarial()
// {}

// Matarial& Matarial::operator =(const Matarial& matarial)
// {
// 	absorb = matarial.absorb;
// 	through = matarial.through;
// 	emission = matarial.emission;
// 	reflectDiv = matarial.reflectDiv;
// 	throughDiv = matarial.throughDiv;
// 	ambient = matarial.ambient;
// 	diffuse = matarial.diffuse;
// 	specular = matarial.specular;

// 	return *this;
// }
