// Filename: p3dToplevelObject.cxx
// Created by:  drose (10Jul09)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#include "p3dToplevelObject.h"

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
P3DToplevelObject::
P3DToplevelObject() :
  _pyobj(NULL)
{
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::Destructor
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
P3DToplevelObject::
~P3DToplevelObject() {
  set_pyobj(NULL);

  // Just in case there are properties we haven't cleared yet.
  Properties::const_iterator pi;
  for (pi = _properties.begin(); pi != _properties.end(); ++pi) {
    P3D_object *value = (*pi).second;
    P3D_OBJECT_DECREF(value);
  }
  _properties.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::get_type
//       Access: Public, Virtual
//  Description: Returns the fundamental type of this kind of object.
////////////////////////////////////////////////////////////////////
P3D_object_type P3DToplevelObject::
get_type() {
  return P3D_OT_object;
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::get_bool
//       Access: Public, Virtual
//  Description: Returns the object value coerced to a boolean, if
//               possible.
////////////////////////////////////////////////////////////////////
bool P3DToplevelObject::
get_bool() {
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::get_int
//       Access: Public, Virtual
//  Description: Returns the object value coerced to an integer, if
//               possible.
////////////////////////////////////////////////////////////////////
int P3DToplevelObject::
get_int() {
  return 0;
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::get_float
//       Access: Public, Virtual
//  Description: Returns the object value coerced to a floating-point
//               value, if possible.
////////////////////////////////////////////////////////////////////
double P3DToplevelObject::
get_float() {
  return 0.0;
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::make_string
//       Access: Public, Virtual
//  Description: Fills the indicated C++ string object with the value
//               of this object coerced to a string.
////////////////////////////////////////////////////////////////////
void P3DToplevelObject::
make_string(string &value) {
  if (_pyobj == NULL) {
    value = "P3DToplevelObject";
  } else {
    int size = P3D_OBJECT_GET_STRING(_pyobj, NULL, 0);
    char *buffer = new char[size];
    P3D_OBJECT_GET_STRING(_pyobj, buffer, size);
    value = string(buffer, size);
    delete[] buffer;
  }    
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::get_property
//       Access: Public, Virtual
//  Description: Returns the named property element in the object.  The
//               return value is a new-reference P3D_object, or NULL
//               on error.
////////////////////////////////////////////////////////////////////
P3D_object *P3DToplevelObject::
get_property(const string &property) {
  if (_pyobj == NULL) {
    // Without a pyobj, we just report whatever's been stored locally.
    Properties::const_iterator pi;
    pi = _properties.find(property);
    if (pi != _properties.end()) {
      P3D_object *result = (*pi).second;
      P3D_OBJECT_INCREF(result);
      return result;
    }
    return NULL;
  }

  // With a pyobj, we pass the query down to it.
  return P3D_OBJECT_GET_PROPERTY(_pyobj, property.c_str());
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::set_property
//       Access: Public, Virtual
//  Description: Modifies (or deletes, if value is NULL) the named
//               property element in the object.  Returns true on
//               success, false on failure.
////////////////////////////////////////////////////////////////////
bool P3DToplevelObject::
set_property(const string &property, P3D_object *value) {
  if (_pyobj == NULL) {
    // Without a pyobj, we just store the value locally.
    if (value != NULL) {
      Properties::iterator pi;
      pi = _properties.insert(Properties::value_type(property, NULL)).first;
      assert(pi != _properties.end());
      P3D_object *orig_value = (*pi).second;
      if (orig_value != value) {
        P3D_OBJECT_XDECREF(orig_value);
        (*pi).second = value;
        P3D_OBJECT_INCREF(value);
      }
    } else {
      // Or delete the property locally.
      Properties::iterator pi;
      pi = _properties.find(property);
      if (pi != _properties.end()) {
        P3D_object *orig_value = (*pi).second;
        P3D_OBJECT_DECREF(orig_value);
        _properties.erase(pi);
      }
    }
    return true;
  }

  // With a pyobj, we pass this request down.
  return P3D_OBJECT_SET_PROPERTY(_pyobj, property.c_str(), value);
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::has_method
//       Access: Public, Virtual
//  Description: Returns true if the named method exists on this
//               object, false otherwise.
////////////////////////////////////////////////////////////////////
bool P3DToplevelObject::
has_method(const string &method_name) {
  if (_pyobj == NULL) {
    // No methods until we get our pyobj.
    return false;
  }

  return P3D_OBJECT_HAS_METHOD(_pyobj, method_name.c_str());
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::call
//       Access: Public, Virtual
//  Description: Invokes the named method on the object, passing the
//               indicated parameters.  If the method name is empty,
//               invokes the object itself.  Returns the return value
//               on success, NULL on error.
////////////////////////////////////////////////////////////////////
P3D_object *P3DToplevelObject::
call(const string &method_name, P3D_object *params[], int num_params) {
  if (_pyobj == NULL) {
    // No methods until we get our pyobj.
    return NULL;
  }

  return P3D_OBJECT_CALL(_pyobj, method_name.c_str(), params, num_params);
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::output
//       Access: Public, Virtual
//  Description: Writes a formatted representation of the value to the
//               indicated string.  This is intended for developer
//               assistance.
////////////////////////////////////////////////////////////////////
void P3DToplevelObject::
output(ostream &out) {
  out << "P3DToplevelObject";
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::set_pyobj
//       Access: Public
//  Description: Changes the internal pyobj pointer.  This is the
//               P3D_object that references the actual PyObject held
//               within the child process, corresponding to the true
//               toplevel object there.  The new object's reference
//               count is incremented, and the previous object's is
//               decremented.
////////////////////////////////////////////////////////////////////
void P3DToplevelObject::
set_pyobj(P3D_object *pyobj) {
  if (_pyobj != pyobj) {
    P3D_OBJECT_XDECREF(_pyobj);
    _pyobj = pyobj;
    if (_pyobj != NULL) {
      P3D_OBJECT_INCREF(_pyobj);

      // Now that we have a pyobj, we have to transfer down all of the
      // properties we'd set locally.
      Properties::const_iterator pi;
      for (pi = _properties.begin(); pi != _properties.end(); ++pi) {
        const string &property_name = (*pi).first;
        P3D_object *value = (*pi).second;
        P3D_OBJECT_SET_PROPERTY(_pyobj, property_name.c_str(), value);
        P3D_OBJECT_DECREF(value);
      }
      _properties.clear();
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: P3DToplevelObject::get_pyobj
//       Access: Public
//  Description: Returns the internal pyobj pointer, or NULL if it has
//               not yet been set.
////////////////////////////////////////////////////////////////////
P3D_object *P3DToplevelObject::
get_pyobj() const {
  return _pyobj;
}