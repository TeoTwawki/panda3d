// Filename: config_cull.cxx
// Created by:  drose (23Mar06)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#include "config_cull.h"

#include "cullBinBackToFront.h"
#include "cullBinFixed.h"
#include "cullBinFrontToBack.h"
#include "cullBinStateSorted.h"
#include "cullBinUnsorted.h"

#include "cullBinManager.h"
#include "dconfig.h"

ConfigureDef(config_cull);
NotifyCategoryDef(cull, "");

ConfigureFn(config_cull) {
  init_libcull();
}

////////////////////////////////////////////////////////////////////
//     Function: init_libcull
//  Description: Initializes the library.  This must be called at
//               least once before any of the functions or classes in
//               this library can be used.  Normally it will be
//               called by the static initializers and need not be
//               called explicitly, but special cases exist.
////////////////////////////////////////////////////////////////////
void
init_libcull() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;

  CullBinBackToFront::init_type();
  CullBinFixed::init_type();
  CullBinFrontToBack::init_type();
  CullBinStateSorted::init_type();
  CullBinUnsorted::init_type();

  CullBinManager *bin_manager = CullBinManager::get_global_ptr();
  bin_manager->register_bin_type(CullBinManager::BT_unsorted,
                                 CullBinUnsorted::make_bin);
  bin_manager->register_bin_type(CullBinManager::BT_state_sorted,
                                 CullBinStateSorted::make_bin);
  bin_manager->register_bin_type(CullBinManager::BT_back_to_front,
                                 CullBinBackToFront::make_bin);
  bin_manager->register_bin_type(CullBinManager::BT_front_to_back,
                                 CullBinFrontToBack::make_bin);
  bin_manager->register_bin_type(CullBinManager::BT_fixed,
                                 CullBinFixed::make_bin);
}
