// Filename: angularForce.cxx
// Created by:  charles (08Aug00)
// 
////////////////////////////////////////////////////////////////////

#include "angularForce.h"

TypeHandle AngularForce::_type_handle;

////////////////////////////////////////////////////////////////////
//    Function : AngularForce
//      Access : protected
// Description : constructor
////////////////////////////////////////////////////////////////////
AngularForce::
AngularForce(void) :
  BaseForce() {
}

////////////////////////////////////////////////////////////////////
//    Function : AngularForce
//      Access : protected
// Description : copy constructor
////////////////////////////////////////////////////////////////////
AngularForce::
AngularForce(const AngularForce &copy) :
  BaseForce(copy) {
}

////////////////////////////////////////////////////////////////////
//    Function : ~AngularForce
//      Access : public, virtual
// Description : destructor
////////////////////////////////////////////////////////////////////
AngularForce::
~AngularForce(void) {
}

////////////////////////////////////////////////////////////////////
//    Function : get_vector
//      Access : public
// Description : access query
////////////////////////////////////////////////////////////////////
LVector3f AngularForce::
get_vector(const PhysicsObject *po) {
  LVector3f v = get_child_vector(po);
  return v;
}

////////////////////////////////////////////////////////////////////
//    Function : is_linear 
//      Access : public
// Description : access query
////////////////////////////////////////////////////////////////////
bool AngularForce::
is_linear(void) const {
  return false;
}
